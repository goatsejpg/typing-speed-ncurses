#include <fstream>

#include "files.h"

bool rand_words(std::string* words, const int& WORDS_AMOUNT) {
	std::ifstream file;
	file.open("words.txt");
	if (!file.is_open()) {
		return false;
	}
	for (std::string* s = words; s != words + WORDS_AMOUNT; ++s) {
		int i =  rand() % 1000;
		std::string temp;
		while (i != 0) {
			--i;
			file >> temp;
		}
		file >> *s;
		file.clear();
		file.seekg(0, std::ios::beg);
	}
}

bool check_record(short* cpm) {
	std::ifstream record_in;
	std::ofstream record_out;
	record_in.open("record.bin");
	if (!record_in.is_open()) {
		record_out.open("record.bin", std::fstream::out | std::fstream::trunc);
		record_out.write((char*)cpm, 2);
		return true;
	} else {
		short t;
		record_in.read((char*)&t, 2);
		if (t >= cpm) 
			return false;
		else {
			record_out.open("record.bin", std::fstream::out | std::fstream::trunc);
			record_out.write((char*)&cpm, 2);
			return true;
		}
	}
}
