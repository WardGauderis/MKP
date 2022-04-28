cmake_minimum_required(VERSION 3.21)
project(MKP)

#set(CMAKE_CXX_STANDARD 20) # Can be changed to a lower standard but the measurements were performed with C++20
#set(CMAKE_CXX_FLAGS "-O3 -march=native")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O3 -march=native -Wnull-dereference -Wall -Werror -Wextra -Wnon-virtual-dtor -Wold-style-cast -Wunused -Woverloaded-virtual -Wpedantic  -Wdouble-promotion -Wformat=2")

file(GLOB_RECURSE
        SRC
        "src/*.h"
        "src/*.cpp"
        )

add_executable(MKP ${SRC})
