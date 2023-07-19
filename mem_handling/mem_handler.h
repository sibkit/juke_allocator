//
// Created by sibkit on 21.07.2020.
//

#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include "types.h"

u8* mh_malloc(size_t size);
void mh_free(u8* cell, size_t size);

bool check_cell(u8* cell, size_t size);

void init();