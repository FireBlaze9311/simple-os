C_SOURCES = $(wildcard kernel/*.c drivers/*c cpu/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h)

CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
GDB = /usr/local/i386elfgcc/bin/i386-elf-gdb

CFLAGS = -g -gdwarf-2

OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o}

all: os-image.bin 

os-image.bin: boot/boot_sect.bin kernel.bin
	cat $^ > $@

kernel.bin: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

kernel.elf: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ 

%.o : %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o : %.asm
	nasm $< -f elf -o $@

%.bin : %.asm
	nasm $< -f bin -o $@

clean:
	rm -rf *.bin *.dis *.o os-image.bin *.elf
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o cpu/*.o

run: os-image.bin
	qemu-system-i386 -fda $< 