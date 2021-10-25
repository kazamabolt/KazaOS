.ONESHELL:all
export PATH:= toolchain/bin:$(PATH)
CC=i686-elf-gcc
AS=i686-elf-as
CFLAGS=-std=gnu99 -masm=att -ffreestanding -O2 -Wall -I./include/
OBJS=kernel.o lstdlib.o keyboard.o cursor.o strings.o

notarget: los

iso: los
	mkdir -p isodir/boot/grub
	cp grub.cfg isodir/boot/grub
	cp kazamaos.bin isodir/boot
	grub-mkrescue -o kazamaos.iso isodir

bootloader: bootloader/boot.o

los: $(OBJS) bootloader
	$(CC) -T linker.ld -o kazamaos.bin -ffreestanding -O2 -nostartfiles -nostdlib bootloader/boot.o $(OBJS) -lgcc

test:
	qemu-system-i386 -curses -kernel kazamaos.bin

test-iso:
	qemu-system-i386 -curses -cdrom kazamaos.iso

test-gui:
	qemu-system-i386 -kernel kazamaos.bin

test-iso-gui:
	qemu-system-i386 -cdrom kazamaos.iso

tools:
	$(MAKE) -C toolchain

clean:
	$(RM) -r isodir kazamaos.bin kazamaos.iso kernel.o bootloader/boot.o
