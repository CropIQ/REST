cmake_minimum_required(VERSION 4.0)
project(CropIQ)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(VENDOR_DIR ${PROJECT_SOURCE_DIR}/vendor)

include(cmake/LocalConfig.cmake OPTIONAL)

find_package(Crow CONFIG REQUIRED)

file(GLOB_RECURSE SRC_FILES src/*.cpp)

add_executable(CropIQ main.cpp ${SRC_FILES})

target_include_directories(CropIQ 
  PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
    ${VENDOR_DIR}
    ${JWT_CPP_INCLUDE_DIRS}
    ${VENDOR_DIR}/dotenv
)

target_link_libraries(CropIQ PRIVATE Crow::Crow)

find_package(jwt-cpp CONFIG REQUIRED)
target_link_libraries(CropIQ PRIVATE jwt-cpp::jwt-cpp)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(CropIQ PRIVATE nlohmann_json::nlohmann_json)

find_package(OpenSSL REQUIRED)
target_link_libraries(CropIQ PRIVATE OpenSSL::SSL)
target_link_libraries(CropIQ PRIVATE OpenSSL::Crypto)
find_package(unofficial-mariadb-connector-cpp CONFIG REQUIRED)
target_link_libraries(CropIQ PRIVATE unofficial::mariadb-connector-cpp::mariadbcpp)

if(WIN32)
  target_link_libraries(CropIQ PRIVATE ws2_32)
elseif(APPLE)
  target_link_libraries(CropIQ PRIVATE pthread)
else()
  target_link_libraries(CropIQ PRIVATE stdc++fs pthread)
endif()