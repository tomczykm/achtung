#pragma once

#include <string>
#include <fstream>
#include <iostream>

class CLogger {
public:
	CLogger( const std::string &filename = "event.log" );
	~CLogger();

	void Out( const std::string &o );
	void Error( const std::string &o );
private:
	std::ofstream file;
};
