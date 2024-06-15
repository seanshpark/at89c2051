set(SRC_FILES
    ../common/delay.c
    ../common/i2c.c
    ../common/mcp23017.c
    hello_mcp23017.c
    )

set(EXEC_NAME hello_mcp23017)

add_executable(${EXEC_NAME} ${SRC_FILES})
target_compile_options(${EXEC_NAME} PRIVATE "--code-size 2048")
target_include_directories(${EXEC_NAME} PUBLIC ../common)
#target_link_libraries(x051prg x051rpictrl)

install(TARGETS ${EXEC_NAME}
        DESTINATION bin)
