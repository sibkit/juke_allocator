//
// Created by sibkit on 22.07.2020.
//


// create_count++;
// if(create_count%100==0)
//     printf("cc:%lu\n",create_count);

#include "mem_block_handler.h"

#include <stdio.h>

/*
size_t mem_block_handler_calculate_block_size(size_t cell_size)
{
    return sizeof(struct MEM_BLOCK) + MEM_BLOCK_CELLS_COUNT * sizeof(u16);
}*/



MEM_BLOCK* mem_block_handler_create_block(MEM_POOL* pool)
{
    MEM_BLOCK* block = malloc(sizeof(struct MEM_BLOCK) + MEM_BLOCK_CELLS_COUNT * sizeof(u16));

    block->pool = pool;
    block->next_block = NULL;
    block->prev_block = NULL;
    block->cur_cell_index = 0;
    block->cur_released_cell_index = 0;
    block->released_cell_indexes = (u8*)(block + 1);
    block->cells = malloc(MEM_BLOCK_CELLS_COUNT * pool->cell_size);// ((u8*) block->released_cell_indexes) + MEM_BLOCK_CELLS_COUNT * sizeof(u16);

    pool->last_block = block;
    pool->blocks_count++;

    pool->blocks_addresses = realloc(pool->blocks_addresses, sizeof(MEM_BLOCK *) * pool->blocks_count);// malloc(sizeof(MEM_BLOCK *)*mem_pools->blocks_count );
    *(pool->blocks_addresses + pool->blocks_count) = (size_t) block;

    block->prev_block = pool->last_block;
    pool->last_block->next_block = block;
    pool->last_block = block;
    return block;

}

void mem_block_handler_delete_block(MEM_BLOCK* block)
{
    if(block->prev_block != NULL) {
        block->prev_block->next_block = block->next_block;
    } else {
        block->pool->first_block = block->next_block;
    }
    if(block->next_block != NULL) {
        block->next_block->prev_block = block->prev_block;
    } else {
        block->pool->last_block = block->prev_block;
    }
    block->pool->blocks_count--;
    if(block->pool->blocks_count == 0) {
        block->pool->first_block = NULL;
        block->pool->last_block = NULL;
    }
    block->pool->released_cells_count = block->pool->released_cells_count-block->cur_released_cell_index;

    free(block->cells);
    free(block);
}

