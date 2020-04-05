add_subdirectory(fmt)

option(toml11_BUILD_TEST OFF)
add_subdirectory(toml11)

if (NOT MSVC)
    add_subdirectory(googletest)
endif()
