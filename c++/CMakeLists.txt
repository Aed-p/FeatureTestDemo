cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)

project(CxxTestDemo LANGUAGES C CXX)

file(GLOB common_sources ${CMAKE_SOURCE_DIR}/common/*.cpp ${CMAKE_SOURCE_DIR}/common/*.h)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/contrib/libunwind/include)
include_directories(${CMAKE_BINARY_DIR}/contrib/gflags/include)

# add submodule
add_subdirectory(${CMAKE_SOURCE_DIR}/contrib/libunwind-cmake)
add_subdirectory(${CMAKE_SOURCE_DIR}/contrib/gflags)
add_subdirectory(${CMAKE_SOURCE_DIR}/common)

add_executable(thread_pool_test ${CMAKE_SOURCE_DIR}/ParallelTask.cpp)
add_executable(template_demo_test ${CMAKE_SOURCE_DIR}/Template.cpp)
add_executable(atomic_load_test ${CMAKE_SOURCE_DIR}/atomic_load.cpp)
add_executable(any_test ${CMAKE_SOURCE_DIR}/any_test.cpp)

target_link_libraries(thread_pool_test  CxxTestDemo_common)
target_link_libraries(template_demo_test CxxTestDemo_common)
target_link_libraries(atomic_load_test CxxTestDemo_common gflags)
target_link_libraries(any_test CxxTestDemo_common gflags)