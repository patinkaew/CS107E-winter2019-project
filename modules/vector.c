#include "vector.h"
#include "printf.h"
#include "malloc.h"
#include <stdbool.h>
#include <stdint.h>

/*
 * Author: Patin Inkaew
 * Date: Mar 2019 (CS 107E Winter 2019)
 */

static struct vector_info {
    int size;
    int capacity;
};

static int base_capacity = 10;

uintptr_t* vector_init(){ //constructor
    struct vector_info* v = malloc(sizeof(uintptr_t) * base_capacity + sizeof(struct vector_info)); //start with 10 element, used first block to store vector_info
    v -> size = 0;
    v -> capacity = base_capacity ;
    return (uintptr_t*)(v + 1);
}

void vector_free(uintptr_t* v){ //destructor
    struct vector_info* info_ptr = (struct vector_info *)v - 1;
    free(info_ptr);
}

int vector_size(uintptr_t* v){ //return current size
    struct vector_info* info_ptr = (struct vector_info *)v - 1;
    return info_ptr -> size;
}

int vector_capacity(uintptr_t* v){ //return current capacity (for debugging)
    struct vector_info* info_ptr = (struct vector_info *)v - 1;
    return info_ptr -> capacity;
}

bool vector_is_empty(uintptr_t* v){ //return whether vector is free or not
    struct vector_info* info_ptr = (struct vector_info *)v - 1;
    return info_ptr -> size == 0;
}

uintptr_t vector_get(uintptr_t* v, int index){ //return element at given index
    struct vector_info* info_ptr = (struct vector_info *)v - 1;
    int size = info_ptr -> size;
    if(index >= size || index < 0){ //out of range
        return 0;
    }
    return *(v + index);
}

uintptr_t* vector_add(uintptr_t* v, uintptr_t val){ //add data at the end
    struct vector_info* info_ptr = (struct vector_info *)v - 1;
    int size = info_ptr -> size;
    int capacity = info_ptr -> capacity;
    size++;
    info_ptr -> size = size; //store new size
    if(size > capacity){
        struct vector_info* new_ptr = realloc(info_ptr, sizeof(uintptr_t) *
                                        (capacity + base_capacity) +
                                        sizeof(struct vector_info));
        new_ptr -> capacity = capacity + base_capacity;
        uintptr_t* new_v = (uintptr_t*)(new_ptr + 1);
        *(new_v + size - 1) = val;
        return new_v;
    }else{
        *(v + size - 1) = val;
        return v;
    }
}

uintptr_t* vector_insert(uintptr_t* v, int index, uintptr_t val){ //insert val at given index
    struct vector_info* info_ptr = (struct vector_info *)v - 1;
    int size = info_ptr -> size;
    int capacity = info_ptr -> capacity;
    if(index > size || index < 0){ //do nothing
        return v;
    }else if(index == size){ //act like vector_add
        return vector_add(v, val);
    }else{
        uintptr_t* new_v = v;
        if(size + 1 > capacity){ //check if we need to expand
            new_v = vector_add(v, 0); //just add something at the end and then shift
        }

        for(int i = size - 1; i >= index; i--){ //shift data
            new_v[i + 1] = new_v[i];
        }
        size++;
        info_ptr -> size = size;
        *(new_v + index) = val; //insert new data

        return new_v;
    }
}

void vector_set(uintptr_t* v, int index, uintptr_t val){
    struct vector_info* info_ptr = (struct vector_info *)v - 1;
    int size = info_ptr -> size;
    if(index >= size || index < 0){ //out of range
        return;
    }
    *(v + index) = val;
}

uintptr_t vector_remove(uintptr_t* v, int index){ //remove element from given index, return removed value
    struct vector_info* info_ptr = (struct vector_info *)v - 1;
    int size = info_ptr -> size;
    uintptr_t remove = v[index];
    for(int i = index; i < size; i++){ //shift data
        v[i] = v[i+1];
    }
    size--;
    info_ptr -> size = size;
    return remove;
}

uintptr_t* vector_clear(uintptr_t* v){ //clear all data in vector
    struct vector_info* info_ptr = (struct vector_info *)v - 1;
    info_ptr -> size = 0; //pretend that nothing is here
    return v;
}

void vector_print(uintptr_t* v){ //print all elements of vector
    struct vector_info* info_ptr = (struct vector_info *)v - 1;
    int size = info_ptr -> size;
    printf("[");
    for(int i = 0; i < size; i++){
        printf("%c", v[i]);
        if(i != size -1){
            printf(", ");
        }
    }
    printf("]\n");
}
