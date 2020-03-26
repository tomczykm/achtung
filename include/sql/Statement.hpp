#pragma once

#include <string_view>

#include <sqlite3.h>

namespace sql {

class Statement {
public:
    bool next();

    template <typename T>
    T get(int colNum);

private:
    friend class Connection;
    Statement(sqlite3* db, std::string_view query);

    sqlite3_stmt* statement_;
};

}  // sql
