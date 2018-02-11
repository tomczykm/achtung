#pragma once

#include <string>
#include <fstream>

enum e_ResAspectRatio {
    RATIO_NULL,
    RATIO_43,
    RATIO_169,
    RATIO_1610,
    RATIO_54
};

enum e_Resolution {
    R_NULL = 0,
    //4:3
    R_1024_768,
    R_1280_600,
    R_1600_1200,
    //16:9
    R_1280_720,
    R_1360_768,
    R_1600_900,
    R_1920_1080,
    R_2048_1152, //2K
    R_4096_2304,  //4K
    //16:10
    R_1280_768,
    R_1280_800,
    R_1440_900,
    //5:4
    R_1280_1024,
    R_1600_1280
};

inline e_ResAspectRatio GetResAspectRatio( e_Resolution res );

class CSettingsHandler {
public:
	CSettingsHandler	( const std::string &filename = "settings.dat" );
	~CSettingsHandler	();

    int                 GetResW() const;
    int                 GetResH() const;
    bool                GetFullscreen() const { return mFullscreen; }
	e_Resolution        GetRes() const { return mResolution; }
private:
	//pomocnicze skladowe
	std::fstream		mFile;
	std::string         mFilename;
	//faktyczne ustawienia

	e_Resolution        mResolution,
                        mNewRes;

    bool                mFullscreen;
};

