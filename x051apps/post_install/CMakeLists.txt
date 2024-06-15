# post install step
# generate bin from hex with installed .ihx files

macro(install_hex2bin PRJ_NAME)
  install(CODE "execute_process(COMMAND ${PROJECT_HEX2BIN} ${PROJECT_BIN_DIR}/${PRJ_NAME})")
endmacro()

install_hex2bin("hello_timer.ihx")
install_hex2bin("hello_mcp23017.ihx")
install_hex2bin("hello_lcd1602.ihx")
install_hex2bin("hello_clock.ihx")
install_hex2bin("hello_lcd1602_i2c.ihx")
install_hex2bin("hello_switch.ihx")
