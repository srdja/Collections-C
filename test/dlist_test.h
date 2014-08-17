/*
 * dlist_test.h
 *
 *  Created on: Jul 29, 2014
 *      Author: srdja
 */

#ifndef DLIST_TEST_H_
#define DLIST_TEST_H_

void test_dlist_new();
void test_dlist_destroy();
void test_dlist_add();
void test_dlist_add_last();
void test_dlist_add_first();
void test_dlist_add_all();
void test_dlist_add_all_at();
void test_dlist_add_at();
void test_dlist_remove();
void test_dlist_remove_first();
void test_dlist_remove_last();
void test_dlist_remove_at();
void test_dlist_remove_all();
void test_dlist_get_first();
void test_dlist_get_last();
void test_dlist_get();
void test_dlist_index_of();
void test_dlist_contains();
void test_dlist_replace_at();
void test_dlist_copy_shallow();
void test_dlist_copy_deep();
void test_dlist_sublist();
void test_dlist_sort();
void test_dlist_iter();
void test_dlist_iter_desc();
void test_dlist_reverse();

void test_dlist_splice();
void test_dlist_splice_after();
void test_dlist_splice_before();
void test_dlist_to_array();

void dlist_test_init();

#endif /* DLIST_TEST_H_ */
