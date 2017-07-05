/*
 * Collections-C
 * Copyright (C) 2013-2014 Srđan Panić <srdja.panic@gmail.com>
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

#include "array.h"
#include "bitset.h"

#define DEFAULT_SIZE 64
#define CEIL(x,y) x/y+(x%y!=0)

struct bitset_s
{
    Array *v;
    /* Memory Management Function Pointers */

    void *(*mem_alloc)  (size_t size);
    void *(*mem_calloc) (size_t blocks, size_t size);
    void  (*mem_free)   (void *block);

    /*The number of ones and zeros in the bitset*/
    size_t nOnes, nZeros;
    /*Number of the bits*/
    size_t size;
};

static void *cp(void *P);

/*
 * Initializes the fields of BitsetConf to default values
 * @param[in, out] conf BitsetConf structure that is being initialized
 */

void bitset_conf_init(BitsetConf *conf)
{
    array_conf_init(&conf->cfg);
    conf->size = DEFAULT_SIZE;
}

/**
 * Creates the new Bitset with the size of Bitset equal to DEFAULT_SIZE
 *
 * @param[out] out pointer to where the newly created Bitset is to be stored
 *
 *
 * @return CC_OK if the creation was successful, or CC_ERR_ALLOC if the
 * memory allocation for the new Bitset structure failed.
 */

enum cc_stat bitset_new(Bitset **out)
{
    BitsetConf conf;
    bitset_conf_init(&conf);
    return bitset_new_conf(&conf, out);
}

/**
 * Creates the new Bitset according to given configurations conf
 *
 * @param[in] conf the pointer to BitsetConf according to which new
 * Bitset will be made
 *
 * @param[out] out the double pointer to the Bitset object created
 * @return CC_OK if Bitset created successfully, CC_ERR_ALLOC if 
 * there was an error in allocating the memory
 */

enum cc_stat bitset_new_conf (BitsetConf const * const conf, Bitset **out)
{
    size_t i, sz;
    char *p;
    Bitset *bset = (Bitset *) conf->cfg.mem_calloc(1, sizeof(Bitset));
    if(!bset)
        return CC_ERR_ALLOC;

    bset->mem_alloc = conf->cfg.mem_alloc;
    bset->mem_calloc = conf->cfg.mem_calloc;
    bset->mem_free = conf->cfg.mem_free;

    bset->size = conf->size;
    bset->nZeros = conf->size;
    bset->nOnes = 0;

    Array *bitvec;

    enum cc_stat status;

    if((status = array_new_conf(&conf->cfg, &bitvec)) == CC_OK) {
        bset->v = bitvec;
    } else {
        conf->cfg.mem_free(bset);
        return status;
    }

    sz = CEIL(conf->size, 8);

    for(i = 0; i < sz; i++) {
        p = (char *) bset->mem_calloc(1, sizeof(char));
        if(!p) {
            array_destroy_free(bset->v);
            bset->mem_free(bset);
            return CC_ERR_ALLOC;
        }
        *p = 0;
        status = array_add(bset->v, p);
        if(status != CC_OK) {
            array_destroy_free(bset->v);
            bset->mem_free(bset);
            return status;
        }
    }

    *out = bset;
    return CC_OK;
}

/*
 * Do a deep copy of the bitset bt to the out
 * @param[in] bs bitset whose copy is to be made
 * @param[out] the bitset where the copy is to be stored
 *
 * @return CC_OK if the operation was a success otherwise CC_ERR_ALLOC
 * if there was error in allocation of memory
 */

enum cc_stat bitset_copy(Bitset *bs, Bitset **out)
{
    Bitset *copy = (Bitset *) bs->mem_alloc(sizeof(Bitset));
    if(!copy) {
        return CC_ERR_ALLOC;
    }
    if(array_copy_deep(bs->v, cp, &copy->v) != CC_OK)
        return CC_ERR_ALLOC;
    copy->mem_alloc = bs->mem_alloc;
    copy->mem_calloc = bs->mem_calloc;
    copy->mem_free = bs->mem_free;
    copy->nOnes = bs->nOnes;
    copy->nZeros = bs->nZeros;
    copy->size = bs->size;
    *out = copy;
    return CC_OK;
}

