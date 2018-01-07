#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>
#include <stdint.h>
#include <stddef.h>

#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

typedef int8_t (*lessthan_predicate_t)(void*, void*);
typedef struct ordered_array {
    void **array;
    uint32_t size, max_size;
    lessthan_predicate_t less_than;
} ordered_array_t;

typedef struct header {
    uint32_t magic, size; // size includes footer
    uint8_t is_hole;
} header_t;

typedef struct  footer {
    uint32_t magic;
    header_t *header;
} footer_t;

typedef struct heap {
    ordered_array_t index;
    uint32_t start_address, end_address, max_address;
    uint8_t supervisor, readonly;
} heap_t;

__attribute__((__noreturn__))
void abort();
void panic(const char *str);
void panic_assert(const char *file, int32_t line, const char *desc);
int atoi(char *);

void *malloc(size_t sz);
void *malloc_a(size_t sz);
void *malloc_p(size_t sz, size_t *phys);
void *malloc_ap(size_t sz, size_t *phys);

heap_t *create_heap(uint32_t start, uint32_t end, uint32_t max, uint8_t supervisor, uint8_t readonly);
void *alloc(uint32_t size, uint8_t page_align, heap_t *heap);
void free(void *p);
void free_internal(void *p, heap_t *heap);

int8_t standard_lessthan_predicate(void *a, void *b);

ordered_array_t ordered_array_create(uint32_t max_size, lessthan_predicate_t less_than);
ordered_array_t ordered_array_place(void *address, uint32_t max_size, lessthan_predicate_t less_than);

void ordered_array_destroy(ordered_array_t *array);
void ordered_array_insert(void *item, ordered_array_t *array);
void *ordered_array_lookup(uint32_t i, ordered_array_t *array);
void ordered_array_remove(uint32_t i, ordered_array_t *array);

#endif
