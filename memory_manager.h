#ifndef __MEM_MNGR_H__
#define  __MEM_MNGR_H__

#include "common.h"

#define MEM_ALIGNMENT_BOUNDARY  8

#define MEM_BATCH_SLOT_COUNT    8

#define SLOT_ALLINED_SIZE(size)  (((size + MEM_ALIGNMENT_BOUNDARY - 1)/MEM_ALIGNMENT_BOUNDARY) * MEM_ALIGNMENT_BOUNDARY)

typedef struct _stru_mem_batch
{
    void * batch_mem; 
    struct _stru_mem_batch * next_batch; 
} STRU_MEM_BATCH;

typedef struct _stru_mem_list
{
    int slot_size;    
    int batch_count; 
    unsigned char * free_slots_bitmap; 
    int bitmap_size; 
    struct _stru_mem_batch * first_batch; 
    struct _stru_mem_list * next_list; 
} STRU_MEM_LIST;

void mem_mngr_init(void);
void mem_mngr_leave(void);
void mem_mngr_print_snapshot(void);
void * mem_mngr_alloc(size_t size);
void mem_mngr_free(void * ptr);


#endif
