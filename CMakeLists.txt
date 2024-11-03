cmake_minimum_required(VERSION 3.10)

project(RSA_prj)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -ggdb")

find_library(GMP_LIB gmp REQUIRED)
find_library(GMPXX_LIB gmpxx REQUIRED)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE ${GMP_LIB} ${GMPXX_LIB})
