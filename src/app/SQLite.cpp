#include "app/SQLite.hpp"

namespace {

constexpr auto dbFilename = "freeachtung.db";

}

SQLite::SQLite() {
    sqlite3_open(dbFilename, &database_);
}

SQLite::~SQLite() {
    sqlite3_close(database_);
}
