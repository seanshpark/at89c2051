
cmake_minimum_required(VERSION 3.18)

set(CMAKE_TOOLCHAIN_FILE "sdcc-toolchain.cmake")
set(PROJECT_PARENT_FOLDER ${CMAKE_SOURCE_DIR}/..)
set(PROJECT_HEX2BIN ${PROJECT_PARENT_FOLDER}/build/install/bin/hex2bin)

project("x051apps")

add_subdirectory(src/hello_timer)
add_subdirectory(src/hello_mcp23017)
add_subdirectory(src/hello_lcd1602)
add_subdirectory(src/hello_clock)
add_subdirectory(src/hello_lcd1602_i2c)
add_subdirectory(src/hello_switch)

# TODO add more

# post install process to convert hex to bin
set(PROJECT_BIN_DIR ${CMAKE_SOURCE_DIR}/build/install/bin)
add_subdirectory(post_install)
