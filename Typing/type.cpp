#include <iostream>
#include <ncurses.h>
#include <random>
#include <string>

const int WORDS_PER_LINE = 8;
const int WORDS_AMOUNT = 100;

#include "ncursesfuncs.h"
#include "type.h"
#include "files.h"

std::string* words;
std::string* memory;
std::string typed = "";
std::size_t currWord = 0;
std::size_t currChar = 0;
struct timespec start_time, finish_time;
short cpm, wpm;
unsigned char mistakes;
int millisecs;
bool going = true;

bool define_words() {
	words = new std::string[WORDS_AMOUNT];
	memory = new std::string[WORDS_AMOUNT];
	return (rand_words(words, WORDS_AMOUNT));
}

void update_time() {
	clock_gettime(CLOCK_REALTIME, &finish_time);
	millisecs = ((finish_time.tv_sec - start_time.tv_sec) * 1000) + 
	            ((finish_time.tv_nsec - start_time.tv_nsec) / 1000000);
}

void update_stats() {
	int charsCorrect = 0;
	mistakes = 0;
	for (std::size_t s = 0; s < currWord; ++s) {
		if (words[s] == memory[s])
			charsCorrect += words[s].length() + 1;
		else {
			charsCorrect -= words[s].length() + 1;
			++mistakes;
		}
	}
	if (charsCorrect < 0) charsCorrect = 0;
	cpm = static_cast<short>(60.0 * ((double)charsCorrect / ((double)millisecs / 1000.0)));
	wpm = cpm / 5;
}

void delete_words() {
	delete[] words;
	delete[] memory;
}

void print_screen() {
	clear();
	unsigned char temp = 8 * (currWord / 8);
	print("       " + typed, 1);
	newline();
	for (std::size_t i = temp; i < temp + WORDS_PER_LINE && i < WORDS_AMOUNT; ++i) {
		if (i != currWord) {
			print(words[i],
			words[i] == memory[i] ? 3 :
			i < currWord ? 4 : 1);
			print(' ', 1);
		} else {
			for (std::size_t c = 0; c < words[i].length(); ++c) {
				if (c < currChar)
					print(words[i][c],
					words[i][c] == typed[c] ? 3 : 4);
				else if (c == currChar)
					print(words[i][c], 2);
				else
					print(words[i][c], 1);
			}
			print(' ', 1);
		}
	}
	newline();
	for (std::size_t i = temp + WORDS_PER_LINE; i < temp + WORDS_PER_LINE*2 && i < WORDS_AMOUNT; ++i) {
		print(words[i] + " ", 1);
	}
	newline();
	print(
	std::to_string(100 - currWord)+" words remaining;   CPM: "+std::to_string(cpm)+
	"("+std::to_string(wpm)+" WPM)    "+std::to_string(millisecs/1000)+" seconds", 5);
	refresh();
}

void handle_input(const char& c) {
	switch (c) {
	case 0x20: // space
		*(memory + currWord) = typed;
		typed = "";
		++currWord;
		currChar = 0;
		update_stats();
		break;
	case 0x7f: // backspace
		if (typed.length() != 0) {
			typed.pop_back();
			--currChar;
		} else {
			if (currWord == 0) break;
			*(memory + currWord) = "";
			--currWord;
			typed = *(memory + currWord);
			currChar = typed.length() - 1;
		}
		break;
	case 0x1b: // escape
		going = false;
		break;
	default: // everthing else
		if (c < 33 || c > 126) break;
		typed += c;
		++currChar;
		break;
	}
	update_time();
}

bool main_loop() {
	print_screen();
	handle_input(static_cast<char>(getch()));
	clock_gettime(CLOCK_REALTIME, &start_time);

	while (going && currWord < 100) {
		print_screen();
		handle_input(static_cast<char>(getch()));
	}
	update_time();
	if (currWord < 100) return false;
	else return true;
}

void display_results() {
	short record;
	std::cout << "Results: " << std::endl << "CPM: " << cpm << std::flush;
	if (check_record(&cpm, &record)) {
		std::cout << " (NEW RECORD!)" << std::endl; 
	} else { 
		std::cout << std::endl;
	}
	std::cout << "WPM: " << wpm << std::endl
	<< (int)mistakes << " mistakes" << std::endl
	<< "Record: " << record << std::endl;
}
