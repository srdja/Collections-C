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
#include "stack.h"


struct stack_s {
    Array *v;

    void *(*mem_alloc)  (size_t size);
    void *(*mem_calloc) (size_t blocks, size_t size);
    void  (*mem_free)   (void *block);
};


/**
 * Initializes the fields of the StackConf struct to default values.
 *
 * @param[in, out] conf StackConf structure that is being initialized
 */
void stack_conf_init(StackConf *conf)
{
    array_conf_init(conf);
}

/**
 * Creates a new empty stack and returns a status code.
 *
 * @param[out] out pointer to where the newly created Stack is to be stored
 *
 * @return CC_OK if the creation was successful, or CC_ERR_ALLOC if the
 * memory allocation for the new Stack structure failed.
 */
enum cc_stat stack_new(Stack **out)
{
    StackConf conf;
    stack_conf_init(&conf);
    return stack_new_conf(&conf, out);
}

/**
 * Creates a new empty Stack based on the StackConf struct and returns a
 * status code.
 *
 * The stack is allocated using the allocators specified in the StackConf
 * struct. The allocation may fail if the underlying allocator fails. It may also
 * fail if the values of exp_factor and capacity in the StackConf do not meet
 * the following condition: <code>exp_factor < (CC_MAX_ELEMENTS / capacity)</code>.
 *
 * @param[in] conf stack configuration structure
 * @param[out] out pointer to where the newly created Stack is to be stored
 *
 * @return CC_OK if the creation was successful, CC_ERR_INVALID_CAPACITY if
 * the above mentioned condition is not met, or CC_ERR_ALLOC if the memory
 * allocation for the new Stack structure failed.
 */
enum cc_stat stack_new_conf(StackConf const * const conf, Stack **out)
{
    Stack *stack = conf->mem_calloc(1, sizeof(Stack));

    if (!stack)
        return CC_ERR_ALLOC;

    stack->mem_alloc  = conf->mem_alloc;
    stack->mem_calloc = conf->mem_calloc;
    stack->mem_free   = conf->mem_free;

    Array *array;
    enum cc_stat status;
    if ((status = array_new_conf(conf, &array)) == CC_OK) {
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
 * @param[in] stack the Stack to be destroyed
 */
void stack_destroy(Stack *stack)
{
    array_destroy(stack->v);
    stack->mem_free(stack);
}

/**
 * Destroys the specified stack structure along with all the data it holds.
 *
 * @note This function should not be called on a Stack that has some of its
 * elements allocated on the stack (stack memory).
 *
 * @param[in] stack the stack to be destroyed
 */
void stack_destroy_cb(Stack *stack, void (*cb) (void*))
{
    array_destroy_cb(stack->v, cb);
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
enum cc_stat stack_push(Stack *stack, void *element)
{
    return array_add(stack->v, element);
}

/**
 * Gets the top element of the Stack without removing it and sets the out
 * parameter to its value.
 *
 * @param[in] stack the stack whose top element is being returned
 * @param[out] out pointer to where the returned value is stored
 *
 * @return CC_OK if the element was found, or CC_ERR_VALUE_NOT_FOUND if the
 * Stack is empty.
 */
enum cc_stat stack_peek(Stack *stack, void **out)
{
    return array_get_last(stack->v, out);
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
 * if the Stack is already empty.
 */
enum cc_stat stack_pop(Stack *stack, void **out)
{
    return array_remove_last(stack->v, out);
}

/**
 * Returns the number of Stack elements.
 *
 * @param[in] stack the Stack whose number of elements is being returned
 *
 * @return the number of Stack elements.
 */
size_t stack_size(Stack *stack)
{
    return array_size(stack->v);
}

/**
 * Applies the function fn to each element of the Stack.
 *
 * @param[in] stack the Stack on which this operation being performed
 * @param[in] fn the operation function that is to be invoked on each
 *               element of the Stack
 */
void stack_map(Stack *stack, void (*fn) (void *))
{
    array_map(stack->v, fn);
}

/**
 * Initializes the iterator.
 *
 * @param[in] iter the iterator that is being initialized
 * @param[in] s the stack to iterate over
 */
void stack_iter_init(StackIter *iter, Stack *s)
{
    array_iter_init(&(iter->i), s->v);
}

/**
 * Advances the iterator and sets the out parameter to the value of the
 * next element in the sequence.
 *
 * @param[in] iter the iterator that is being advanced
 * @param[out] out pointer to where the next element is set
 *
 * @return CC_OK if the iterator was advanced, or CC_ITER_END if the
 * end of the Stack has been reached.
 */
enum cc_stat stack_iter_next(StackIter *iter, void **out)
{
    return array_iter_next(&(iter->i), out);
}

/**
 * Replaces the last returned element by <code>stack_iter_next()</code>
 * with the specified element and optionally sets the out parameter to
 * the value of the replaced element.
 *
 * @note This function should only ever be called after a call to <code>
 * stack_iter_next()</code>.
 *
 * @param[in] iter the iterator on which this operation is being performed
 * @param[in] element the replacement element
 * @param[out] out pointer to where the replaced element is stored, or NULL
 *                if it is to be ignored
 *
 * @return  CC_OK if the element was replaced successfully, or
 * CC_ERR_OUT_OF_RANGE.
 */
enum cc_stat stack_iter_replace(StackIter *iter, void *element, void **out)
{
    return array_iter_replace(&(iter->i), element, out);
}

/**
 * Initializes the zip iterator.
 *
 * @param[in] iter iterator that is being initialized
 * @param[in] s1   first Stack
 * @param[in] s2   second Stack
 */
void stack_zip_iter_init(StackZipIter *iter, Stack *s1, Stack *s2)
{
    array_zip_iter_init(&(iter->i), s1->v, s2->v);
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
enum cc_stat stack_zip_iter_next(StackZipIter *iter, void **out1, void **out2)
{
    return array_zip_iter_next(&(iter->i), out1, out2);
}

/**
 * Replaces the last returned element pair by <code>stack_zip_iter_next()</code>
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
enum cc_stat stack_zip_iter_replace(StackZipIter *iter, void *e1, void *e2, void **out1, void **out2)
{
    return array_zip_iter_replace(&(iter->i), e1, e2, out1, out2);
}
