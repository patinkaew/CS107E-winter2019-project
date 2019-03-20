#ifndef DATASET_H
#define DATASET_H

#include <stdint.h>

typedef struct{
    int label;
    uintptr_t *list;
}data_set_t;

void dataset_init();

uintptr_t* dataset_get_all_datasets();

uintptr_t* dataset_get_dataset(int label);

uintptr_t* dataset_get_list(int label);

int dataset_get_dataset_size();

int dataset_get_list_size(int label);

void dataset_add_list(int label, uintptr_t* list);

void dataset_add_data(int label, uintptr_t data_addr);

#endif //close DATASET_H
