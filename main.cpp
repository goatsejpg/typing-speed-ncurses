#include <iostream>
#include <sys/time.h>
#include <vector>
#include <string>
#include <fstream>
#include <ncurses.h>

std::string typed = "";
int currWord = 0, currChar = 0;
short wordsTyped = 0, charsTyped = 0, charsCorrect = 0;
short wpm = 0, cpm = 0;
double accuracy = 100.0;
int errorCount = 0;
bool started = false;
bool error = false;
double tempc;

std::ifstream in;
std::string words[116];
std::string wordsTypedMem[100];

int main(int argc, char* argv[]) {
	std::string temp;

	struct timespec start, endt;
	long seconds, nseconds;
	double mtime;

	srand(time(NULL));

	for (int i = 0; i < 100; ++i) {
		in.open("/usr/bin/typingspeedwords");
		int tempi = rand() % 850;
		for (int j = 0; j <= tempi; ++j) {
			in >> temp;
		}
		in.close();
		words[i] = temp;
	}
	for (int i = 100; i < 108; ++i) {
		words[i] = ""; // BUFFER
	}

	initscr();
	noecho();
	raw();
	start_color();

	init_color(COLOR_RED, 1000, 0, 0);
	init_color(COLOR_BLACK,0,0,0);
	init_color(COLOR_GREEN,0,1000,0);
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_WHITE, COLOR_RED);
	init_pair(4, COLOR_WHITE, COLOR_CYAN);
	init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
	


	while (true) {
		clear();
		printw("\n");
		error = false;
		int divided = currWord / 8;
		int moded   = currWord - (divided * 8);

		for (int i = 0; i < moded; ++i) {
			if (wordsTypedMem[i+(divided*8)] != words[i+(divided*8)]) {
				attron(COLOR_PAIR(3));
				printw("%s ", words[i+(divided*8)].c_str());
				attroff(COLOR_PAIR(3));
			} else {
				printw("%s ", words[i+(divided*8)].c_str());
			}
		}
		for (int f = 0; f < typed.length(); ++f) {
			if (typed[f] != words[currWord][f]) {
				attron(COLOR_PAIR(3));
				error = true;
				printw("%c",typed[f]);	
				attroff(COLOR_PAIR(3));
			} else {
				attron(COLOR_PAIR(2));
				printw("%c",typed[f]);
				attroff(COLOR_PAIR(2));
			}
		}

		attron(COLOR_PAIR(5));
		std::string temps;
		for (auto v = words[currWord].begin() + currChar; v < words[currWord].end(); ++v) {
			temps = temps + *v;
		}
		printw("%s ",temps.c_str());
		attroff(COLOR_PAIR(5));

		for (auto a = std::begin(words) + currWord + 1; a != std::begin(words) + (divided * 8) + 8; ++a) {
			printw("%s ", a->c_str());
		}
		printw("\n");
		for (auto a = std::begin(words) + (divided*8) + 8; a != std::begin(words) + (divided*8) + 16; ++a) {
			printw("%s ", a->c_str());
		}

		printw("\n\n%i words left", 100 - currWord);
		refresh();

		if (not started) {
			tempc = getch();
			if (tempc != 127 && tempc != ' ') {
				started = true;
				typed += tempc;
				++currChar; ++charsTyped;
				clock_gettime(CLOCK_MONOTONIC, &start);
			}
		} else {
			tempc = getch();
			if (tempc == KEY_ENTER) {
				return 0;
			}
			if (tempc == 127) {
				if (currChar == 0 && currWord > 0) {
					--currWord;
					--charsTyped;
					currChar = wordsTypedMem[currWord].length();
					--wordsTyped;
					typed = wordsTypedMem[currWord];
					wordsTypedMem[currWord+1] == "";
				} else {
					if (currChar == 0 && currWord == 0) {
					} else {
						--charsTyped;
						--currChar;
						typed.pop_back();
					}
				}
			} else {
				if (tempc == ' ') {
					++charsTyped;
					++wordsTyped;
					currChar = 0;
					++currWord;
					wordsTypedMem[currWord-1] = typed;
					typed = "";
				} else {
					++currChar;
					++charsTyped;
					typed += tempc;
					beep();
				}
			}
		}
		
		if (currWord == 100) {
			break;	
		}
	}

	clock_gettime(CLOCK_MONOTONIC, &endt);
	seconds = endt.tv_sec - start.tv_sec;
	nseconds = endt.tv_nsec - start.tv_nsec;

	charsCorrect = charsTyped;

	for (int i = 0; i < 100; ++i) {
		if (words[i] != wordsTypedMem[i]) {
			charsCorrect -= (words[i].length());
			++errorCount;
		}
	}

	cpm = std::max(0,(int)((charsCorrect / ((double)seconds) * 60)));
	wpm = std::max(0,cpm / 5);
	refresh();
	endwin();

	std::cout << "\n100 words took " << seconds << " seconds"
	<< "\nCharacters per minute: " << cpm 
	<< "\nWords per minute: " << wpm << 
	"\nWords Misspelt: " << errorCount << "/" << wordsTyped;
	if (errorCount == 0) {
		std::cout << " PERFECT";
	}

	std::cout << "\nRANK: ";

	if (cpm < 25) {
		std::cout << "F-" << std::endl;
	}
	if (cpm < 60 && cpm >= 25) {
		std::cout << "F" << std::endl;
	}
	if (cpm < 110 && cpm >= 60) {
		std::cout << "E" << std::endl;
	}
	if (cpm < 170 && cpm >= 110) {
		std::cout << "D" << std::endl;
	}
	if (cpm < 230 && cpm >= 170) {
		std::cout << "D+" << std::endl;
	}
	if (cpm < 290 && cpm >= 230) {
		std::cout << "C" << std::endl;
	}
	if (cpm < 350 && cpm >= 290) {
		std::cout << "B" << std::endl;
	}
	if (cpm < 400 && cpm >= 350) {
		std::cout << "B+" << std::endl;
	}
	if (cpm < 460 && cpm >= 400) {
		std::cout << "A" << std::endl;
	}
	if (cpm < 510 && cpm >= 460) {
		std::cout << "A+" << std::endl;
	}
	if (cpm < 570 && cpm >= 510) {
		std::cout << "S" << std::endl;
	}
	if (cpm < 640 && cpm >= 570) {
		std::cout << "SS" << std::endl;
	}
	if (cpm >= 640) {
		std::cout << "SS+" << std::endl;
	}

	std::cout << std::endl;
	
	return 0;
}
