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
    u8 *result = mph_get_new_cell(size);
    if (result == NULL) {
        printf("mh_malloc: get_free_cell returns NULL\n");
    }
    return result;
}

void mh_free(u8* cell, size_t size)
{
    MEM_BLOCK* block = mph_get_block_by_cell(cell, size);
    MEM_POOL* pool = block->pool;
    if(block!=NULL) {
        check_(pool);



        /*
         Проверяем имеет ли текущий индекс записи индекс освобождаемой ячейки, если да - уменьшаем значение индекса записи на 1;
         Если нет, добавляем индекс свободной ячейки в released_cell_indexes
         Если released_cells_count ==cells_count-1 (освобожденных ячеек столько же, сколько всего) удаляем MEM_BLOCK
         */


        if (block->cells_count -1 == block->released_cells_count) {
            mem_block_handler_delete_block(block);
            return;
        }



        u16 cell_index = (cell - (u8 *) block->cells) / size;

        if((cell - (u8 *) block->cells) % size != 0) {
            printf("ERROR: mh_free (----)\n");
        }

        if(cell_index> block->cells_count - 1) {
            printf("ERROR: mh_free (released index > cells_count) %p, %i\n", cell,*cell);
            return;
        }

        //если освобождаемая ячейка последняя записанная - уменьшаем индекс записи
        if (block->cells_count-1 == cell_index) {
            block->cells_count--;
            if (block->cells_count == 0) {
                mem_block_handler_delete_block(block);
            }
        } else {
            *(block->released_cell_indexes + block->released_cells_count) = cell_index;
            block->released_cells_count++;
            block->pool->released_cells_count++;
        }
        check_(pool);
    }
    else
        printf("ERROR: mh_free (block not found)\n");
}

