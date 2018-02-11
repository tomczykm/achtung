#include "inc/SettingsHandler.h"
#include "inc/Global.h"

/**

Globalne

**/

inline e_ResAspectRatio GetResAspectRatio( e_Resolution res ) {
    if( res == R_NULL ) return RATIO_NULL;
    else if( res < R_1280_720 ) return RATIO_43;
    else if( res < R_1280_768 ) return RATIO_169;
    else if( res < R_1280_1024 ) return RATIO_1610;
    else return RATIO_54;
}
/**

Klasowe

**/

CSettingsHandler::CSettingsHandler( const std::string &fname ):
    mFilename( fname ),
    mResolution( R_NULL ),
    mFullscreen( false )
{
    logger->Out( "Reading settings..." );
	mFile.open( mFilename.c_str(), std::ios::in | std::ios::binary );
	if( mFile ) {
        mFile.read( reinterpret_cast< char* >( &mResolution ), sizeof( e_Resolution ) );
        mFile.read( reinterpret_cast< char* >( &mFullscreen ), sizeof( bool ) );
        mFile.close();
        logger->Out( "Done" );
	} else {
	    logger->Error( "ERROR: Failed to read settings - reverting to defaults" );
        //brak pliku/inny blad otwarcia
        //bierzemy domyslne ustawienia
        mResolution = R_1280_768;
        mFullscreen = false;
	}
}

CSettingsHandler::~CSettingsHandler() {
    logger->Out( "Saving settings..." );
    mFile.open( mFilename.c_str(), std::ios::out | std::ios::trunc | std::ios::binary );
    if( mFile ) {
        mFile.write( reinterpret_cast< char* >( &mResolution ), sizeof( e_Resolution ) );
        mFile.write( reinterpret_cast< char* >( &mFullscreen ), sizeof( bool ) );
        mFile.close();
        logger->Out( "Done." );
    } else {
        logger->Error( "ERROR: Failed to save settings." );
    }
}

int CSettingsHandler::GetResW() const {
    switch( mResolution ) {
    case R_1024_768:
        return 1024;
    case R_1280_1024:
    case R_1280_600:
    case R_1280_720:
    case R_1280_768:
    case R_1280_800:
        return 1280;
    case R_1360_768:
        return 1360;
    case R_1600_1200:
    case R_1600_1280:
    case R_1600_900:
        return 1600;
    case R_1920_1080:
        return 1920;
    case R_1440_900:
        return 1440;
    case R_2048_1152:
        return 2048;
    case R_4096_2304:
        return 4096;
    case R_NULL:
    default:
        return -1;
    }
}

int CSettingsHandler::GetResH() const {
    switch( mResolution ) {
    case R_1024_768:
    case R_1360_768:
    case R_1280_768:
        return 768;
    case R_1280_600:
        return 600;
    case R_1600_1200:
        return 1200;
    case R_1280_720:
        return 720;
    case R_1600_900:
    case R_1440_900:
        return 900;
    case R_1920_1080:
        return 1080;
    case R_2048_1152:
        return 1152;
    case R_4096_2304:
        return 2304;
    case R_1280_800:
        return 800;
    case R_1280_1024:
        return 1024;
    case R_1600_1280:
        return 1280;
    case R_NULL:
    default:
        return -1;
    }
}
