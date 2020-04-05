#pragma once

#include <cstdint>
#include <optional>
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

    const Map& profiles() {
        return profiles_;
    }

    const ProfileInfo& operator[](ProfileId id) {
        return profiles_[id];
    }

    void updateProfile(std::optional<ProfileId>&, const ProfileInfo&);
    void deleteProfile(ProfileId);

private:
    void loadProfiles();

    ProfileId makeNewId();
    std::string_view makeUpdateQuery(bool isNewProfile);

    sf::Color decodeColor(std::uint32_t);

    sql::Connection& db_;

    Map profiles_;
};
