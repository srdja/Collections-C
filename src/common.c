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


#include "common.h"


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

/**
 * Pointer comparator function.
 *
 * @param[in] ptr1 first pointer
 * @param[in] ptr2 second pointer
 *
 * @return
 */
int cc_common_cmp_ptr(const void *ptr1, const void *ptr2)
{
    if (ptr1 < ptr2)
        return -1;
    else if (ptr1 > ptr2)
        return 1;
    return 0;
}
