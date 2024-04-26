.PHONY: all
all:
	make -C libs/libk all
	make -C kernel all
	cp kernel/bin/kernel isodir/boot/kernel.bin
	grub-mkrescue -o averageos.iso isodir

.PHONY: clean
clean:
	make -C kernel clean
	make -C libs/libk clean
	rm isodir/boot/kernel.bin

.PHONY: run
run:
	qemu-system-i386 -m 512M -cdrom averageos.iso