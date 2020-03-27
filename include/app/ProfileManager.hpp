#pragma once

#include <cstdint>
#include <string>
#include <map>

#include <SFML/Graphics.hpp>

#include "sql/Connection.hpp"

using ProfileId = std::uint32_t;

struct ProfileInfo {
    std::string name;
    sf::Color color;
};

class ProfileManager {
public:
    using Map = std::map<ProfileId, ProfileInfo>;

    explicit ProfileManager(sql::Connection& sql);

    Map& profiles() {
        return profiles_;
    }

private:
    void loadProfiles();
    sf::Color decode(int);

    sql::Connection& db_;

    Map profiles_;
};
