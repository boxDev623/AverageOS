#pragma once

#include <stdint.h>

#include "multiboot.h"

typedef struct {
    struct {
        uint32_t k_start_addr;
        uint32_t k_end_addr;
        uint32_t k_len;
        uint32_t text_start_addr;
        uint32_t text_end_addr;
        uint32_t text_len;
        uint32_t data_start_addr;
        uint32_t data_end_addr;
        uint32_t data_len;
        uint32_t rodata_start_addr;
        uint32_t rodata_end_addr;
        uint32_t rodata_len;
        uint32_t bss_start_addr;
        uint32_t bss_end_addr;
        uint32_t bss_len;
    } kernel;

    struct {
        uint32_t total_memory;
    } system;

    struct {
        uint32_t start_addr;
        uint32_t end_addr;
        uint32_t size;
    } available;
} kernel_memory_map_t;

extern kernel_memory_map_t kmap;

int32_t get_kernel_memory_map(kernel_memory_map_t *kmap, multiboot_info_t *mboot_info);
void display_kernel_memory_map(kernel_memory_map_t *kmap);