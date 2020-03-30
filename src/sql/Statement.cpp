#include "sql/Statement.hpp"

#include "app/Log.hpp"

namespace sql {

Statement::Statement(sqlite3* db, std::string_view query) {
    const char* unused;
    auto result = sqlite3_prepare_v2(db, query.data(), query.size(), &statement_, &unused);
    if (result != SQLITE_OK) {
        print::error("Failed to prepare statement {}, sqlite result = {}", query, result);
        // todo throw
    }
}

bool Statement::next() {
    auto result = sqlite3_step(statement_);
    if (result == SQLITE_DONE) return false;
    if (result == SQLITE_ROW) return true;
    print::error("bad result {}", result);
    return false;  // todo throw
}

template <>
std::string Statement::get(int colNum) {
    return reinterpret_cast<const char*>(sqlite3_column_text(statement_, colNum));
}

template <>
int Statement::get(int colNum) {
    return sqlite3_column_int(statement_, colNum);
}

}  // namespace sql
