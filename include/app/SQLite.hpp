#pragma once

#include <sqlite3.h>

class SQLite {
public:
    SQLite();
    ~SQLite();
private:
    sqlite3* database_;
};
