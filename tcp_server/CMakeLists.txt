cmake_minimum_required(VERSION 2.8)
project(tcp_server)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(PROJECT_HEADER_DIR  "./header")
find_package(Threads REQUIRED)
include_directories(${PROJECT_HEADER_DIR})
add_executable(tcp_server    
./source/server_utils.cpp                        
./source/tcp_server.cpp)
target_link_libraries(tcp_server Threads::Threads)
