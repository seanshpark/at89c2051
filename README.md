# Hobby project with AT89C2051/AT89C4051

This project is just to have fun with old 8 bit 8051 core with 89C2051/89C4051 !

Prerequisite
```
sudo apt-get install clang-format-15
```

## How to build

Build programmer
```
ln -s Makefile.me Makefile

make cfg
make debug
make install
```

Build x051 apps
```
make apps
```

## Programming x051

Erase x051 PEROM
```
make erase
```

Program `hello_lcd1602` app
```
make pglcd1602
```
