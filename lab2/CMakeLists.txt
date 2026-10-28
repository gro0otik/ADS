cmake_minimum_required(VERSION 3.28)
project(lab2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lab2 main.cpp)

if(MSVC)
    target_compile_options(lab2 PRIVATE /W4 /WX)
else()
    target_compile_options(lab2 PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
