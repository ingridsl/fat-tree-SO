#include "log.h"

#include <iostream>

std::string Log::msg_log;

Log::Log(){
	msg_log = "";
}

Log::~Log(){ }

void Log::appendText(const std::string msg){
	msg_log.append(msg);
}

void Log::printText(){
	std::cout << msg_log;
}