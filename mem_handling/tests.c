
#include <stdio.h>
#include "mem_pool_handler.h"


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

void test_fill_and_clear_pool_reverse() {
    printf("TEST reverse_clear_pool........");
    bool test_ok = true;
    MEM_POOL* pool = mem_pool_handler_get_pool(16);

    u32 bc = pool->blocks_count;

    int cells_count = 4 * MEM_BLOCK_CELLS_COUNT;
    u8** arr = malloc(16*cells_count);
    for(int i=0;i<cells_count;i++) {
        *(arr+i) = (u8*) mh_malloc(16);
    }

    for(int i=0;i<cells_count;i++) {
        mh_free(*(arr+i) , 16);
    }

    if(pool->blocks_count!=bc)
        test_ok = false;

    if (test_ok)
        printf("OK\n");
    else
        printf("FAILED\n");
}

void test_fill_and_clear_pool() {

    printf("TEST clear_pool................");
    bool test_ok = true;
    MEM_POOL* pool = mem_pool_handler_get_pool(16);

    u32 bc = pool->blocks_count;

    int cells_count = 4 * MEM_BLOCK_CELLS_COUNT;
    u8** arr = malloc(16*cells_count);
    for(int i=0;i<cells_count;i++) {
        *(arr+i) = (u8*) mh_malloc(16);
    }

    for(int i=cells_count-1;i>=0;i--) {
        mh_free(*(arr+i) , 16);
    }

    if(pool->blocks_count!=bc)
        test_ok = false;

    if (test_ok)
        printf("OK\n");
    else
        printf("FAILED\n");
}

void test_mph_get_pool() {
    printf("TEST MPH get_pool()............");
    bool test_ok = true;

    for (int i = 0; i < 256; i++) {
        MEM_POOL *pool = mem_pool_handler_get_pool(i);
        bool correct_size = is_correct_cell_size(i);
        if (correct_size) {
            if (pool == NULL || pool->cell_size != i) {
                test_ok = false;
                printf("(%i))", i);
            }
        } else {
            if (pool != NULL) {
                test_ok = false;
                printf("(%i))", i);
            }
        }
    }
    if (test_ok)
        printf("OK\n");
    else
        printf("FAILED\n");
}

void test_write_read() {
    printf("TEST write_read................");
    bool test_ok = true;
    MEM_POOL* pool = mem_pool_handler_get_pool(32);

    u32 bc = pool->blocks_count;



    int cells_count = 4 * MEM_BLOCK_CELLS_COUNT;
    u32** arr = malloc(32*cells_count);
    for(i32 i=0;i<cells_count;i++) {
        *(arr+i) = (u8*) mh_malloc(32);
        **(arr+i) = i;
    }

    for(i32 i=cells_count-1;i>=0;i--) {
        if((**(arr+i)) != i) {
            test_ok = false;
        }
        mh_free(*(arr+i) , 32);
    }



    if (test_ok)
        printf("OK\n");
    else
        printf("FAILED\n");
}

void test() {
    test_fill_and_clear_pool();
    test_fill_and_clear_pool_reverse();
    test_mph_get_pool();
    test_write_read();
/*
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
*/

}

