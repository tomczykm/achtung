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
        print::info("profile {} \"{}\" {}", id, name, color);
        profiles_.emplace(id, ProfileInfo{name, decodeColor(color)});
    }
}

void ProfileManager::updateProfile(std::optional<ProfileId>& id, const ProfileInfo& p) {
    bool isNewProfile = !id;
    const std::string_view query = makeUpdateQuery(isNewProfile);
    if (isNewProfile) {
        id = makeNewId();
        profiles_.emplace(*id, p);
    } else {
        profiles_[*id] = p;
    }
    auto statement = db_.prepare(query);
    statement.bind(1, *id);
    statement.bind(2, p.name);
    statement.bind(3, p.color.toInteger() >> 8);  // shifted because of alpha, not stored in db
    statement.execute();
}

void ProfileManager::deleteProfile(ProfileId id) {
    profiles_.erase(id);
    const std::string_view query = "DELETE FROM Players WHERE id = ?1;";
    auto statement = db_.prepare(query);
    statement.bind(1, id);
    statement.execute();
}

std::string_view ProfileManager::makeUpdateQuery(bool isNewProfile) {
    if (isNewProfile) {
        return "INSERT INTO Players VALUES(?1, ?2, ?3);";
    } else {
        return "UPDATE Players SET name = ?2, color = ?3 WHERE id = ?1;";
    }
}

ProfileId ProfileManager::makeNewId() {
    return profiles_.size() == 0 ? 0 : profiles_.rbegin()->first + 1;
}

sf::Color ProfileManager::decodeColor(int color) {
    return sf::Color{(color << 8) | 0xff};
}
