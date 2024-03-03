CC = sdcc
CFLAGS = -mmcs51 --code-size 8192 --iram-size 256 --xram-size 0

HEX_FILE = main.hex
BIN_FILE = main.bin

.PHONY: all
all: $(HEX_FILE) $(BIN_FILE)

$(HEX_FILE): main.ihx
	packihx $< > $@

$(BIN_FILE): main.ihx
	makebin -p $< > $@

main.ihx: main.c
	$(CC) $(CFLAGS) $<

.PHONY: clean
clean:
	$(RM) $(HEX_FILE) $(BIN_FILE) main.asm main.ihx main.lk main.lst main.map main.mem main.rel main.rst main.sym

.PHONY: program
program: $(HEX_FILE)
	minipro -p AT89S52 -w main.hex
	