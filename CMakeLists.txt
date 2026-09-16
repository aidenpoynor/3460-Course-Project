cmake_minimum_required(VERSION 3.12)
project(SystemMonitor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(SystemMonitor
    src/main.cpp
    src/MemoryCollector.cpp
    src/ProcessCollector.cpp
    src/Reporter.cpp
)

target_include_directories(SystemMonitor PRIVATE include)
