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

/**
 * Initializs the iterator.
 *
 * @param[in] iter the iterator that is being initialized
 * @param[in] stack the stack to iterate over
 */
void stack_iter_init(StackIter *iter, Stack *s)
{
    array_iter_init(&(iter->i), s->v);
}

/**
 * Retruns the next element in the sequence and advances the iterator.
 *
 * @param[in] iter the iterator that is being advanced
 *
 * @return the next element in the sequence
 */
enum cc_stat stack_iter_next(StackIter *iter, void **out)
{
    return array_iter_next(&(iter->i), out);
}

/**
 * Replaces the last returned element by the specified iterator.
 *
 * @param[in] iter the iterator on which this operation is being performed
 * @param[in] replacement the replacement element
 *
 * @return the old element that was replaced
 */
enum cc_stat stack_iter_replace(StackIter *iter, void *element, void **out)
{
    return array_iter_replace(&(iter->i), element, out);
}

/**
 * Initializes the zip iterator.
 *
 * @param[in] iter Iterator that is being initialized
 * @param[in] q1   First stack
 * @param[in] q2   Second stack
 */
void stack_zip_iter_init(StackZipIter *iter, Stack *s1, Stack *s2)
{
    array_zip_iter_init(&(iter->i), s1->v, s2->v);
}

/**
 * Outputs the next element pair in the sequence and advances the iterator.
 *
 * @param[in]  iter Iterator that is being advanced
 * @param[out] out1 Output of the first stack element
 * @param[out] out2 Output of the second stack element
 *
 * @return CC_OK if a next element pair is returned, or CC_ITER_END if the end of one
 * of the stacks has been reached.
 */
enum cc_stat stack_zip_iter_next(StackZipIter *iter, void **out1, void **out2)
{
    return array_zip_iter_next(&(iter->i), out1, out2);
}

/**
 * Replaces the last returned element pair by <code>stack_zip_iter_next()</code>
 * with the specified replacement element pair.
 *
 * @param[in] iter  Iterator on which this operation is being performed
 * @param[in]  e1   First stack's replacement element
 * @param[in]  e2   Second stack's replacement element
 * @param[out] out1 Output of the replaced element from the first stack
 * @param[out] out2 Output of the replaced element from the second stack
 */
enum cc_stat stack_zip_iter_replace(StackZipIter *iter, void *e1, void *e2, void **out1, void **out2)
{
    return array_zip_iter_replace(&(iter->i), e1, e2, out1, out2);
}
