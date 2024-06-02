set(SRC_FILES
    src/binary.c
    src/common.c
    src/libcrc.c
    src/hex2bin.c
)

add_executable(hex2bin ${SRC_FILES})
target_include_directories(hex2bin PUBLIC src)
set_property(TARGET hex2bin PROPERTY C_STANDARD 99)

install(TARGETS hex2bin DESTINATION bin)
