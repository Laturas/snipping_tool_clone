/**
* Arena library
*/
#ifndef DEFINITIONS
#include "types.c"
#endif

#include <stdlib.h>

typedef struct Arena {
    void* bytes;
    i64 total_reserved_bytes;
    i64 first_unallocated_byte;
} Arena;

#define DEFAULT_ARENA_RESERVATION 1000000

/**
* Pushes the number of bytes as specified
* Returns NULL if the arena is full, if an empty arena failed to allocate, and on a negative number of bytes
*/
void* arena_push(Arena* arena, i64 number_of_bytes) {
    if (number_of_bytes < 0) return NULL;

    if (arena->bytes == NULL) {
        arena->bytes = malloc(DEFAULT_ARENA_RESERVATION * sizeof(u8));
        if (arena->bytes == NULL) {
            return NULL;
        }
        arena->total_reserved_bytes = DEFAULT_ARENA_RESERVATION;
    }

    if (arena->first_unallocated_byte + number_of_bytes < arena->total_reserved_bytes) {
        void* bytes = (void*)((u8*)arena->bytes + arena->first_unallocated_byte);
        arena->first_unallocated_byte += number_of_bytes;
        return bytes;
    } else {
        return NULL;
    }
}

void arena_destroy(Arena* arena) {
    free(arena->bytes);
    arena->bytes = NULL;
    arena->total_reserved_bytes = 0;
    arena->first_unallocated_byte = 0;
}