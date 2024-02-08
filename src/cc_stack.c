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

#include "cc_array.h"
#include "cc_stack.h"


struct cc_stack_s {
    CC_Array *v;

    void *(*mem_alloc)  (size_t size);
    void *(*mem_calloc) (size_t blocks, size_t size);
    void  (*mem_free)   (void *block);
};


/**
 * Initializes the fields of the CC_StackConf struct to default values.
 *
 * @param[in, out] conf CC_StackConf structure that is being initialized
 */
void cc_stack_conf_init(CC_StackConf *conf)
{
    cc_array_conf_init(conf);
}

/**
 * Creates a new empty stack and returns a status code.
 *
 * @param[out] out pointer to where the newly created CC_Stack is to be stored
 *
 * @return CC_OK if the creation was successful, or CC_ERR_ALLOC if the
 * memory allocation for the new CC_Stack structure failed.
 */
enum cc_stat cc_stack_new(CC_Stack **out)
{
    CC_StackConf conf;
    cc_stack_conf_init(&conf);
    return cc_stack_new_conf(&conf, out);
}

/**
 * Creates a new empty CC_Stack based on the CC_StackConf struct and returns a
 * status code.
 *
 * The stack is allocated using the allocators specified in the CC_StackConf
 * struct. The allocation may fail if the underlying allocator fails. It may also
 * fail if the values of exp_factor and capacity in the CC_StackConf do not meet
 * the following condition: <code>exp_factor < (CC_MAX_ELEMENTS / capacity)</code>.
 *
 * @param[in] conf stack configuration structure
 * @param[out] out pointer to where the newly created CC_Stack is to be stored
 *
 * @return CC_OK if the creation was successful, CC_ERR_INVALID_CAPACITY if
 * the above mentioned condition is not met, or CC_ERR_ALLOC if the memory
 * allocation for the new CC_Stack structure failed.
 */
enum cc_stat cc_stack_new_conf(CC_StackConf const * const conf, CC_Stack **out)
{
    CC_Stack *stack = conf->mem_calloc(1, sizeof(CC_Stack));

    if (!stack)
        return CC_ERR_ALLOC;

    stack->mem_alloc  = conf->mem_alloc;
    stack->mem_calloc = conf->mem_calloc;
    stack->mem_free   = conf->mem_free;

    CC_Array *array;
    enum cc_stat status;
    if ((status = cc_array_new_conf(conf, &array)) == CC_OK) {
        stack->v = array;
    } else {
        conf->mem_free(stack);
        return status;
    }
    *out = stack;
    return CC_OK;
}

/**
 * Destroys the specified stack structure, while leaving the data it holds
 * intact.
 *
 * @param[in] stack the CC_Stack to be destroyed
 */
void cc_stack_destroy(CC_Stack *stack)
{
    cc_array_destroy(stack->v);
    stack->mem_free(stack);
}

/**
 * Destroys the specified stack structure along with all the data it holds.
 *
 * @note This function should not be called on a CC_Stack that has some of its
 * elements allocated on the stack (stack memory).
 *
 * @param[in] stack the stack to be destroyed
 */
void cc_stack_destroy_cb(CC_Stack *stack, void (*cb) (void*))
{
    cc_array_destroy_cb(stack->v, cb);
    free(stack);
}

/**
 * Pushes a new element onto the stack.
 *
 * @param[in] stack the stack on which the element is being pushed onto
 * @param[in] element the element being pushed onto the stack
 *
 * @return CC_OK if the element was successfully pushed, or CC_ERR_ALLOC
 * if the memory allocation for the new element failed.
 */
enum cc_stat cc_stack_push(CC_Stack *stack, void *element)
{
    return cc_array_add(stack->v, element);
}

/**
 * Gets the top element of the CC_Stack without removing it and sets the out
 * parameter to its value.
 *
 * @param[in] stack the stack whose top element is being returned
 * @param[out] out pointer to where the returned value is stored
 *
 * @return CC_OK if the element was found, or CC_ERR_VALUE_NOT_FOUND if the
 * CC_Stack is empty.
 */
enum cc_stat cc_stack_peek(CC_Stack *stack, void **out)
{
    return cc_array_get_last(stack->v, out);
}

/**
 * Pops (removes) the top element of the stack and optionally sets the out
 * parameter to the value of the popped element.
 *
 * @param[in] stack the stack whose top element is being popped
 * @param[out] out pointer to where the popped value is stored, or NULL if it is
 *                 to be ignored
 *
 * @return CC_OK if the element was successfully popped, or CC_ERR_OUT_OF_RANGE
 * if the CC_Stack is already empty.
 */
enum cc_stat cc_stack_pop(CC_Stack *stack, void **out)
{
    return cc_array_remove_last(stack->v, out);
}

/**
 * Returns the number of CC_Stack elements.
 *
 * @param[in] stack the CC_Stack whose number of elements is being returned
 *
 * @return the number of CC_Stack elements.
 */
size_t cc_stack_size(CC_Stack *stack)
{
    return cc_array_size(stack->v);
}

/**
 * Applies the function fn to each element of the CC_Stack.
 *
 * @param[in] stack the CC_Stack on which this operation being performed
 * @param[in] fn the operation function that is to be invoked on each
 *               element of the CC_Stack
 */
