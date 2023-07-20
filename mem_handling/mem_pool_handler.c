//
// Created by sibkit on 23.07.2020.
//

#include "mem_pool_handler.h"
#include <stdio.h>

MEM_POOL* mem_pools = NULL;

MEM_POOL* mem_pool_handler_get_pools() {
    return mem_pools;
}

 void fill_new_pool(MEM_POOL* pool, size_t size) {
    //MEM_POOL* new_pool = malloc(sizeof(MEM_POOL));
     pool->cell_size=size;
     pool->first_block = NULL;
     pool->last_block = NULL;
     pool->released_cells_count = 0;
     pool->blocks_count=0;
}

typedef struct SIZE_POOL_ADDR {
    size_t size;
    MEM_POOL* pool_addr;
} SIZE_POOL_ADDR;

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

MEM_BLOCK* find_block_with_released_cells(MEM_BLOCK* block)
{
    if(block==NULL)
        return NULL;

    if(block->cur_released_cell_index > 0)
        return block;

    return find_block_with_released_cells(block->prev_block);
}


MEM_BLOCK* find_block(void* cell, MEM_POOL* pool, size_t start, size_t end) {
    return NULL;
    size_t middle = end-start;
    void* mid_addr = pool->blocks_addresses + middle;
    //if(mid_addr
    //size_t middle = (size_t) (pool->blocks_addresses + end/2);
}

MEM_BLOCK* mph_get_block_by_cell(void* cell, size_t size) {
    MEM_POOL* pool = mem_pool_handler_get_pool(size);
    return find_block(cell, pool, 0, pool->blocks_count);
}



void* mph_get_free_cell_in_pool(MEM_POOL* pool)
{

    //Проверяем есть ли блоки в пуле (если нет, то создаем и возвращаем первую ячейку)
    if (pool->first_block == NULL)
    {
        pool->first_block = mem_block_handler_create_block(pool);
        pool->first_block->cur_cell_index++;
        //printf("fc1 0x%x\n",pool->first_block->cells);
        return pool->first_block->cells;
    }
    //Проверяем наличие освобожденных ячеек, если есть
    //ищем, начиная с последнего, блок с освобожденной ячейкой
    //возвращаем адрес последней освобожденной ячейки, уменьшаем индекс освободившихся ячеек.
    if(pool->released_cells_count > 0)
    {
        MEM_BLOCK* block = find_block_with_released_cells(pool->last_block);
        u8* byte_cells = (u8*)block->cells;
        u16 released_cell_index = *(block->released_cell_indexes + block->cur_released_cell_index);

        block->cur_released_cell_index--;
        pool->released_cells_count--;
        //printf("fc2 0x%x\n",byte_cells + pool->cell_size * released_cell_index);
        return byte_cells + (pool->cell_size * released_cell_index);
    }
    else
    {
        MEM_BLOCK* last_block = pool->last_block;
        //Проверяем последний блок на наличие свободных ячеек, если есть - пишем в него, если нет - создаем новый блок
        if(last_block->cur_cell_index<MEM_BLOCK_CELLS_COUNT-1)
        {
            u8* cells = (u8*)last_block->cells;
            cells+=(pool->cell_size * last_block->cur_cell_index);
            last_block->cur_cell_index++;
            return cells;
        }
        else
        {
            MEM_BLOCK* new_block = mem_block_handler_create_block(pool);
            new_block->cur_cell_index++;
            return new_block->cells;
        }
    }
}

void* mph_get_free_cell(size_t size) {
    MEM_POOL* pool = mem_pool_handler_get_pool(size);
    if(pool==NULL)
        return NULL;
    return mph_get_free_cell_in_pool(pool);
}

