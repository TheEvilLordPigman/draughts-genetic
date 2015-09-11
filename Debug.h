#ifndef DEBUG_H_
#define DEBUG_H_

#include <fstream>
#include <ctime>

class Debug {
private:
	std::ofstream* file;
public:
	Debug();
	~Debug();
	void write(std::string toWrite);
};

#endif
