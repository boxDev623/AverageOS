.PHONY: all
all:
	make -C kernel all
	cp kernel/bin/kernel isodir/boot/kernel.bin
	grub-mkrescue -o averageos.iso isodir

.PHONY: clean
clean:
	make -C kernel clean
	rm isodir/boot/kernel.bin

.PHONY: run
run:
	qemu-system-i386 -cdrom averageos.iso