#ifndef LOG_H
#define LOG_H

#include <string>

class Log {
public:
	Log();
	~Log();
	static void appendText(const std::string msg);
	static void printText();
private:
	static std::string msg_log;
};

#endif