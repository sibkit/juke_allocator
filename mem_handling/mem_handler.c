//
// Created by sibkit on 23.07.2020.
//
#include <stdio.h>
#include <stdbool.h>
#include "mem_pool_handler.h"

#define MAX_CELL_SIZE 255

void init() {
    setbuf(stdout, NULL);

}

u8* mh_malloc(size_t size) {
    u8 *result = mem_pool_handler_get_free_cell(size);
    if (result == NULL) {
        printf("mh_malloc: get_free_cell returns NULL\n");
    }
    return result;
}

bool check_cell(u8* cell, size_t size) {
    if(size > MAX_CELL_SIZE)
        return true;
    MEM_BLOCK* block = mem_pool_handler_get_block_by_cell(cell, size);
    return block->pool->cell_size == size;
}

void mh_free(u8* cell, size_t size)
{
    MEM_BLOCK* block = mem_pool_handler_get_block_by_cell(cell, size);

    if(block!=NULL)
    {
        /*
         Проверяем имеет ли текущий индекс записи индекс освобождаемой ячейки, если да - уменьшаем значение индекса записи на 1;
         Если нет, добавляем индекс свободной ячейки в released_cell_indexes
         Если cur_released_cell_index ==cur_cell_index-1 (освобожденных ячеек столько же, сколько всего) удаляем MEM_BLOCK
         */
        if(block->cur_cell_index == block->cur_released_cell_index+1)
        {
            mem_block_handler_delete_block(block);
            return;
        }
        else
        {
            u16 cell_index = (cell - (u8*)block->cells) / size;
            //если освобождаемая ячейка последняя записанная - уменьшаем индекс записи
            if(block->cur_cell_index-1 == cell_index) {
                block->cur_cell_index--;
                if(block->cur_cell_index==0) {
                    mem_block_handler_delete_block(block);
                }
            }
            else
            {
                u16* cur_free_cell_index = block->released_cell_indexes + block->cur_released_cell_index;
                *cur_free_cell_index = cell_index;
                block->cur_released_cell_index++;
                block->pool->released_cells_count++;
            }
        }
    }
    else
        printf("ERROR: mh_free (block not found)\n");
}

