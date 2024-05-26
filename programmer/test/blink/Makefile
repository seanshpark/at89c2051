CC=sdcc
# code size 2K for 2051
OPTIONS=-V -mmcs51 --out-fmt-ihx --code-size 2048 --model-small

all:
	mkdir -p out
	$(CC) $(OPTIONS) -o out/blink.hex blink.c

clean:
	rm -rf out/*
