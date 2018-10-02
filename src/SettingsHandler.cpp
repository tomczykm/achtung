#include "SettingsHandler.hpp"
#include "Log.hpp"

inline AspRatio GetAspectRatio(Resolution res) {
    if (res == Resolution::null) return AspRatio::null;
    else if (res < Resolution::r_1280_720) return AspRatio::r_43;
    else if (res < Resolution::r_1280_768) return AspRatio::r_169;
    else if (res < Resolution::r_1280_1024) return AspRatio::r_1610;
    else return AspRatio::r_54;
}

SettingsHandler::SettingsHandler(const std::string &fname):
    filename_(fname),
    resolution_(Resolution::null),
    fullscreen_(false)
{
    file_.open(filename_.c_str(), std::ios::in | std::ios::binary);
    if (file_) {
        file_.read(reinterpret_cast<char*>(&resolution_), sizeof(Resolution));
        file_.read(reinterpret_cast<char*>(&fullscreen_), sizeof(bool));
        file_.close();
    } else {
        log_ << warning << "Failed to read settings - reverting to defaults";
        resolution_ = Resolution::r_1280_768;
        fullscreen_ = false;
    }
}

SettingsHandler::~SettingsHandler() {
    file_.open(filename_.c_str(), std::ios::out | std::ios::trunc | std::ios::binary);
    if (file_) {
        file_.write(reinterpret_cast<char*>(&resolution_), sizeof(Resolution));
        file_.write(reinterpret_cast<char*>(&fullscreen_), sizeof(bool));
        file_.close();
    } else {
        log_ << error << "Failed to save settings.";
    }
}

int SettingsHandler::getResW() const {
    switch (resolution_) {
    case Resolution::r_1024_768:
        return 1024;
    case Resolution::r_1280_1024:
    case Resolution::r_1280_600:
    case Resolution::r_1280_720:
    case Resolution::r_1280_768:
    case Resolution::r_1280_800:
        return 1280;
    case Resolution::r_1360_768:
        return 1360;
    case Resolution::r_1600_1200:
    case Resolution::r_1600_1280:
    case Resolution::r_1600_900:
        return 1600;
    case Resolution::r_1920_1080:
        return 1920;
    case Resolution::r_1440_900:
        return 1440;
    case Resolution::r_2048_1152:
        return 2048;
    case Resolution::r_4096_2304:
        return 4096;
    case Resolution::null:
    default:
        return -1;
    }
}

int SettingsHandler::getResH() const {
    switch (resolution_) {
    case Resolution::r_1024_768:
    case Resolution::r_1360_768:
    case Resolution::r_1280_768:
        return 768;
    case Resolution::r_1280_600:
        return 600;
    case Resolution::r_1600_1200:
        return 1200;
    case Resolution::r_1280_720:
        return 720;
    case Resolution::r_1600_900:
    case Resolution::r_1440_900:
        return 900;
    case Resolution::r_1920_1080:
        return 1080;
    case Resolution::r_2048_1152:
        return 1152;
    case Resolution::r_4096_2304:
        return 2304;
    case Resolution::r_1280_800:
        return 800;
    case Resolution::r_1280_1024:
        return 1024;
    case Resolution::r_1600_1280:
        return 1280;
    case Resolution::null:
    default:
        return -1;
    }
}
