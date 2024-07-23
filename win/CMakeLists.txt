# the build for Windows X64
project(aspen-win64 CXX)

set(ZTERMAPP_DIR "${CMAKE_CURRENT_LIST_DIR}"
    CACHE PATH "Aspen application directory")

add_executable(${PROJECT_NAME} WIN32 
	Aspen.cpp
	stdafx.cpp
	Aspen.rc
	)

# Specify the precompiled header(s)
set(PRECOMPILE_HEADER_FILES stdafx.h)
target_precompile_headers(${PROJECT_NAME} PRIVATE ${PRECOMPILE_HEADER_FILES})

set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/wtl ${CMAKE_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} scintilla)

