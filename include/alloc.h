#ifndef CYB3053_PROJECT2_ALLOC_H
#define CYB3053_PROJECT2_ALLOC_H

#include <stddef.h>

#define ALIGNMENT 16
#define MAGIC_NUMBER 0x01234567

typedef struct header {
    size_t size;
    int magic;
    struct header *next;
} header;

typedef header free_block;

void *tumalloc(size_t size);
void *tucalloc(size_t num, size_t size);
void *turealloc(void *ptr, size_t new_size);
void tufree(void *ptr);

#endif
