cmake_minimum_required(VERSION 3.10)

project(YourProjectName)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_COMPILER /usr/bin/gcc)
set(CMAKE_CXX_COMPILER /usr/bin/g++)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

include_directories(src)

set(SOURCE_FILES
    src/host.cpp
    src/client.cpp
    src/conn_mq.cpp
    src/conn_fifo.cpp
    src/conn_sock.cpp
    src/logger.cpp
)

add_executable(Host src/host.cpp ${SOURCE_FILES})
add_executable(Client src/client.cpp ${SOURCE_FILES})
