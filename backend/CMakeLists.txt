cmake_minimum_required(VERSION 3.15)

project(MultiVendorMarketplace)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Backend executable
add_executable(marketplace
    src/main.cpp
)

# SQLite
find_package(SQLite3 REQUIRED)

target_link_libraries(marketplace
    SQLite::SQLite3
)
