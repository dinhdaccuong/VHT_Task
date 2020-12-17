cmake_minimum_required(VERSION 2.8)
project(tcp_client)
set(OpenCV_DIR /home/cuongd/OpenCV)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(PROJECT_HEADER_DIR  "./header")
find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)
include_directories(${PROJECT_HEADER_DIR})
add_executable(tcp_client
./source/communication.cpp
./source/client_utils.cpp 
./source/tcp_client.cpp)
target_link_libraries(tcp_client ${OpenCV_LIBS} Threads::Threads)
