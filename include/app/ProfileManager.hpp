#pragma once

#include <cstdint>
#include <string>
#include <map>

#include <SFML/Graphics.hpp>

#include "sql/Connection.hpp"

struct ProfileInfo {
    std::string name;
    sf::Color color;
};

class ProfileManager {
public:
    using ProfileId = std::uint32_t;

    explicit ProfileManager(sql::Connection& sql);

    ProfileInfo& operator[](ProfileId id) {
        return profiles_[id];
    }

private:
    void loadProfiles();

    sql::Connection& db_;

    std::map<ProfileId, ProfileInfo> profiles_;
};
