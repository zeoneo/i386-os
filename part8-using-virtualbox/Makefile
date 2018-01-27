C_SOURCES = $(wildcard kernel/*.c drivers/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h)
# Nice syntax for file extension replacement
OBJ = ${C_SOURCES:.c=.o}

# Change this if your cross-compiler is somewhere else
CC = i386-elf-gcc
GDB = i386-elf-gdb
# -g: Use debugging symbols in gcc
CFLAGS = -g

# First rule is run by default
myos.iso: boot/boot-sector.bin kernel.bin
	# cat $^ > os-image.bin
	mkdir -p iso
	cat boot/boot-sector.bin kernel.bin > disk.bin
	dd if=/dev/zero of=iso/floppy.img bs=1024 count=1440
	dd if=disk.bin of=iso/floppy.img seek=0 conv=notrunc
	mkisofs -quiet -V 'MYOS' -input-charset iso8859-1 -o ../myos.iso -b floppy.img -hide floppy.img iso/

# '--oformat binary' deletes all symbols as a collateral, so we don't need
# to 'strip' them manually on this case
kernel.bin: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

# Used for debugging purposes
kernel.elf: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ 

run: myos.iso
	qemu-system-i386 -cdrom ../myos.iso

vm: myos.iso
	(killall VirtualBox && sleep 1) || true
	virtualBox --startvm "myos" &

# Open the connection to qemu and load our kernel-object file with symbols
debug: os-image.bin kernel.elf
	qemu-system-i386 -s -fda os-image.bin &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

# Generic rules for wildcards
# To make an object, always compile from its .c
%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.s
	nasm $< -f elf -o $@

%.bin: %.s
	nasm $< -f bin -o $@

clean:
	rm -rf iso/
	rm -rf ../*.iso
	rm -f *.dmg *.img *.iso
	rm -rf *.bin *.dis *.o os-image.bin *.elf
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o
