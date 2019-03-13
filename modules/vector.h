/*
 * vector<uintptr_t> module
 *
 * vector stores the address of the data, so we can use any datatype
 */
#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include <stdint.h>

uintptr_t* vector_init();

void vector_free(uintptr_t* v);

int vector_size(uintptr_t* v);

int vector_capacity(uintptr_t* v);

bool vector_is_empty(uintptr_t* v);

uintptr_t vector_get(uintptr_t* v, int index);

uintptr_t* vector_add(uintptr_t* v, uintptr_t val);

uintptr_t* vector_insert(uintptr_t* v, int index, uintptr_t val);

uintptr_t vector_remove(uintptr_t* v, int index);

uintptr_t* vector_clear(uintptr_t* v);

void vector_print(uintptr_t* v);

#endif VECTOR_H
