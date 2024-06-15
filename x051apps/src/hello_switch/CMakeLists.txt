set(SRC_FILES
    ../common/delay.c
    ../common/i2c.c
    ../common/lcd1602_i2c.c
    hello_switch.c
    )

set(EXEC_NAME hello_switch)

add_executable(${EXEC_NAME} ${SRC_FILES})
target_compile_options(${EXEC_NAME} PRIVATE "--code-size 2048")
target_include_directories(${EXEC_NAME} PUBLIC ../common)
#target_link_libraries(x051prg x051rpictrl)

install(TARGETS ${EXEC_NAME}
        DESTINATION bin)
