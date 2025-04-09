cmake_minimum_required(VERSION 3.8)

project("bleak")

add_library(bleak INTERFACE)

target_include_directories(bleak INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
