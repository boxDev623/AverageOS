#include <stdint.h>

#define PMM_BLOCK_SIZE 4096

typedef struct pmm_info
{
    uint32_t memory_size;
    uint32_t max_blocks;
    uint32_t *memory_map_array;
    uint32_t memory_map_end;
    uint32_t used_blocks;
}
pmm_info_t;

uint32_t pmm_get_max_blocks(void);
uint32_t pmm_get_used_blocks(void);

int pmm_next_free_frame(int32_t size);

void pmm_initialize(uint32_t bitmap, uint32_t total_memory_size);

void pmm_initialize_region(uint32_t base, uint32_t region_size);
void pmm_deinit_region(uint32_t base, uint32_t region_size);

void *pmm_alloc_block(void);
void pmm_free_block(void* p);

void *pmm_alloc_blocks(uint32_t size);
void pmm_free_blocks(void* p, uint32_t size);