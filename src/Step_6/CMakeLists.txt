cmake_minimum_required(VERSION 3.8)

project (test-memory-pool
        VERSION 1.0.0
        DESCRIPTION "Test Memory Pool")

include_directories(.)
link_directories(../ ./)

set(CMAKE_CXX_FLAGS "-Wall -g -std=c99")

# create executable
add_executable (test.out test-memory-pool.c memory_pool.c)
set_property(TARGET test.out PROPERTY C_STANDARD 99)