void cc_stack_map(CC_Stack *stack, void (*fn) (void *))
{
    cc_array_map(stack->v, fn);
}

/**
 * Filters the CC_Stack by modifying it. It removes all elements that don't
 * return true on pred(element)..
 *
 * @param[in] stack the CC_Stack on which this operation being performed
 * @param[in] predicate predicate function which returns true if the element should
 *                 be kept in the CC_Stack
 *
 * @return CC_OK if the CC_Stack was filtered successfully, or CC_ERR_OUT_OF_RANGE
 *  if the CC_Stack is empty.
 */

enum cc_stat cc_stack_filter_mut(CC_Stack *stack, bool (*predicate)(const void *))
{
    return cc_array_filter_mut(stack->v, predicate);
}

/**
 * Filters the CC_Stack by creating a new CC_Stack that contains all elements from the
 * original CC_Stack that return true on pred(element) without modifying the original
 * CC_Stack.
 *
 * @param[in] stack   CC_Stack that is to be filtered
 * @param[in] predicate predicate function which returns true if the element should
 *                 be kept in the filtered stack
 * @param[out] out pointer to where the new filtered CC_Stack is to be stored
 *
 * @return CC_OK if the CC_Stack was filtered successfully, CC_ERR_OUT_OF_RANGE
 * if the CC_Stack is empty, or CC_ERR_ALLOC if the memory allocation for the
 * new CC_Stack failed.
 */

enum cc_stat cc_stack_filter(CC_Stack *stack, bool (*predicate)(const void *), CC_Stack **out)
{
    if (cc_stack_size(stack) == 0)
        return CC_ERR_OUT_OF_RANGE;

    CC_StackIter iter;
    cc_stack_iter_init(&iter, stack);

    cc_stack_new(out);

    void *e;
    while (cc_stack_iter_next(&iter, &e) != CC_ITER_END)
    {
        if (predicate(e))
            cc_stack_push(*out, e);
    }

    return CC_OK;
}

/**
 * Initializes the iterator.
 *
 * @param[in] iter the iterator that is being initialized
 * @param[in] s the stack to iterate over
 */
void cc_stack_iter_init(CC_StackIter *iter, CC_Stack *s)
{
    cc_array_iter_init(&(iter->i), s->v);
}

/**
 * Advances the iterator and sets the out parameter to the value of the
 * next element in the sequence.
 *
 * @param[in] iter the iterator that is being advanced
 * @param[out] out pointer to where the next element is set
 *
 * @return CC_OK if the iterator was advanced, or CC_ITER_END if the
 * end of the CC_Stack has been reached.
 */
enum cc_stat cc_stack_iter_next(CC_StackIter *iter, void **out)
{
    return cc_array_iter_next(&(iter->i), out);
}

/**
 * Replaces the last returned element by <code>cc_stack_iter_next()</code>
 * with the specified element and optionally sets the out parameter to
 * the value of the replaced element.
 *
 * @note This function should only ever be called after a call to <code>
 * cc_stack_iter_next()</code>.
 *
 * @param[in] iter the iterator on which this operation is being performed
 * @param[in] element the replacement element
 * @param[out] out pointer to where the replaced element is stored, or NULL
 *                if it is to be ignored
 *
 * @return  CC_OK if the element was replaced successfully, or
 * CC_ERR_OUT_OF_RANGE.
 */
enum cc_stat cc_stack_iter_replace(CC_StackIter *iter, void *element, void **out)
{
    return cc_array_iter_replace(&(iter->i), element, out);
}

/**
 * Initializes the zip iterator.
 *
 * @param[in] iter iterator that is being initialized
 * @param[in] s1   first CC_Stack
 * @param[in] s2   second CC_Stack
 */
void cc_stack_zip_iter_init(CC_StackZipIter *iter, CC_Stack *s1, CC_Stack *s2)
{
    cc_array_zip_iter_init(&(iter->i), s1->v, s2->v);
}

/**
 * Outputs the next element pair in the sequence and advances the iterator.
 *
 * @param[in]  iter iterator that is being advanced
 * @param[out] out1 output of the first stack element
 * @param[out] out2 output of the second stack element
 *
 * @return CC_OK if a next element pair is returned, or CC_ITER_END if the end
 * of one of the stacks has been reached.
 */
enum cc_stat cc_stack_zip_iter_next(CC_StackZipIter *iter, void **out1, void **out2)
{
    return cc_array_zip_iter_next(&(iter->i), out1, out2);
}

/**
 * Replaces the last returned element pair by <code>cc_stack_zip_iter_next()</code>
 * with the specified replacement element pair.
 *
 * @param[in] iter  iterator on which this operation is being performed
 * @param[in]  e1   first stack's replacement element
 * @param[in]  e2   second stack's replacement element
 * @param[out] out1 output of the replaced element from the first stack
 * @param[out] out2 output of the replaced element from the second stack
 *
 * @return CC_OK if the element was successfully replaced, or CC_ERR_OUT_OF_RANGE.
 */
enum cc_stat cc_stack_zip_iter_replace(CC_StackZipIter *iter, void *e1, void *e2, void **out1, void **out2)
{
    return cc_array_zip_iter_replace(&(iter->i), e1, e2, out1, out2);
}


size_t cc_stack_struct_size()
{
    return cc_array_struct_size();
}