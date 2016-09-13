# inspired by https://github.com/toeb/moderncmake

cmake_minimum_required(VERSION 3.3)

project(except)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

add_library(except_lib
    include/except/except.hpp
    src/except.cpp
)

target_include_directories(except_lib PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
)

if (MSVC)
    target_compile_options(except_lib PUBLIC "/EHa")
else()
    target_compile_options(except_lib PUBLIC "-fnon-call-exceptions")
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  target_link_libraries(except_lib INTERFACE "-lstdc++")
endif()

option(USE_POSIX_SIGNAL "Use the portable, but non thread safe, POSIX signal" OFF)
option(USE_WINDOWS_SE "Use Windows Structured Exceptions" OFF)
option(USE_POSIX_SIGACTION "Use POSIX sigaction" OFF)

add_executable(except_test tests/except_test.cpp)
target_link_libraries(except_test except_lib)
add_test(except_test except_test)
