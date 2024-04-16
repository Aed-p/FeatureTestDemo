cmake_minimum_required(VERSION 3.20)

project(CxxTestDemo LANGUAGES C CXX)

file(GLOB common_sources ${CMAKE_SOURCE_DIR}/common/*.cpp ${CMAKE_SOURCE_DIR}/common/*.h)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/contrib/libunwind/include)

# add submodule
add_subdirectory(${CMAKE_SOURCE_DIR}/contrib/libunwind-cmake)

add_executable(thread_pool_test ${CMAKE_SOURCE_DIR}/ParallelTask.cpp ${common_sources})

target_link_libraries(thread_pool_test pthread LIBUNWIND_LIBRARIES)