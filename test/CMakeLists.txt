add_executable(test_bin main.cpp)
target_link_libraries(test_bin xvent)
set_property(TARGET test_bin PROPERTY CXX_STANDARD 20)

set(GTEST_LINK gtest gtest_main gmock gmock_main)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fconcepts")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -fconcepts")

include(GoogleTest)

function(add_ut bin files)
	add_executable(${bin} ${files})
	target_link_libraries(${bin} PUBLIC xvent ${GTEST_LINK})
	set_property(TARGET ${bin} PROPERTY CXX_STANDARD 20)
	gtest_add_tests(TARGET ${bin})
endfunction()

file (GLOB_RECURSE TEST_SRC ${CMAKE_CURRENT_SOURCE_DIR}/ut/*.cpp)

add_ut(xvent_unit_tests "${TEST_SRC}")

