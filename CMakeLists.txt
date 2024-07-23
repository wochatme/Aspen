cmake_minimum_required(VERSION 3.28)

project(Aspen)

add_definitions(-D_UNICODE -DUNICODE -DNOMINMAX)

set(CMAKE_CXX_STANDARD 20)
add_subdirectory(win)
add_subdirectory(scintilla)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT aspen-win64)

