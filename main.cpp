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
std::ofstream read;
std::string words[116];
std::string wordsTypedMem[100];

int getCPM_WPM(const long& seconds) {
	charsCorrect = charsTyped;
	int errorC = 0;

	for (int i = 0; i < wordsTyped; ++i) {
		if (words[i] != wordsTypedMem[i]) {
			charsCorrect -= (words[i].length());
			++errorC;
		}
	}

	cpm = std::max(0,(int)((charsCorrect / ((double)seconds) * 60)));
	wpm = std::max(0,cpm / 5);
}

int main(int argc, char* argv[]) {

	if (argc > 1) {
		if (argv[1] == "-r") {
			in.open("records");
			in >> currWord;
			std::cout << currWord << std::endl;
			return 0;
		}
	}

	std::string temp;

	struct timespec start, endt;
	long seconds = 0;
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
	init_pair(6, COLOR_CYAN, COLOR_BLACK);
	
	int cc = 0;

	while (currWord < 100) {
		clear();
		printw("\n");
		error = false;
		int divided = currWord / 8;
		int moded   = currWord - (divided * 8);

		printw("            %s\n",typed.c_str());

		for (int i = 0; i < moded; ++i) {
			if (wordsTypedMem[i+(divided*8)] != words[i+(divided*8)]) {
				attron(COLOR_PAIR(3));
				printw("%s", words[i+(divided*8)].c_str());
				attroff(COLOR_PAIR(3));
				printw(" ");
			} else {
				printw("%s ", words[i+(divided*8)].c_str());
			}
		}
		for (int f = 0; f < typed.length(); ++f) {
			if (typed[f] != words[currWord][f]) {
				attron(COLOR_PAIR(3));
				error = true;
				printw("%c",words[currWord][f]);	
				attroff(COLOR_PAIR(3));
			} else {
				attron(COLOR_PAIR(2));
				printw("%c",words[currWord][f]);
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
		attron(COLOR_PAIR(5));
		printw("\n\n%i words left   %i CPM : %i WPM   %is", 100 - currWord, cpm, wpm, seconds);
		attroff(COLOR_PAIR(5));
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
			clock_gettime(CLOCK_MONOTONIC, &endt);
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
					getCPM_WPM(seconds);
				} else {
					++currChar;
					++charsTyped;
					typed += tempc;
				}
			}
			seconds = endt.tv_sec - start.tv_sec;
		}

		if (currWord == 100) {
			break;	
		}
	}

	clock_gettime(CLOCK_MONOTONIC, &endt);
	seconds = endt.tv_sec - start.tv_sec;

	charsCorrect = charsTyped;

	for (int i = 0; i < 100; ++i) {
		if (words[i] != wordsTypedMem[i]) {
			charsCorrect -= (words[i].length());
			++errorCount;
		}
	}

	bool record = false;

	cpm = std::max(0,(int)((charsCorrect / (double)seconds) * 60));
	wpm = std::max(0,cpm / 5);

	currWord = 0;
	in.close();
	in.open ("/usr/bin/tprecords");
	in >> currWord; // reuse of currWord to save memory
	if (currWord < cpm) {
		read.open("/usr/bin/tprecords", std::ofstream::out | std::ofstream::trunc);
		read << cpm;
		read.close();
		record = true;
		currWord = cpm;
	}
	in.close();

	refresh();
	endwin();

	std::cout << "\n100 words took " << seconds << " seconds"
	<< "\nCharacters per minute: " << cpm << (record ? " NEW RECORD":"") 
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

	// currWord at this point is used to hold the record
	std::cout << "Record: " << currWord <<" CPM (" <<currWord/5<<" WPM)"<< std::endl;
	
	return 0;
}
