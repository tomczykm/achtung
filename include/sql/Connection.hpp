#pragma once

#include <sqlite3.h>

#include "sql/Statement.hpp"

namespace sql {

class Connection {
public:
    Connection();
    ~Connection();

    Statement prepare(std::string_view query);
private:
    sqlite3* db_;
};

}  // namespace sql
