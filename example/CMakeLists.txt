cmake_minimum_required(VERSION 3.16)
project(boost_test)

set(CMAKE_CXX_STANDARD 14)

#Boost 라이브러리 설치 확인
find_package(Boost)

if(Boost_FOUND)
    # include 패스 설정
    include_directories(${Boost_INCLUDE_DIRS})
endif()
add_executable(boost_test main.cpp)