#include "Logger.h"
#include <ctime>

Logger::Logger( const std::string &filename ) {
	file_.open( filename.c_str(), std::ios::trunc );
}

Logger::~Logger() {
    Out( "End log." );
	file_.close();
}

void Logger::Out( const std::string &o ) {
	file_ << std::time( nullptr ) << ' ' << o << '\n';
	std::cout << o << '\n';
}

void Logger::Error( const std::string &o ) {
	file_ << std::time( nullptr ) << ' ' <<o << '\n' << std::flush;
	std::cerr << o << '\n';
}
