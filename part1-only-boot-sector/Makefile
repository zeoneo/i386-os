all:
	nasm -fbin boot-sector.s -o boot-sector.bin
run: all
	qemu-system-i386 boot-sector.bin

clean:
	rm -rf *.o *.bin