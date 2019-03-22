#include "dataset.h"
#include "strings.h"
#include "vector.h"
#include "malloc.h"
#include "printf.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static uintptr_t *data_sets;

void dataset_init(){
    data_sets = vector_init();
}

uintptr_t* dataset_get_all_datasets(){
    return data_sets;
}

uintptr_t* dataset_get_dataset(int label){
    int size = vector_size(data_sets);
    for(int i = 0 ; i < size ; i++){
        uintptr_t curr_data_set = vector_get(data_sets, i);
        if(label == ((data_set_t *)curr_data_set) -> label){
            return (uintptr_t*)curr_data_set;
        }
    }
    return NULL;
}

uintptr_t* dataset_get_list(int label){
    int size = vector_size(data_sets);
    for(int i = 0 ; i < size ; i++){
        data_set_t * curr_data_set = (data_set_t *)vector_get(data_sets, i);
        if(label == curr_data_set -> label){
            return curr_data_set -> list;
        }
    }
    return NULL;
}

int dataset_get_dataset_size(){
    return vector_size(data_sets);
}

int dataset_get_list_size(int label){
    int size = vector_size(data_sets);
    for(int i = 0 ; i < size ; i++){
        data_set_t * curr_data_set = (data_set_t *)vector_get(data_sets, i);
        if(label == curr_data_set -> label){
            return vector_size(curr_data_set -> list);
        }
    }
    return -1;
}

void dataset_add_list(int label, uintptr_t* list){
    data_set_t *new_data_set = malloc(sizeof(data_set_t));
    new_data_set -> label = label;
    new_data_set -> list = list;
    data_sets = vector_add(data_sets, (uintptr_t)new_data_set);
}

void dataset_add_data(int label, uintptr_t data_addr){
    int size = vector_size(data_sets);
    for(int i = 0; i < size; i++){
        data_set_t * curr_data_set = (data_set_t *)vector_get(data_sets, i);
        if(label == curr_data_set -> label){
            uintptr_t* curr_list = curr_data_set -> list;
            curr_list = vector_add(curr_list, data_addr);
            curr_data_set -> list = (uintptr_t)curr_list; //curr_list change if vector expand
            return;
        }
    }

    //no list with given label, create new
    uintptr_t* new_list = vector_init();
    new_list = vector_add(new_list, data_addr);
    dataset_add_list(label, new_list);
}

void dataset_remove_list(int label, uintptr_t* list);

void dataset_remove_data(int label, uintptr_t data_addr);
