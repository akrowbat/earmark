cmake_minimum_required(VERSION 3.10)
project(earmark LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(earmark main.cpp)

# Include libmagic
find_path(LIBMAGIC_INCLUDE_DIR magic.h REQUIRED)
find_library(LIBMAGIC_LIBRARY magic REQUIRED)
target_include_directories(earmark PRIVATE ${LIBMAGIC_INCLUDE_DIR})
target_link_libraries(earmark PRIVATE ${LIBMAGIC_LIBRARY})

# Include TagLib headers
target_include_directories(earmark PRIVATE /usr/local/include/taglib)

# Link TagLib and zlib
target_link_directories(earmark PRIVATE /usr/local/lib)
find_package(ZLIB REQUIRED)
target_link_libraries(earmark PRIVATE tag ZLIB::ZLIB)

install(TARGETS earmark RUNTIME DESTINATION bin)
