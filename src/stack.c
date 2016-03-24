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
 * @param[in, out] conf the configuration object that is being initialized
 */
void stack_conf_init(StackConf *conf)
{
    array_conf_init(conf);
}

/**
 * Returns a new empty stack, or NULL if the space allocation fails.
 *
 * @return a new empty stack, or NULL.
 */
enum cc_stat stack_new(Stack **out)
{
    StackConf conf;
    stack_conf_init(&conf);
    return stack_new_conf(&conf, out);
}

/**
 * Returns a new empty stack based on the StackConf struct.
 *
 * The stack is allocated using the allocators specified in the StackConf
 * struct. The allocation may fail if the undlying allocator fails.
 *
 * @param[in] conf the stack configuration
 *
 * @return a new empty stack, or NULL if the allocation fails
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
    if (!(status = array_new_conf(conf, &array))) {
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
 * @param[in] stack the stack to be destroyed
 */
void stack_destroy(Stack *stack)
{
    array_destroy(stack->v);
    stack->mem_free(stack);
}

/**
 * Destroys the specified stack structure along with all the data it holds.
 *
 * @param[in] stack the stack to be destroyed
 */
void stack_destroy_free(Stack *stack)
{
    array_destroy_free(stack->v);
    free(stack);
}

/**
 * Pushes a new element onto the stack. This operation may fail if the space
 * for the new element cannot be allocated.
 *
 * @param[in] stack the stack on which the elment is being pushed onto
 * @param[in] element the element being pushed onto the stack
 *
 * @return true if the operation was successful, false if not
 */
enum cc_stat stack_push(Stack *stack, void *element)
{
    return array_add(stack->v, element);
}

/**
 * Returns the top element of the stack without removing it.
 *
 * @param[in] stack the stack whose top element is being returned
 *
 * @return the top element of the stack
 */
enum cc_stat stack_peek(Stack *stack, void **out)
{
    return array_get_last(stack->v, out);
}

/**
 * Returns and removes the top element of the stack.
 *
 * @param[in] stack the stack whose top element is being popped
 *
 * @return the top element of the stack
 */
enum cc_stat stack_pop(Stack *stack, void **out)
{
    return array_remove_last(stack->v, out);
}

/**
 * Returns the number stack element.
 *
 * @param[in] stack the stack whose number of elements is being returned.
 *
 * @return the number of stack elements
 */
size_t stack_size(Stack *stack)
{
    return array_size(stack->v);
}

/**
 * A stack foreach loop function that invokes the specified function on
 * each element of the stack.
 *
 * @param[in] stack the stack on which this operation being performed
 * @param[in] op the operation function that is to be invoked on each
 *               element of the stack
 */
void stack_map(Stack *stack, void (*fn) (void *))
{
    array_map(stack->v, fn);
}
