#pragma once

#include <string_view>
#include <type_traits>

#include <sqlite3.h>

template <class T>
struct remove_cvref {
    typedef std::remove_cv_t<std::remove_reference_t<T>> type;
};

template <class T>
using remove_cvref_t = typename remove_cvref<T>::type;

namespace sql {

class Statement {
public:
    bool next();
    void execute() { next(); }  // alias for those one-shot statements

    template <typename T>
    T get(int colNum);

    template <typename T, std::enable_if_t<std::is_convertible_v<T, int>, int> = 0>
    void bind(int id, T val) {
        sqlite3_bind_int(statement_, id, val);
    }

    template <typename T, std::enable_if_t<std::is_convertible_v<T, std::string_view>, int> = 0>
    void bind(int id, T&& val) {
        auto sv = std::string_view{val};
        sqlite3_bind_text(statement_, id, sv.data(), sv.size(), SQLITE_STATIC);
    }

private:
    friend class Connection;
    Statement(sqlite3* db, std::string_view query);

    sqlite3_stmt* statement_;
};

}  // sql
