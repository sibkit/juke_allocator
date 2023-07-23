//
// Created by sibkit on 23.07.2020.
//

#include "mem_pool_handler.h"
#include <stdio.h>

MEM_POOL* mem_pools = NULL;

int cmp_blocks(const void* a, const void* b)
{
    MEM_BLOCK* b_a = *(MEM_BLOCK**)a;
    MEM_BLOCK* b_b = *(MEM_BLOCK**)b;
    if (b_a->cells < b_b->cells) return -1;
    if (b_b->cells < b_a->cells) return 1;
    return 0;
}

void check_(MEM_POOL* pool) {
/*
    for (int i = 0; i < pool->blocks_count; i++) {
        MEM_BLOCK *block = *(pool->blocks_addresses + i);
        if (block->pool != pool) {
            printf("ERROR: POOL BLOCKS ADDRESSES IS INCORRECT\n");
        }
        if (block->released_cells_count > block->cells_count) {
            printf("ERROR: BLOCKS INDEXES IS INCORRECT\n");
        }
        if (block->released_cells_count >= block->cells_count && block->cells_count > 0) {
            printf("ERROR: CELLS COUNT IS INCORRECT\n");
        }
        for (int j = 0; j < pool->blocks_count; j++) {
            if (i != j) {
                MEM_BLOCK* j_block = *(pool->blocks_addresses + j);
                if(j_block->cells==block->cells) {
                    printf("ERROR: CELLS ADDRESSES IS DOUBLED\n");
                }
            }
        }
    }

    if(pool->blocks_count>0) {
        u8* cell_addr = (*(pool->blocks_addresses))->cells;

        for (int i = 0; i < pool->blocks_count; i++) {
            MEM_BLOCK *block = *(pool->blocks_addresses + i);
            if(block->cells < cell_addr) {
                printf("ERROR: BLOCKS SORT CORRUPT\n");
            }
        }
    }
*/
}

MEM_BLOCK* mbh_create_block(MEM_POOL* pool) {

    check_(pool);

    MEM_BLOCK *block = malloc(sizeof(struct MEM_BLOCK) + MEM_BLOCK_CELLS_COUNT * sizeof(u16));

    block->pool = pool;
    block->cells_count = 0;
    block->released_cells_count = 0;
    block->released_cell_indexes = (u16*) (block + 1); //индексы освобожденных ячеек начинаются сразу после блока
    block->cells = malloc(MEM_BLOCK_CELLS_COUNT * pool->cell_size);

    pool->blocks_count++;

    pool->blocks_addresses = realloc(pool->blocks_addresses, sizeof(MEM_BLOCK**) * pool->blocks_count);
    *(pool->blocks_addresses + pool->blocks_count - 1) = block;
    qsort(pool->blocks_addresses, pool->blocks_count, sizeof(void *),cmp_blocks);

    check_(pool);

    return block;
}



void mem_block_handler_delete_block(MEM_BLOCK* block)
{
    MEM_POOL* pool = block->pool;

    check_(pool);

    if(pool->blocks_count==1) {
        free(pool->blocks_addresses);
        pool->blocks_addresses = NULL;
        pool->released_cells_count =0;
        pool->blocks_count=0;
        check_(pool);
    } else {
        check_(pool);
        pool->released_cells_count = pool->released_cells_count - block->released_cells_count;
        MEM_BLOCK** new_blocks_addresses = malloc(sizeof (MEM_BLOCK**) * pool->blocks_count-1);

        for (int i = 0; i < pool->blocks_count; i++) {

            if ( (*(pool->blocks_addresses + i))->cells < block->cells) {
                *(new_blocks_addresses + i) = *(pool->blocks_addresses + i);
            } else
                if ( (*(pool->blocks_addresses + i))->cells > block->cells) {
                *(new_blocks_addresses + (i - 1)) = *(pool->blocks_addresses + i);
            }
        }
        pool->blocks_count--;
        //MEM_BLOCK** temp = pool->blocks_addresses;
        free(pool->blocks_addresses);
        pool->blocks_addresses = new_blocks_addresses;
        check_(pool);

    }
    free(block->cells);
    free(block);
    check_(pool);
}




MEM_POOL* mem_pool_handler_get_pools() {
    return mem_pools;
}

 void fill_new_pool(MEM_POOL* pool, size_t size) {
     pool->cell_size=size;
     pool->released_cells_count = 0;
     pool->blocks_count=0;
}

MEM_BLOCK* get_block_by_index(int index, MEM_POOL* pool) {

    return (MEM_BLOCK *) *(pool->blocks_addresses + index);
}

