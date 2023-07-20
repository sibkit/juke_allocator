//
// Created by sibkit on 22.07.2020.
//

#pragma once

#include "mem_handler.h"
#include "mem_block_handler.h"



typedef struct MEM_BLOCK MEM_BLOCK;

typedef struct MEM_POOL
{
    size_t cell_size;            //Размер ячейки в байтах
    MEM_BLOCK* first_block;      //двусвязный список
    MEM_BLOCK* last_block;       //Обеспечение быстрого доступа
    size_t released_cells_count; //Количество ячеек помеченных освобожденными суммарно во всех блоках пула (должно быть 0 прежде чем создавать новый блок)
    u32 blocks_count;            //Количество блоков
    size_t* blocks_addresses;     //массив адресов блоков (должно быть отсортированно
} MEM_POOL;


void* mph_get_free_cell(size_t size);

MEM_BLOCK* mph_get_block_by_cell(void* cell, size_t size);

MEM_POOL* mem_pool_handler_get_pools();
// void* mph_get_free_cell(MEM_POOL* pool);
MEM_POOL* mem_pool_handler_get_pool(size_t cell_size);