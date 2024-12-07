cmake_minimum_required(VERSION 3.10)

project(RSA_prj)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -ggdb -Wall")

set(TARGET main)

find_library(GMP_LIB gmp REQUIRED)
find_library(GMPXX_LIB gmpxx REQUIRED)

include_directories(
    inc/
)

add_executable(
    ${TARGET}
    main.cpp
    src/RSA.cpp
    src/utils.cpp
)

target_link_libraries(${TARGET} PRIVATE ${GMP_LIB} ${GMPXX_LIB})
