#include "common.h"

int bitmap_find_first_bit(unsigned char * bitmap, int size, int val)
{
	for (int i = 0; i < size * 8; i++) {
        int bit_val = (bitmap[i / 8] >> (i % 8)) & 1;
        if (bit_val == val) {
            return i;
        }
    }
    return BITMAP_OP_NOT_FOUND;
}

int bitmap_set_bit(unsigned char * bitmap, int size, int target_pos)
{
    if (target_pos < 0 || target_pos >= size * 8) {
        return BITMAP_OP_ERROR;
    }
    bitmap[target_pos / 8] |= (1 << (target_pos % 8));
    return BITMAP_OP_SUCCEED;
}

int bitmap_clear_bit(unsigned char * bitmap, int size, int target_pos)
{
     if (target_pos < 0 || target_pos >= size * 8) {
        return BITMAP_OP_ERROR;
    }
    bitmap[target_pos / 8] &= ~(1 << (target_pos % 8));
    return BITMAP_OP_SUCCEED;
}

int bitmap_bit_is_set(unsigned char * bitmap, int size, int pos)
{
    if (pos < 0 || pos >= size * 8) {
        return BITMAP_OP_ERROR;
    }
    return (bitmap[pos / 8] >> (pos % 8)) & 1;
}

int bitmap_print_bitmap(unsigned char * bitmap, int size)
{
    int pos = 0;
    int total_bits = size * BIT_PER_BYTE;
    unsigned char current_byte = 0;

    if (NULL == bitmap)
    {
        printf("ERROR: NULL bit map!\n");
        return BITMAP_OP_ERROR;
    }

    printf("bitmap %p size %d bytes: ", bitmap, size);

    while (pos < total_bits)
    {
        int v = 0;
        
        current_byte = *(bitmap + pos/BIT_PER_BYTE);
        v = ((current_byte >> pos % BIT_PER_BYTE) & 0x01);
        printf("%d", v);
        if ((pos + 1) % 4 == 0)
        {
            printf("  ");
        }
        
        pos++;
    }

    printf("\n");

    return BITMAP_OP_SUCCEED;
}
