cmake_minimum_required(VERSION 3.10)
project(xvent)

add_subdirectory(src)

if (XVENT_TEST_BUILD)
	enable_testing()
	add_subdirectory(3rdparty/googletest)
	add_subdirectory(test)
endif()

