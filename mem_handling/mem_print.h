//
// Created by sibkit on 01.09.2020.
//
#include <stdio.h>
#include "mem_pool_handler.h"

#pragma once

void mem_block_handler_print_block(MEM_BLOCK* block)
{
    printf("        block (0x%x)\n",block);
    printf("            block r_ci   = %hu\n",block->released_cells_count);
    printf("            block ci   = %hu\n",block->cells_count);
}

void print_pool(MEM_POOL* pool)
{
    if(pool->blocks_count!=0)
    {
        printf("pool (cell_size = %lu)\n", pool->cell_size);
        printf("    blocks_count    = %lu\n", pool->blocks_count);
        printf("    r_cells_count   = %lu\n", pool->released_cells_count);

        MEM_BLOCK* cur_block = pool->first_block;
        while (cur_block!=NULL)
        {
            mem_block_handler_print_block(cur_block);
            cur_block = cur_block->next_block;
        }
    }
    if(pool->next_pool!=NULL)
        print_pool(pool->next_pool);
}

void mem_pool_handler_print_pools()
{
    MEM_POOL* mem_pools = mem_pool_handler_get_pools();
    if(mem_pools==NULL)
        printf("NULL\n");
    else
    {
        for(int i=0;i<16;i++)
        {
            print_pool(mem_pools+i);
        }
    }
}

void mh_print_m()
{
    printf("##### mh_mem #####\n");
    mem_pool_handler_print_pools();
}