/**
 * Destroys the specified Bitset structure, while leaving the data it holds
 * intact.
 *
 * @param[in] bs the Bitset to be destroyed
 */

void bitset_destroy(Bitset *bs)
{
    array_destroy(bs->v);
    bs->mem_free(bs);
}

/**
 * Destroys the specified Bitset structure along with all the data it holds.
 *
 * @note This function should not be called on a Bitset that has some of its
 * elements allocated on the Stack (stack memory of function calls).
 *
 * @param[in] bs the Bitset to be destroyed
 */

void bitset_destroy_free(Bitset *bs)
{
    array_destroy_free(bs->v);
    bs->mem_free(bs);
}

/*
 * Return the bit at the position specified in the Bitset
 * The index starts from left and is 0-indexed
 *
 * @param[in] bs the Bitset whose bit we need
 * @param[index] the position parameter
 *
 * @return 1, if the bit at given position was set
 *         0, if the bit at given position was unset
 *        -1, if the bit at given position was invalid
 */

ssize_t bitset_getbit_at(Bitset *bs, size_t index)
{
    size_t q = index / 8, r = 7 - index % 8;
    char *ch;
    if(index >= bs->size)
        return INVALID_BIT;
    array_get_at(bs->v, q, (void **)&ch);
    if((*ch & (1 << r)) == 0)
        return 0;
    else
        return 1;
}

/*
 * Sets the bit at the position specified in the bitset
 *
 * @param[in] bs the bitmask whose bit need to be set
 * @param[in] index the position of the bit to be set
 * @return CC_OK if the operation was successful otherwise
 * if the position was invalid, CC_ERR_OUT_OF_RANGE is returned
 */

enum cc_stat bitset_setbit_at(Bitset *bs, size_t index)
{
    size_t q = index / 8, r = 7 - index % 8;
    char *ch;
    int bit;
    if(index >= bs->size)
        return CC_ERR_OUT_OF_RANGE;
    bit = bitset_getbit_at(bs, index);
    if(bit == 0)
        bs->nOnes++, bs->nZeros--;
    array_get_at(bs->v, q, (void **)&ch);
    *ch = *ch | (1 << r);
    return CC_OK;
}

/*
 * Unsets the bit at the position specified in the bitset
 *
 * @param[in] bs the bitmask whose bit need to be unset
 * @param[in] index the position of the bit to be unset
 * @return CC_OK if the operation was successful otherwise
 * if the position was invalid, CC_ERR_OUT_OF_RANGE is returned
 */

enum cc_stat bitset_unsetbit_at(Bitset *bs, size_t index)
{
    size_t q = index / 8, r = 7 - index % 8;
    char *ch;
    if(index >= bs->size)
        return CC_ERR_OUT_OF_RANGE;
    int bit = bitset_getbit_at(bs, index);
    if(bit == 1)
        bs->nOnes--, bs->nZeros++;
    array_get_at(bs->v, q, (void **)&ch);
    *ch = *ch & ~(1 << r);
    return CC_OK;
}

/*
 * Flips the bit at the position specified in the bitset
 *
 * @param[in] bs the bitmask whose bit need to be flip
 * @param[in] index the position of the bit to be flip
 * @return CC_OK if the operation was successful otherwise
 * if the position was invalid, CC_ERR_OUT_OF_RANGE is returned
 */

enum cc_stat bitset_flipbit_at(Bitset *bs, size_t index)
{
    int bit = bitset_getbit_at(bs, index);
    if(index >= bs->size)
        return CC_ERR_OUT_OF_RANGE;
    if(bit == 0)
        return bitset_setbit_at(bs, index);
    else if(bit == 1)
        return bitset_unsetbit_at(bs, index);
    else
        return CC_ERR_OUT_OF_RANGE;

}

/*
 * Sets the bits in the given range
 * @param[in] bs the input bitset
 * @param[in] l the lower bound parameter
 * @param[in] r upper bound parameter
 *
 * @return CC_OK if the operation was successful,
 * otherwise CC_ERR_OUT_OF_RANGE if the range is not valid
 */

