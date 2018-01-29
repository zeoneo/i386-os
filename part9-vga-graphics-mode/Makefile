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
myos.iso: boot/stage1/boot1.bin boot/stage2/stage2.bin krnl32.bin
	# cat $^ > os-image.bin
	mkdir -p iso
	dd if=/dev/zero of=iso/floppy.img bs=1024 count=1440
	diskutil eraseVolume "MS-DOS FAT12" MYFLOPPY `hdiutil attach -nomount iso/floppy.img`
	diskutil eject MYFLOPPY
	dd if=boot/stage1/boot1.bin of=iso/floppy.img seek=0 conv=notrunc
	hdiutil attach iso/floppy.img
	cp boot/stage2/stage2.bin /Volumes/MYFLOPPY
	cp krnl32.bin /Volumes/MYFLOPPY
	diskutil eject MYFLOPPY
	mkisofs -quiet -V 'MYOS' -input-charset iso8859-1 -o ../myos.iso -b floppy.img -hide floppy.img iso/
# '--oformat binary' deletes all symbols as a collateral, so we don't need
# to 'strip' them manually on this case
krnl32.bin: boot/kernel_entry.o ${OBJ}
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
