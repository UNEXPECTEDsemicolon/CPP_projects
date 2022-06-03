cmake_minimum_required(VERSION 3.21)
project(Variant)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(Variant
        variant_test.cpp
        variant.h
        )
