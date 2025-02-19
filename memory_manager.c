#include "memory_manager.h"

static STRU_MEM_LIST * mem_pool = NULL;

void mem_mngr_print_snapshot(void)
{
    STRU_MEM_LIST * mem_list = NULL;

    printf("============== Memory snapshot ===============\n");

    mem_list = mem_pool;
    while(NULL != mem_list)
    {
        STRU_MEM_BATCH * mem_batch = mem_list->first_batch;

        printf("mem_list %p slot_size %d batch_count %d free_slot_bitmap %p\n", 
                   mem_list, mem_list->slot_size, mem_list->batch_count, mem_list->free_slots_bitmap);
        bitmap_print_bitmap(mem_list->free_slots_bitmap, mem_list->bitmap_size);

        while (NULL != mem_batch)
        {
            printf("\t mem_batch %p batch_mem %p\n", mem_batch, mem_batch->batch_mem);
            mem_batch = mem_batch->next_batch; // get next mem batch
        }

        mem_list = mem_list->next_list;
    }

    printf("==============================================\n");
}

void mem_mngr_init(void) {
    mem_pool = (STRU_MEM_LIST *)malloc(sizeof(STRU_MEM_LIST));
    if (!mem_pool) {
        printf("Memory manager initialization failed.\n");
        exit(1);
    }

    mem_pool->slot_size = MEM_ALIGNMENT_BOUNDARY;
    mem_pool->batch_count = 1;
    mem_pool->bitmap_size = (MEM_BATCH_SLOT_COUNT + 7) / 8;
    mem_pool->free_slots_bitmap = (unsigned char *)malloc(mem_pool->bitmap_size);
    memset(mem_pool->free_slots_bitmap, 0xFF, mem_pool->bitmap_size);

    mem_pool->first_batch = (STRU_MEM_BATCH *)malloc(sizeof(STRU_MEM_BATCH));
    mem_pool->first_batch->batch_mem = malloc(MEM_BATCH_SLOT_COUNT * mem_pool->slot_size);
    mem_pool->first_batch->next_batch = NULL;
    mem_pool->next_list = NULL;

    printf("Memory manager initialized.\n");
}

void mem_mngr_leave(void) {
    STRU_MEM_LIST *current_list = mem_pool;
    while (current_list) {
        STRU_MEM_BATCH *current_batch = current_list->first_batch;
        while (current_batch) {
            STRU_MEM_BATCH *next_batch = current_batch->next_batch;
            free(current_batch->batch_mem);
            free(current_batch);
            current_batch = next_batch;
        }
        STRU_MEM_LIST *next_list = current_list->next_list;
        free(current_list->free_slots_bitmap);
        free(current_list);
        current_list = next_list;
    }
    mem_pool = NULL;
    printf("Memory manager cleaned.\n");
}

void * mem_mngr_alloc(size_t size) {
    if (size <= 0) return NULL;
    size_t aligned_size = (size + MEM_ALIGNMENT_BOUNDARY - 1) & ~(MEM_ALIGNMENT_BOUNDARY - 1);
    STRU_MEM_LIST *current_list = mem_pool, *prev_list = NULL;
    
    while (current_list != NULL && current_list->slot_size < (int)aligned_size) {
        prev_list = current_list;
        current_list = current_list->next_list;
    }

    if (current_list == NULL) {
        current_list = (STRU_MEM_LIST *)malloc(sizeof(STRU_MEM_LIST));
        if (!current_list) return NULL;

        current_list->slot_size = aligned_size;
        current_list->batch_count = 1;
        current_list->bitmap_size = (MEM_BATCH_SLOT_COUNT + 7) / 8;
        current_list->free_slots_bitmap = (unsigned char *)malloc(current_list->bitmap_size);
        memset(current_list->free_slots_bitmap, 0xFF, current_list->bitmap_size);

        current_list->first_batch = (STRU_MEM_BATCH *)malloc(sizeof(STRU_MEM_BATCH));
        if (!current_list->first_batch) {
            free(current_list->free_slots_bitmap);
            free(current_list);
            return NULL;
        }

        current_list->first_batch->batch_mem = malloc(MEM_BATCH_SLOT_COUNT * current_list->slot_size);
        if (!current_list->first_batch->batch_mem) {
            free(current_list->first_batch);
            free(current_list->free_slots_bitmap);
            free(current_list);
            return NULL;
        }
        current_list->first_batch->next_batch = NULL;
        current_list->next_list = NULL;

        if (prev_list) {
            prev_list->next_list = current_list;
        } else {
            mem_pool = current_list;
        }
    }

    int slot_index = bitmap_find_first_bit(current_list->free_slots_bitmap, current_list->bitmap_size, 1);
    if (slot_index != BITMAP_OP_NOT_FOUND) {
        bitmap_clear_bit(current_list->free_slots_bitmap, current_list->bitmap_size, slot_index);
        return ((char*)current_list->first_batch->batch_mem + (slot_index * current_list->slot_size));
    }


    return NULL; 
}

void mem_mngr_free(void *ptr) {
    if (!ptr) {
        printf("Attempt to free a NULL pointer.\n");
        return;
    }

    STRU_MEM_LIST *current_list = mem_pool;
    while (current_list) {
        STRU_MEM_BATCH *current_batch = current_list->first_batch;
        while (current_batch) {
            if ((char*)ptr >= (char*)current_batch->batch_mem && 
                (char*)ptr < (char*)current_batch->batch_mem + (MEM_BATCH_SLOT_COUNT * current_list->slot_size)) {
                size_t offset = (char*)ptr - (char*)current_batch->batch_mem;
                int slot_index = offset / current_list->slot_size;
                if (slot_index < MEM_BATCH_SLOT_COUNT) {
                    bitmap_set_bit(current_list->free_slots_bitmap, current_list->bitmap_size, slot_index);
                    printf("Memory at %p freed.\n", ptr);
                    return;
                }
            }
            current_batch = current_batch->next_batch;
        }
        current_list = current_list->next_list;
    }

    printf("Error: Attempted to free unmanaged an invalid memory (%p).\n", ptr);
}
