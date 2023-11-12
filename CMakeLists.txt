cmake_minimum_required(VERSION 3.26)
project(c_polynomials C)

set(CMAKE_C_STANDARD 11)

add_executable(c_polynomials main.c
        polynomial.c
        polynomial.h)
