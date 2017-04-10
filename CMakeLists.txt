cmake_minimum_required (VERSION 3.1)
project (distance_stats)
set (CMAKE_CXX_STANDARD 11)

set(JSONCONS_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src/jsoncons/src)
set(OTHER_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src)

add_definitions(-D_SCL_SECURE_NO_WARNINGS)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

include_directories(${JSONCONS_INCLUDE_DIR} ${OTHER_INCLUDE_DIR} ${PROJECT_BINARY_DIR})

add_executable (distance_stats "${CMAKE_SOURCE_DIR}/src/distance_stats.cpp")
add_executable (global_stats "${CMAKE_SOURCE_DIR}/src/global_stats.cpp")
add_executable (trip_stats "${CMAKE_SOURCE_DIR}/src/trip_stats.cpp")

install (TARGETS distance_stats global_stats trip_stats
	DESTINATION ${CMAKE_SOURCE_DIR}/bin/)



