/*
 * Collections-C
 * Copyright (C) 2013-2017 Srđan Panić <srdja.panic@gmail.com>
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

#ifndef COLLECTIONS_C_BITSET_H
#define COLLECTIONS_C_BITSET_H

#include "array.h"
#include "common.h"
#define INVALID_BIT -1

/*
 * A static, space optimized Bitset, all initiallized to 0 and have a constant size.
 * Various bitwise operations can be performed on it
 *
 * It is made on array_s, the size taken for its parameter will be the number of
 * bits the actual value for the size of will be calculated internally
 */
typedef struct array_s Bitset;


typedef struct bitset_conf_s
{
    /*
     * The Bitset struct is based on the Array so ArrayConf is added */

    ArrayConf cfg;

    /*
     * The size is the number of the bits, this is added explicitly even
     * ArrayConf has size as it makes the use of BitsetConf easier */

    size_t size;
} BitsetConf;

enum cc_stat bitset_new                (Bitset **out);
enum cc_stat bitset_new_conf           (BitsetConf const * const conf, Bitset **out);
void         bitset_conf_init          (BitsetConf *cfg);
enum cc_stat bitset_copy               (Bitset *bs1, Bitset **out);

void          bitset_destroy           (Bitset *bs);
void          bitset_destroy_free      (Bitset *bs);

enum cc_stat bitset_setbits_range      (Bitset *bs, size_t l, size_t r);
enum cc_stat bitset_unsetbits_range    (Bitset *bs, size_t l, size_t r);
enum cc_stat bitset_flipbits_range     (Bitset *bs, size_t l, size_t r);
enum cc_stat bitset_setbit_at          (Bitset *bs, size_t position);
enum cc_stat bitset_unsetbit_at        (Bitset *bs, size_t position);
enum cc_stat bitset_flipbit_at         (Bitset *bs, size_t position);
int          bitset_getbit_at          (Bitset *bs, size_t position);
enum cc_stat bitset_and_operator       (Bitset *bs1, Bitset *bs2);
enum cc_stat bitset_or_operator        (Bitset *bs1, Bitset *bs2);
enum cc_stat bitset_xor_operator       (Bitset *bs1, Bitset *bs2);
enum cc_stat bitset_not_operator       (Bitset *bs);
enum cc_stat bitset_left_shift         (Bitset *bs);
enum cc_stat bitset_left_shift_cyclic  (Bitset *bs);
enum cc_stat bitset_right_shift        (Bitset *bs);
enum cc_stat bitset_right_shift_cyclic (Bitset *bs);
int  bitset_count_ones                 (Bitset *bs);
int  bitset_count_zeros                (Bitset *bs);
enum cc_stat bitset_add                (Bitset *bs1, Bitset *bs2, Bitset **out);
enum cc_stat bitset_sub                (Bitset *bs1, Bitset *bs2, Bitset **out);
enum cc_stat bitset_multiply           (Bitset *bs1, Bitset *bs2, Bitset **out);

#endif