enum cc_stat bitset_setbits_range(Bitset *bs, size_t l, size_t r)
{
    size_t i;
    if(l >= bs->size || r >= bs->size)
        return CC_ERR_OUT_OF_RANGE;
    for(i = l; i < r; i++)
        bitset_setbit_at(bs, i);
    return CC_OK;
}

/*
 * Unsets the bits in the given range
 * @param[in] bs the input bitset
 * @param[in] l the lower bound parameter
 * @param[in] r upper bound parameter
 *
 * @return CC_OK if the operation was successful,
 * otherwise CC_ERR_OUT_OF_RANGE if the range is not valid
 */

enum cc_stat bitset_unsetbits_range(Bitset *bs, size_t l, size_t r)
{
    size_t i;
    if(l >= bs->size || r >= bs->size)
        return CC_ERR_OUT_OF_RANGE;
    for(i = l; i < r; i++)
        bitset_unsetbit_at(bs, i);
    return CC_OK;
}

/*
 * Flips the bits in the given range
 * @param[in] bs the input bitset
 * @param[in] l the lower bound parameter
 * @param[in] r upper bound parameter
 *
 * @return CC_OK if the operation was successful,
 * otherwise CC_ERR_OUT_OF_RANGE if the range is not valid
 */

enum cc_stat bitset_flipbits_range(Bitset *bs, size_t l, size_t r)
{
    size_t i;
    if(l >= bs->size || r >= bs->size)
        return CC_ERR_OUT_OF_RANGE;
    for(i = l; i < r; i++)
        bitset_flipbit_at(bs, i);
    return CC_OK;
}

/*
 * Return the number of the set bits in the bitset
 * @param[in] bs the bitset on which this query is done
 *
 * @return the number of ones in the bitset
 */

size_t bitset_count_ones(Bitset *bs)
{
    return bs->nOnes;
}

/*
 * Return the number of the unset bits in the bitset
 * @param[in] bs the bitset on which this query is done
 *
 * @return the number of zeros in the bitset
 */

size_t bitset_count_zeros(Bitset *bs)
{
    return bs->nZeros;
}

/*
 * Return the number of the bits in the bitset
 * @param[in] bs the bitset on which this query is done
 * @return the size of the bitset bs
 */

size_t bitset_size(Bitset *bs)
{
    return bs->size;
}

/*
 * Apply the bitwise AND operator on the bitsets bs1 and bs2
 * if the size of the two bitsets are not equal then the 
 * result will be of big bitset and the remaining bits of the bitset
 * are 0 on which the operation can't be performed.
 * The AND operation will be performed in the direction of right to left
 * which is according to standard AND operation.
 *
 * The result will be stored int the out
 *
 * @param[in] bs1 is input bitset
 * @param[in] bs2 the second bitset
 * @param[out] out set the output to the out
 *
 * @return CC_OK if there is success or CC_ERR_ALLOC if there was error in
 * allocation of memory for the new bitset
 */

enum cc_stat bitset_and(Bitset *bs1, Bitset *bs2, Bitset **out)
{
    Bitset *tmp1, *tmp2;
    enum cc_stat status;
    bs1->size > bs2->size ? (status = bitset_copy(bs1, &tmp1), tmp2 = bs2) : (status = bitset_copy(bs2, &tmp1), tmp2 = bs1);
    if(status != CC_OK)
        return status;
    ssize_t i, j;
    for(i = tmp1->size - 1, j = tmp2->size - 1; i >= 0 && j >= 0; i--, j--)
    {
        ssize_t bit1 = bitset_getbit_at(tmp1, i);
        ssize_t bit2 = bitset_getbit_at(tmp2, j);
        bit1 = bit1 & bit2;
        if(bit1 == 0)
            bitset_unsetbit_at(tmp1, i);
        else
            bitset_setbit_at(tmp1, i);
    }
    for(i = tmp1->size - tmp2->size - 1; i >= 0; i--)
        bitset_unsetbit_at(tmp1, i);
    *out = tmp1;
    return CC_OK;
}