MEM_POOL* mem_pool_handler_get_pool(size_t cell_size)
{
    /**В идеале должны задаваться при инициализации, надо будет переделать
     */
    if (mem_pools == NULL)
    {
        mem_pools = calloc(20, sizeof(MEM_POOL));

        fill_new_pool(mem_pools,1);
        fill_new_pool(mem_pools+1,2);
        fill_new_pool(mem_pools+2,4);
        fill_new_pool(mem_pools+3,8);
        fill_new_pool(mem_pools+4,16);
        fill_new_pool(mem_pools+5,32);
        fill_new_pool(mem_pools+6,48);
        fill_new_pool(mem_pools+7,64);
        fill_new_pool(mem_pools+8,80);
        fill_new_pool(mem_pools+9,96);
        fill_new_pool(mem_pools+10,112);
        fill_new_pool(mem_pools+11,128);
        fill_new_pool(mem_pools+12,144);
        fill_new_pool(mem_pools+13,160);
        fill_new_pool(mem_pools+14,176);
        fill_new_pool(mem_pools+15,192);
        fill_new_pool(mem_pools+16,208);
        fill_new_pool(mem_pools+17,224);
        fill_new_pool(mem_pools+18,240);
        fill_new_pool(mem_pools+19,256);
    }

    int index = 0;
    if (cell_size>=16) {
        if (cell_size%16!=0) {
            return NULL;
        } else {
            index = 3 + cell_size/16;
        }
    } else {
        switch (cell_size) {
            case 0:
                return NULL;
            case 1:
                index=0;
                break;
            case 2:
                index=1;
                break;
            case 3:
                return NULL;
            case 4:
                index=2;
                break;
            case 5:
            case 6:
            case 7:
                return NULL;
            case 8:
                index=3;
                break;
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
                return NULL;
            default:
                printf("ERROR: mem_pool_handler_get_pool(1)\n");
        }
    }

    return mem_pools+index;
}

MEM_BLOCK* find_block(void* cell, MEM_POOL* pool, size_t start, size_t end) {
    check_(pool);
    size_t middle = start + (end-start)/2;
    MEM_BLOCK* mid_block = get_block_by_index(middle, pool);
    if(cell >= mid_block -> cells && cell < mid_block -> cells + (MEM_BLOCK_CELLS_COUNT) * pool->cell_size) {
        check_(mid_block->pool);
        return mid_block;
    }

    else {
        if(cell<mid_block->cells)
            return find_block(cell,pool,start,middle-1);
        else
            return find_block(cell, pool, middle+1, end);
    }
    check_(pool);
}

MEM_BLOCK* mph_get_block_by_cell(void* cell, size_t size) {
    MEM_POOL* pool = mem_pool_handler_get_pool(size);
    check_(pool);



    return find_block(cell, pool, 0, pool->blocks_count-1);
}

void* mph_get_free_cell_in_pool(MEM_POOL* pool)
{
    check_(pool);
    if(pool->blocks_count == 0) {
        MEM_BLOCK* block = mbh_create_block(pool);
        block->cells_count++;
        return block->cells;
    }

    //Проверяем наличие освобожденных ячеек, если есть
    //ищем, начиная с последнего, блок с освобожденной ячейкой
    //возвращаем адрес последней освобожденной ячейки, уменьшаем индекс освободившихся ячеек.
    if(pool->released_cells_count > 0)
    {
        for(int i = 0;i <pool->blocks_count;i++)
        {
            MEM_BLOCK* block = (MEM_BLOCK *) *(pool->blocks_addresses + i);
            if(block->released_cells_count > 0)
            {
                u16 released_cell_index = *(block->released_cell_indexes + block->released_cells_count-1);
                block->released_cells_count--;
                pool->released_cells_count--;
                return block->cells + (pool->cell_size * released_cell_index);
            }
        }
        printf("ERROR: mph_get_free_cell_in_pool");
        check_(pool);
        return NULL;
    }
    else
    {
        for(int i = pool->blocks_count-1; i >=0; i--) {
            MEM_BLOCK *block = (MEM_BLOCK *) *(pool->blocks_addresses + i);
            if (block->cells_count < MEM_BLOCK_CELLS_COUNT) {
                block->cells_count++;
                check_(pool);
                return block->cells + pool->cell_size * (block->cells_count - 1);
            }
        }
        MEM_BLOCK* new_block = mbh_create_block(pool);
        new_block->cells_count++;
        check_(pool);
        return new_block->cells;
    }
}

void* mph_get_new_cell(size_t size) {
    MEM_POOL* pool = mem_pool_handler_get_pool(size);

    if(pool==NULL)
        return NULL;
    return mph_get_free_cell_in_pool(pool);
}

