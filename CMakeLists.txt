cmake_minimum_required(VERSION 3.10)
project(xvent)

enable_testing()

add_subdirectory(3rdparty/googletest)
add_subdirectory(src)
add_subdirectory(test)