/*
 * Apply the bitwise OR operator on the bitsets bs1 and bs2
 * if the size of the two bitsets are not equal then the 
 * result will be of big bitset and the remaining bits of the bitset
 * are left as were in the big bitset on which the operation can't be performed.
 * As the standard OR operation the 0 | X = X, so they were left as it is
 * The OR operation will be performed in the direction of right to left
 * which is according to standard OR operation.
 *
 * The result will be stored int the out
 *
 * @param[in] bs1 is input bitset
 * @param[in] bs2 the second bitset
 * @param[out] out set the output to the out
 *
 * @return CC_OK if there is success or CC_ERR_ALLOC if there was error in
 * allocation of memory for the new bitset
 */

enum cc_stat bitset_or(Bitset *bs1, Bitset *bs2, Bitset **out)
{
    Bitset *tmp1, *tmp2;
    enum cc_stat status;
    bs1->size > bs2->size ? (status = bitset_copy(bs1, &tmp1), tmp2 = bs2) : (status = bitset_copy(bs2, &tmp1), tmp2 = bs1);
    if(status != CC_OK)
        return status;
    ssize_t i, j;
    for(i = tmp1->size - 1, j = tmp2->size - 1; i >= 0 && j >= 0; i--, j--)
    {
        ssize_t bit1 = bitset_getbit_at(tmp1, i);
        ssize_t bit2 = bitset_getbit_at(tmp2, j);
        bit1 = bit1 | bit2;
        if(bit1 == 0)
            bitset_unsetbit_at(tmp1, i);
        else
            bitset_setbit_at(tmp1, i);
    }
    *out = tmp1;
    return CC_OK;

}

/*
 * Apply the bitwise XOR operator on the bitsets bs1 and bs2
 * if the size of the two bitsets are not equal then the 
 * result will be of big bitset and the remaining bits of the bitset
 * are left as were in the big bitset on which the operation can't be performed.
 * As the standard OR operation the 0 ^ X = X, so they were left as it is
 * The XOR operation will be performed in the direction of right to left
 * which is according to standard XOR operation.
 *
 * The result will be stored int the out
 *
 * @param[in] bs1 is input bitset
 * @param[in] bs2 the second bitset
 * @param[out] out set the output to the out
 *
 * @return CC_OK if there is success or CC_ERR_ALLOC if there was error in
 * allocation of memory for the new bitset
 */

enum cc_stat bitset_xor(Bitset *bs1, Bitset *bs2, Bitset **out)
{
    Bitset *tmp1, *tmp2;
    enum cc_stat status;
    bs1->size > bs2->size ? (status = bitset_copy(bs1, &tmp1), tmp2 = bs2) : (status = bitset_copy(bs2, &tmp1), tmp2 = bs1);
    if(status != CC_OK)
        return status;
    ssize_t i, j;
    for(i = tmp1->size - 1, j = tmp2->size - 1; i >= 0 && j >= 0; i--, j--)
    {
        ssize_t bit1 = bitset_getbit_at(tmp1, i);
        ssize_t bit2 = bitset_getbit_at(tmp2, j);
        bit1 = bit1 ^ bit2;
        if(bit1 == 0)
            bitset_unsetbit_at(tmp1, i);
        else
            bitset_setbit_at(tmp1, i);
    }
    *out = tmp1;
    return CC_OK;

}

/*
 * Do the bitwise NOT operatinon on bs and store the result in out
 *
 * @param[in] bs the input bitset
 * @param[out] out the output of function
 *
 * @return CC_ERR_ALLOC if error in allocation of memory
 * otherwise CC_OK
 */

enum cc_stat bitset_not(Bitset *bs, Bitset **out)
{
    Bitset *tmp;
    enum cc_stat status = bitset_copy(bs, &tmp);
    if(status != CC_OK)
        return status;
    bitset_flipbits_range(tmp, 0, tmp->size - 1);
    *out = tmp;
    return CC_OK;
}

/*
 * A helper function for the bitset_copy, creates a memory of 1 byte and returns
 * @param[in] P the pointer to the memory whose copy is needed
 * @return ret the pointer of new memory location and value of P copied to it
 */

static void *cp(void *P)
{
    char *ret = (char *) malloc(sizeof(char));
    *ret = *((char *)P);
    return (void *) ret;
}
