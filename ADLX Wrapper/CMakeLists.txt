cmake_minimum_required(VERSION 3.10...4.3)

project(ADLX_Wrapper LANGUAGES CXX)

set(SOURCES
    adlxWrapper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../SDK/ADLXHelper/Windows/Cpp/ADLXHelper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../SDK/platform/Windows/WinAPIs.cpp
)

add_library(ADLX_Wrapper SHARED ${SOURCES})

target_include_directories(ADLX_Wrapper PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../SDK/ADLXHelper/Windows/Cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../SDK/platform
)