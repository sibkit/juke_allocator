#pragma once

#include <stdio.h>
#include "mem_pool_handler.h"

typedef struct TEST_STRUCT
{
    u64 val1;
    u8 val2;
} TEST_STRUCT;

bool is_correct_cell_size(size_t cell_size) {
    size_t available_sizes[] = {1,2,4,8,16,32,48,64,
                                80,96,112,128,144,160,176,192,
                                208,224,240,256};
    for(int j=0; j<20;j++)
    {
        if (available_sizes[j]==cell_size)
            return true;
    }
    return false;
}

void test_mph_get_pool() {
    printf("TEST MPH get_pool()......");

    bool test_ok = true;
    for(int i=0;i<256;i++){
        MEM_POOL* pool = mem_pool_handler_get_pool(i);
        bool correct_size = is_correct_cell_size(i);
        if(correct_size) {
            if(pool==NULL || pool->cell_size!=i) {
                test_ok = false;
                printf("(%i))", i);
            }
        }
        else
        {
            if(pool!=NULL)
            {
                test_ok = false;
                printf("(%i))", i);
            }
        }
    }
    if(test_ok)
        printf("OK\n");
    else
        printf("FAILED\n");
}

void test() {
    test_mph_get_pool();

    printf("test struct size: %i\n", sizeof(TEST_STRUCT));
    for(int i=0;i<256;i++){
        MEM_POOL* pool = mem_pool_handler_get_pool(i);
        if(pool !=NULL) {
            printf("cell_size: %i, pool->cell_size: %i\n", i, pool->cell_size);
        } else {
            printf("cell_size: %i, pool is NULL\n", i);
        }
        printf("\n");
    }


}

