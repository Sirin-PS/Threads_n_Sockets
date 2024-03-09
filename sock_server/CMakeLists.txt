cmake_minimum_required(VERSION 3.3)

project(server1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

include_directories(
    ../include
)

find_library(PTHREAD_LIBRARY pthread)

add_executable(
  server1 server1.cpp)

target_link_libraries(
  server1 ${PTHREAD_LIBRARY})

