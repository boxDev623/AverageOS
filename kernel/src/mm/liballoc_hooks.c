#include "liballoc.h"
#include "pmm.h"

int liballoc_lock(void)
{
    return 0;
}

int liballoc_unlock(void)
{
    return 0;
}

void *liballoc_alloc(size_t size)
{
    pmm_alloc_blocks(size);
}

int liballoc_free(void *ptr, size_t size)
{
    pmm_free_blocks(ptr, size);
    return 0;
}