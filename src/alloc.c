#include "alloc.h"
#include <unistd.h>
#include <string.h> // <-- Needed for memcpy, memset
#include <stdio.h>
#include <stdlib.h> // <-- Needed for abort()

static free_block *HEAD = NULL;
static free_block *next_fit_ptr = NULL;

static void remove_free_block(free_block *block) {
    if (HEAD == block) {
        HEAD = block->next;
        return;
    }
    free_block *curr = HEAD;
    while (curr && curr->next != block) {
        curr = curr->next;
    }
    if (curr) {
        curr->next = block->next;
    }
}

static void *split(free_block *block, size_t size) {
    if (block->size < size + sizeof(header) + ALIGNMENT) return NULL;

    char *split_point = (char *)block + sizeof(header) + size;
    free_block *new_block = (free_block *)split_point;
    new_block->size = block->size - size - sizeof(header);
    new_block->next = block->next;

    block->size = size;
    block->next = NULL;

    return new_block;
}

static void coalesce(free_block *block) {
    free_block *curr = HEAD;
    while (curr) {
        char *end = (char *)curr + sizeof(header) + curr->size;
        if (end == (char *)block) {
            curr->size += block->size + sizeof(header);
            curr->next = block->next;
            return;
        }
        curr = curr->next;
    }
}

void *tumalloc(size_t size) {
    size = (size + ALIGNMENT - 1) & ~(ALIGNMENT - 1);

    free_block *prev = NULL;
    free_block *curr = next_fit_ptr ? next_fit_ptr : HEAD;

    while (curr) {
        if (curr->size >= size) {
            if (curr->size > size + sizeof(header)) {
                free_block *new_block = split(curr, size);
                if (new_block) {
                    new_block->next = HEAD;
                    HEAD = new_block;
                }
            }

            if (prev) prev->next = curr->next;
            else HEAD = curr->next;

            curr->magic = MAGIC_NUMBER;
            next_fit_ptr = curr->next;
            return (void *)(curr + 1);
        }

        prev = curr;
        curr = curr->next;
    }

    header *new_block = sbrk(size + sizeof(header));
    if ((void *)new_block == (void *)-1) return NULL;

    new_block->size = size;
    new_block->magic = MAGIC_NUMBER;
    new_block->next = NULL;

    return (void *)(new_block + 1);
}

void *tucalloc(size_t num, size_t size) {
    size_t total = num * size;
    void *ptr = tumalloc(total);
    if (ptr) memset(ptr, 0, total);
    return ptr;
}

void *turealloc(void *ptr, size_t new_size) {
    if (!ptr) return tumalloc(new_size);

    header *old_hdr = (header *)ptr - 1;
    if (old_hdr->magic != MAGIC_NUMBER) {
        printf("MEMORY CORRUPTION DETECTED\n");
        abort();
    }

    if (old_hdr->size >= new_size) return ptr;

    void *new_ptr = tumalloc(new_size);
    if (new_ptr) {
        memcpy(new_ptr, ptr, old_hdr->size);
        tufree(ptr);
    }
    return new_ptr;
}

void tufree(void *ptr) {
    if (!ptr) return;

    header *hdr = (header *)ptr - 1;
    if (hdr->magic != MAGIC_NUMBER) {
        printf("MEMORY CORRUPTION DETECTED\n");
        abort();
    }

    hdr->magic = 0;
    hdr->next = HEAD;
    HEAD = hdr;
    coalesce(hdr);
}
