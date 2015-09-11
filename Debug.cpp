#include "Debug.h"

Debug::Debug() {
	file = new std::ofstream();
	file->open("log.txt");
}

Debug::~Debug() {
	delete file;
}

void Debug::write(std::string toWrite) {
	*file << toWrite.c_str();
	*file << "\n";
}
