#include <stdlib.h>

#include <stdint.h>
#include <string.h>
#include <stdio.h>

int8_t standard_lessthan_predicate(void *a, void *b)
{
    return (a < b) ? 1 : 0;
}

ordered_array_t ordered_array_create(uint32_t max_size, lessthan_predicate_t less_than)
{
    ordered_array_t to_ret;
    to_ret.array = (void *)malloc(max_size*sizeof(void *));
    memset(to_ret.array, 0, max_size * sizeof(void *));
    to_ret.size = 0;
    to_ret.max_size = max_size;
    to_ret.less_than = less_than;
    return to_ret;
}

ordered_array_t ordered_array_place(void *address, uint32_t max_size, lessthan_predicate_t less_than)
{
    ordered_array_t to_ret;
    to_ret.array = (void *)address;
    memset(to_ret.array, 0, max_size * sizeof(void *));
    to_ret.size = 0;
    to_ret.max_size = max_size;
    to_ret.less_than = less_than;
    return to_ret;
}

void ordered_array_destroy(ordered_array_t *array)
{
    free(array->array);
}

void ordered_array_insert(void *item, ordered_array_t *array)
{
    ASSERT(array->less_than);
    uint32_t itr = 0;
    while(itr < array->size && array->less_than(array->array[itr], item))
        itr++;
    if(itr == array->size)
        array->array[array->size++] = item;
    else
    {
        void *tmp = array->array[itr];
        array->array[itr] = item;
        while(itr < array->size)
        {
            itr++;
            void *tmp2 = array->array[itr];
            array->array[itr] = tmp;
            tmp = tmp2;
        }
        array->size++;
    }
}

void *ordered_array_lookup(uint32_t i, ordered_array_t *array)
{
    ASSERT(i < array->size);
    return array->array[i];
}

void ordered_array_remove(uint32_t i, ordered_array_t *array)
{
    while(i < array->size)
    {
        array->array[i] = array->array[i + 1];
        i++;
    }
    array->size--;
}