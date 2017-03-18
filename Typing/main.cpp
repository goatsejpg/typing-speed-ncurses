#include <iostream>
#include <fstream>
#include <ncurses.h>
#include <random>
#include <time.h>

#include "ncursesfuncs.h"
#include "type.h"
#include "files.h"

int main(int argc, char* argv[]) {
	srand(time(NULL));
	setlocale(LC_ALL, "");
	initscr();
	start_color();
	define_colors();
	
	cbreak();
	noecho();
	intrflush(stdscr, FALSE);

	if (!define_words()) {
		endwin();
		std::cerr << "Error" << std::endl; 
		delete_words();
		return -1;
	}

	if (main_loop()) {
		endwin();
		display_results();
	} else endwin();

	delete_words();
	return 0;
}
