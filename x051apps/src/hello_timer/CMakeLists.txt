set(SRC_FILES
    hello_timer.c
    )

set(EXEC_NAME hello_timer)

add_executable(${EXEC_NAME} ${SRC_FILES})
target_compile_options(${EXEC_NAME} PRIVATE "--code-size 2048")
target_include_directories(${EXEC_NAME} PUBLIC ../common)
#target_link_libraries(x051prg x051rpictrl)

install(TARGETS ${EXEC_NAME}
        DESTINATION bin)
