#pragma once

#ifndef TYPE_H
#define TYPE_H

#include <iostream>
#include <sys/time.h>

extern std::string* words;
extern std::string* memory;
extern std::string typed;
extern std::size_t currWord;
extern std::size_t currChar;
extern struct timespec start_time, finish_time;
extern bool going;
extern short cpm, wpm;
extern unsigned char mistakes;
extern int millisecs;

bool define_words();
void delete_words();

void update_time();

void handle_input(const char& c);
bool main_loop();

void display_results();

void print_screen();

#endif // TYPE_H
