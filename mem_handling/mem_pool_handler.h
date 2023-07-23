//
// Created by sibkit on 22.07.2020.
//

#pragma once

#include "mem_handler.h"

#define MEM_BLOCK_CELLS_COUNT 65535


typedef struct MEM_POOL MEM_POOL;

typedef struct MEM_BLOCK
{
    MEM_POOL* pool;                 //пул в который входит блок
    u16 cells_count;             //текущий индекс записи
    u16 released_cells_count;       //количество ячеек помеченных освобожденными, прежде чем выдавать новую должно быть 0
    u16* released_cell_indexes;     //индексы свободных блоков (статический массив из MEM_BLOCK_CELLS_COUNT элементов)
    u8* cells;                    //ячейки блока

} MEM_BLOCK;

typedef struct MEM_POOL
{
    size_t cell_size;            //Размер ячейки в байтах
    size_t released_cells_count; //Количество ячеек помеченных освобожденными суммарно во всех блоках пула (должно быть 0 прежде чем создавать новый блок)
    u32 blocks_count;            //Количество блоков
    MEM_BLOCK** blocks_addresses;     //массив адресов блоков (должно быть отсортированно по адресу MEM_BLOCK.cells)
} MEM_POOL;

MEM_BLOCK* mbh_create_block(MEM_POOL* pool);
void mem_block_handler_delete_block(MEM_BLOCK* block);
void* mph_get_new_cell(size_t size);

MEM_BLOCK* mph_get_block_by_cell(void* cell, size_t size);

MEM_POOL* mem_pool_handler_get_pools();
// void* mph_get_new_cell(MEM_POOL* pool);
MEM_POOL* mem_pool_handler_get_pool(size_t cell_size);

void check_(MEM_POOL* pool);