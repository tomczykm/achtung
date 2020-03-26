#include "sql/Connection.hpp"

#include "app/AssetManager.hpp"
#include "app/Log.hpp"

namespace sql {
namespace {

constexpr auto dbFilename = "freeachtung.db";

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
