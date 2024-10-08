project(base)

add_definitions(-D_UNICODE -DUNICODE -DNOMINMAX)

file(GLOB BASE_SRC 
	*.cpp
	debug/*.cpp
	)

source_group("Source Files\\debug" REGULAR_EXPRESSION "debug/.*\\.cpp")
# source_group("Source Files\\i18n" REGULAR_EXPRESSION "i18n/.*\\.cpp")
# source_group("Source Files\\icu" REGULAR_EXPRESSION "icu/.*\\.cpp")
# source_group("Source Files\\memory" REGULAR_EXPRESSION "memory/.*\\.cpp")
# source_group("Source Files\\metric" REGULAR_EXPRESSION "metric/.*\\.cpp")
# source_group("Source Files\\synchronization" REGULAR_EXPRESSION "synchronization/.*\\.cpp")
# source_group("Source Files\\threading" REGULAR_EXPRESSION "threading/.*\\.cpp")
# source_group("Source Files\\system_monitor" REGULAR_EXPRESSION "system_monitor/.*\\.cpp")
# source_group("Source Files\\win" REGULAR_EXPRESSION "win/.*\\.cpp")

add_library(${PROJECT_NAME} ${BASE_SRC})

set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_include_directories(${PROJECT_NAME} 
    PUBLIC ${PROJECT_SOURCE_DIR} 
           ${CMAKE_SOURCE_DIR}/chromium
           ${CMAKE_SOURCE_DIR}/chromium/gen
           )
           
