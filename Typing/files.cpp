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
	file.close();
	return true;
}

bool check_record(short* cpm, short* record) {
	std::ifstream record_in;
	std::ofstream record_out;
	record_in.open("record.bin", std::fstream::in | std::fstream::binary);
	if (!record_in.is_open()) {
		record_out.open("record.bin", std::fstream::out | std::fstream::trunc | std::fstream::binary);
		record_out.write((char*)cpm, sizeof(short));
		record_in.close(); record_out.close();
		*record = *cpm;
		return true;
	} else {
		short t;
		record_in.read((char*)&t, sizeof(short));
		if (t >= *cpm) {
			*record = t;
			record_in.close(); record_out.close();
			return false;
		} else {
			record_out.open("record.bin", std::fstream::out | std::fstream::trunc | std::fstream::binary);
			record_out.write((char*)cpm, sizeof(short));
			record_in.close(); record_out.close();
			*record = *cpm;
			return true;
		}
	}
}
