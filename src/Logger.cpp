#include "inc/Logger.h"
#include <ctime>

CLogger::CLogger( const std::string &filename ) {
	file.open( filename.c_str(), std::ios::trunc );
}

CLogger::~CLogger() {
    Out( "End log." );
	file.close();
}

void CLogger::Out( const std::string &o ) {
	file << time( nullptr ) << ' ' << o << '\n';
	std::cout << o << '\n';
}

void CLogger::Error( const std::string &o ) {
	file << time( nullptr ) << ' ' <<o << '\n' << std::flush;
	std::cerr << o << '\n';
}
