/*
 * Collections-C
 * Copyright (C) 2013-2016 Srđan Panić <i@srdja.me>
 *
 * This file is part of Collections-C.
 *
 * Collections-C is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Collections-C is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Collections-C.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "cc_common.h"


static CC_Allocators global_alloc = {
    .mem_alloc  = malloc,
    .mem_calloc = calloc,
    .mem_free   = free
};

CC_Allocators* cc_global_alloc = &global_alloc;


void cc_global_set_default_allocators(CC_Allocators* alloc)
{
    cc_global_alloc = alloc;
}

/**
 * String comparator function.
 *
 * @param[in] str1 first string
 * @param[in] str2 second string
 *
 * @return
 */
int cc_common_cmp_str(const void *str1, const void *str2)
{
    return strcmp((const char*) str1, (const char*) str2);
}


void cc_global_init()
{

}