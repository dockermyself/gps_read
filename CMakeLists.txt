cmake_minimum_required(VERSION 3.0)
project(gps_g60)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Release)

include_directories(
    ${EIGEN3_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/include

)
add_executable(read_gps main.cpp gps_reader.cpp)
target_link_libraries(read_gps pthread atomic)
