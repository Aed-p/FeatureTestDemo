set (CxxTestDemo_common_DIR "${CxxTestDemo_SOURCE_DIR}/common")

file(GLOB common_sources ${CxxTestDemo_common_DIR}/*.cpp ${CxxTestDemo_common_DIR}/*.h)

add_library(CxxTestDemo_common ${common_sources})

target_link_libraries(CxxTestDemo_common pthread unwind)
