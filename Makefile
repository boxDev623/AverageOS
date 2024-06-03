.PHONY: all
all:
	make -C libs/libk all
	make -C libs/openlibm libopenlibm.a ARCH=i386
	make -C kernel all
	cp kernel/bin/kernel isodir/boot/kernel.bin
	grub-mkrescue -o averageos.iso isodir

.PHONY: clean
clean:
	make -C kernel clean
	make -C libs/libk clean
	make -C libs/openlibm clean
	rm isodir/boot/kernel.bin

.PHONY: run
run:
	qemu-system-i386 -display sdl -m 512M -cdrom averageos.iso