#pragma once

#include <string>
#include <fstream>

enum class AspRatio {
    null,
    r_43,
    r_169,
    r_1610,
    r_54
};

enum class Resolution {
    null = 0,
    // 4:3
    r_1024_768,
    r_1280_600,
    r_1600_1200,
    // 16:9
    r_1280_720,
    r_1360_768,
    r_1600_900,
    r_1920_1080,
    r_2048_1152, // 2K
    r_4096_2304,  // 4K
    // 16:10
    r_1280_768,
    r_1280_800,
    r_1440_900,
    // 5:4
    r_1280_1024,
    r_1600_1280
};

inline AspRatio GetAspRatio(Resolution res);

class SettingsHandler {
public:
    SettingsHandler (const std::string &filename = "settings.dat");
    ~SettingsHandler  ();

    int getResW() const;
    int getResH() const;
    bool getFullscreen() const { return fullscreen_; }
    Resolution getRes() const { return resolution_; }
private:
    std::fstream file_;
    std::string filename_;

    Resolution resolution_;
    // Resolution resolution_, newRes_;
    bool fullscreen_;
};
