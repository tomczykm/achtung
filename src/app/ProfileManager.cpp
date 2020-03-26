#include "app/ProfileManager.hpp"

#include "app/Log.hpp"
#include "sql/Connection.hpp"

ProfileManager::ProfileManager(sql::Connection& sql):
    db_(sql)
{
    loadProfiles();
}

void ProfileManager::loadProfiles() {
    print::info("{}", __func__);
    constexpr auto query = "SELECT * FROM Players;";
    auto s = db_.prepare(query);
    while (s.next()) {
        auto id = s.get<int>(0);
        auto name = s.get<std::string>(1);
        auto color = s.get<int>(2);
        print::info("player {} \"{}\" {}", id, name, color);
        profiles_.emplace(id, ProfileInfo{name, decode(color)});
    }
}

sf::Color ProfileManager::decode(int color) {
    auto blue = color & 0xff;
    auto green = (color >> 8) & 0xff;
    auto red = (color >> 16) & 0xff;
    return sf::Color{red, green, blue};
}
