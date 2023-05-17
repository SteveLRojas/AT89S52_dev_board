CC = sdcc
CFLAGS = -mmcs51 --code-size 8192 --iram-size 256 --xram-size 0
TARGET = main.hex

.PHONY: all
all: $(TARGET)

$(TARGET): main.ihx
	packihx $< > $@

main.ihx: main.c
	$(CC) $(CFLAGS) $<

.PHONY: clean
clean:
	$(RM) $(TARGET) main.asm main.ihx main.lk main.lst main.map main.mem main.rel main.rst main.sym

.PHONY: program
program: $(TARGET)
	minipro -p AT89S52 -w main.hex
	