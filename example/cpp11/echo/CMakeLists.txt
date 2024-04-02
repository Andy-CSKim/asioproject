cmake_minimum_required(VERSION 3.16)
project(boost_test)

set(CMAKE_CXX_STANDARD 11)

#Boost 라이브러리 설치 확인
find_package(Boost)

if(Boost_FOUND)
    # include 패스 설정
    include_directories(${Boost_INCLUDE_DIRS})
endif()

#add_executable(blocking_tcp_server blocking_tcp_echo_server.cpp)
#add_executable(blocking_tcp_client blocking_tcp_echo_client.cpp)

add_executable(async_tcp_server async_tcp_echo_server.cpp)
