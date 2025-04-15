cmake_minimum_required(VERSION 3.8)
project(CropIQ)
set(CMAKE_CXX_STANDARD 17)

set(VENDOR_DIR ${PROJECT_SOURCE_DIR}/vendor)

include(cmake/LocalConfig.cmake OPTIONAL)
#include_directories(${VENDOR_DIR}/crow)

find_package(asio CONFIG REQUIRED)
find_package(crow CONFIG REQUIRED)
find_path(JWT_CPP_INCLUDE_DIRS "jwt-cpp/base.h")

file(GLOB_RECURSE SRC_FILES src/*.cpp)

add_executable(CropIQ main.cpp ${SRC_FILES})

target_include_directories(CropIQ 
  PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
    ${VENDOR_DIR}
    ${JWT_CPP_INCLUDE_DIRS}
)

target_link_libraries(CropIQ PRIVATE asio::asio Crow::Crow)

if(WIN32)
  target_link_libraries(CropIQ PRIVATE ws2_32)
else()
  target_link_libraries(CropIQ PRIVATE stdc++fs pthread)
endif()