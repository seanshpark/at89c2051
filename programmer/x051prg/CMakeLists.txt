set(LIB_FILES
    lib/i2c/src/i2c.cpp
    lib/mcp23017/src/mcp23017.cpp
)

add_library(x051rpictrl STATIC ${LIB_FILES})
target_include_directories(x051rpictrl PUBLIC lib/gpio/include)
target_include_directories(x051rpictrl PUBLIC lib/i2c/include)
target_include_directories(x051rpictrl PUBLIC lib/mcp23017/include)

set(SRC_FILES
    src/x051prg.cpp
    src/programmer.cpp
)

add_executable(x051prg ${SRC_FILES})
target_include_directories(x051prg PUBLIC include)
target_link_libraries(x051prg x051rpictrl)

install(TARGETS x051prg DESTINATION bin)
