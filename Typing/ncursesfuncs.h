#pragma once

#ifndef NCURSES_FUNCS_H
#define NCURSES_FUNCS_H

#include <iostream>
#include <ncurses.h>

void define_colors();

void print(const char& c, const int& color_pair);
void print(const char* c, const int& color_pair);
void print(const std::string& c, const int& color_pair);
void newline();
 
#endif // NCURSES_FUNCS_H
