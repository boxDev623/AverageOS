#pragma once

// GDT + IDT + ISR
#include "intr/idt.h"
#include "intr/gdt.h"
#include "intr/isr.h"
// Terminal
#include "terminal.h"
// Memory
#include "mm/pmm.h"
#include "mm/memmap.h"
#include "mm/liballoc.h"
// BIOS 32
#include "sys/bios32.h"
// MULTIBOOT
#include "multiboot.h"
// LINEAR FRAMEBUFFER
#include "devices/lfb.h"

#include "devices/ps2/keyboard.h"
#include "devices/ps2/mouse.h"

// IO
#include "io.h"

void kernel_initialize(multiboot_info_t* mboot_info);