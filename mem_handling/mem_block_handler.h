//
// Created by sibkit on 22.07.2020.
//

#pragma once

#include "types.h"
#include "mem_pool_handler.h"

#define MEM_BLOCK_CELLS_COUNT 65536
//#define MEM_BLOCK_CELLS_COUNT 256

typedef struct MEM_POOL MEM_POOL;

typedef struct MEM_BLOCK
{
    MEM_POOL* pool;                 //пул в который входит блок
    struct MEM_BLOCK* next_block;   //следующий блок памяти
    struct MEM_BLOCK* prev_block;   //предыдущий блок памяти

    u16 cur_cell_index;             //текущий индекс записи
    u16 cur_released_cell_index;    //текущий индекс записи индекса свободного блока

    u16* released_cell_indexes;         //индексы свободных блоков (статический массив из MEM_BLOCK_CELLS_COUNT элементов)
    void* cells;                    //ячейки блока

} MEM_BLOCK;

//size_t mem_block_handler_calculate_block_size(size_t cell_size);

MEM_BLOCK* mem_block_handler_create_block(MEM_POOL* pool);
void mem_block_handler_delete_block(MEM_BLOCK* block);

