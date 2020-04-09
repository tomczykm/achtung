#include "sql/Connection.hpp"

#include "app/AssetManager.hpp"
#include "app/Log.hpp"

using namespace std::literals;

namespace sql {
namespace {

constexpr auto dbFilename = "freeachtung.db"sv;

}

Connection::Connection() {
    sqlite3_open(AssetManager::getActualFileName(dbFilename).c_str(), &db_);
}

Connection::~Connection() {
    sqlite3_close(db_);
}

Statement Connection::prepare(std::string_view query) {
    return Statement{db_, query};
}

}  // namespace sql
