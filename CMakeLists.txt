# CMakeList.txt : CMake project for SRTEditor, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project(SRTEditor)

set(CMAKE_CXX_STANDARD 17)

find_package(unofficial-date CONFIG REQUIRED)
add_executable (SRTEditor "main.cpp")

# Add CoRoutines  Support
# Check compiler options for Clang-Cl
# Clang-Cl is a Special MSVC Driver for Clang
# Also specify same values for MSVC
if(("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") AND ("${CMAKE_CXX_SIMULATE_ID}" STREQUAL "MSVC"))
		target_compile_options(SRTEditor PRIVATE /Zc:__cplusplus /permissive- /Zc:twoPhase -Xclang -fcoroutines-ts -Xclang -Wpedantic)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
		target_compile_options(SRTEditor PRIVATE /Zc:__cplusplus /W3 /permissive- /await)
elseif(("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang|GNU"))
	target_compile_options(SRTEditor PRIVATE -Wall -Wextra -Wpedantic)
endif()
target_link_libraries(SRTEditor PRIVATE unofficial::date::date)

# TODO: Add tests and install targets if needed.
