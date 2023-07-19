//
// Created by sibkit on 23.07.2020.
//

#include "mem_pool_handler.h"
#include <stdio.h>

MEM_POOL* mem_pools = NULL;

MEM_POOL* mem_pool_handler_get_pools() {
    return mem_pools;
}

 MEM_POOL* fill_new_pool(size_t size) {
    MEM_POOL* new_pool = malloc(sizeof(MEM_POOL));
    new_pool->next_pool=NULL;
    new_pool->cell_size=size;
    new_pool->first_block = NULL;
    new_pool->last_block = NULL;
    new_pool->released_cells_count = 0;
    new_pool->blocks_count=0;
    return new_pool;
}

MEM_POOL* get_pool_recursive(MEM_POOL* pool, size_t cell_size)
{
    if(pool == NULL) {
        printf("allocator.c get_pool_recursive");
        return NULL;
    }

    if(pool->cell_size == cell_size)
        return pool;

    if(pool->next_pool == NULL) {
        MEM_POOL* new_pool = malloc(sizeof (MEM_POOL));
        fill_new_pool(cell_size);
        pool->next_pool = new_pool;
        return new_pool;
    }
    else
        return get_pool_recursive(pool->next_pool, cell_size);
}

MEM_POOL* mem_pool_handler_get_pool(size_t cell_size)
{
    if (mem_pools == NULL)
    {
        mem_pools = calloc(16, sizeof(MEM_POOL));
        for (int i = 1; i <= 16; i++)
            fill_new_pool(i);
    }

    //u32 tableRow = cell_size % 16;
    MEM_POOL *first_line = mem_pools + ((cell_size - 1) % 16);//линии с нулевым размером быть не может

    if (first_line->cell_size == cell_size)
        return first_line;
    else
    {
        MEM_POOL* result = get_pool_recursive(first_line, cell_size);
        return result;
    }
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
    void* mid_addr = pool->block_addresses+middle;
    //if(mid_addr
    //size_t middle = (size_t) (pool->block_addresses + end/2);
}

MEM_BLOCK* mem_pool_handler_get_block_by_cell(void* cell, size_t size) {
    MEM_POOL* pool = mem_pool_handler_get_pool(size);
    return find_block(cell, pool, 0, pool->blocks_count);
}

void* mem_pool_handler_get_free_cell_in_pool(MEM_POOL* pool)
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
        //u16 free_index = block->cur_released_cell_index;

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

void* mem_pool_handler_get_free_cell(size_t size) {
    MEM_POOL* pool = mem_pool_handler_get_pool(size);
    if(pool->cell_size!=size) {
        printf("ERROR: mem_pool_handler_get_free_cell(1)");
    }
    return mem_pool_handler_get_free_cell_in_pool(pool);
}

