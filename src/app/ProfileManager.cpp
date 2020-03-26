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
        print::info("player {} {}", id, name);
    }
}
