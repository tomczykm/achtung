#pragma once

#include <string>
#include <fstream>
#include <iostream>

class Logger {
public:
	Logger( const std::string &filename = "event.log" );
	~Logger();

	void Out( const std::string &o );
	void Error( const std::string &o );
private:
	std::ofstream file_;
};
