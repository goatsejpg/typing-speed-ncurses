#include <ncurses.h>

#include "ncursesfuncs.h"

void define_colors() {
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	init_pair(4, COLOR_WHITE, COLOR_RED);
	init_pair(5, COLOR_CYAN, COLOR_BLACK);
}

void print(const char& c, const int& color_pair) {
	attron(COLOR_PAIR(color_pair));
	printw("%c", c);
	attroff(COLOR_PAIR(color_pair));
}

void print(const char* c, const int& color_pair) {
	attron(COLOR_PAIR(color_pair));
	printw("%s", c);
	attroff(COLOR_PAIR(color_pair));
}

void print(const std::string& c, const int& color_pair) {
	attron(COLOR_PAIR(color_pair));
	printw("%s", c.c_str());
	attroff(COLOR_PAIR(color_pair));
}

void newline() {
	printw("%c", '\n');
}
