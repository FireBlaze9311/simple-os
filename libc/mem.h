#ifndef MEM_H
#define MEM_H


#include "../cpu/types.h"

void *malloc(size_t size);
void free(void *);
void memory_copy(u8 *source, u8 *dest, int nbytes);
void memory_set(u8 *dest, u8 val, u32 len);

#endif