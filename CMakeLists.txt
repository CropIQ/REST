cmake_minimum_required(VERSION 3.8)

project(CropIQ)

set(CMAKE_CXX_STANDARD 17)

include_directories(/home/cropiq/Crow/include)
include_directories(/home/cropiq/Crow/scripts)

find_package(Crow)

add_executable(CropIQ ../main.cpp)

target_link_libraries(CropIQ stdc++fs pthread)