#include "functional.h"
#include "tasks.h"
#include "tests.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
	array_t array_t;
	int idx;
} indexed_array;

void reverse_helper(void *acc, void *elem) {
	((indexed_array *)acc)->idx--;
	*((int *)(((indexed_array *)acc)->array_t.data) + ((indexed_array *)acc)->idx) = *(int *)elem;
}

array_t reverse(array_t list) {
	indexed_array new_list;
	new_list.idx = list.len;
	new_list.array_t.data = malloc(list.elem_size * list.len);
	new_list.array_t.destructor = list.destructor;
	new_list.array_t.len = list.len;
	new_list.array_t.elem_size = list.elem_size;
	reduce(reverse_helper, &new_list, list);
	return new_list.array_t;
}

void number_t_destructor(void *elem) {
	free(((number_t *)elem)->string);
}

void construct_number(void *new_elem, void **vect) {
	((number_t *)new_elem)->integer_part = ((int **)vect)[0][0];
	((number_t *)new_elem)->fractional_part = ((int **)vect)[1][0];
	((number_t *)new_elem)->string = malloc(30);
	/* Writing the [integer_part].[fractional_part]
	to the address pointed by the string */
	snprintf(((number_t *)new_elem)->string,
			 30, "%d.%d", ((number_t *)new_elem)->integer_part,
			((number_t *)new_elem)->fractional_part);
}

array_t create_number_array(array_t integer_part, array_t fractional_part) {
	return map_multiple(construct_number, sizeof(number_t),
						number_t_destructor, 2, integer_part, fractional_part);
}

boolean pass(void *arg) {
	return ((student_t *)arg)->grade >= 5;
	/* Determines which of the students passed */
}

void get_name(void *to_create, void *student) {
	*(char **)to_create = malloc(30);
	/* Allocating memory for the string and copy it */
	strcpy(*(char **)to_create, ((student_t *)student)->name);
}

array_t get_passing_students_names(array_t list) {
	/* Filtering then mapping the initial list */
	array_t new_list = filter(pass, list);
	return map(get_name, sizeof(char *), NULL, new_list);
}

void sum(void *acc, void *elem) {
	(*(int *)acc) = (*(int *)acc) + (*(int *)elem);
	// Determine the sum of the elements of an array;
}

void sum_sum(void *new_elem, void *old_elem) {
	/* Function that maps a new list consisting of the sum
	of the elemets of the initial lits */
	int accumulator = 0;
	reduce(sum, &accumulator, *(array_t *)old_elem);
	*(int *)new_elem = accumulator;
}

void test_sum(void *new_elem, void **old_elements) {
	/* Function that maps the 2 lists accordingly. If the sum
	is greater or equal with the integer from int_list the new
	element is set to 1, otherwise it's set to 0 */
	*(boolean *)new_elem = *(*(int **)old_elements) >= *(*((int **)old_elements + 1));
}

array_t check_bigger_sum(array_t list_list, array_t int_list) {
	list_list = map(sum_sum, sizeof(int), NULL, list_list);
	/* Performing the sum of the elemets (map) then
	comparing the results (map_multiple)*/
	return map_multiple(test_sum, sizeof(boolean), NULL, 2, list_list, int_list);
}

void get_even_strings(void *acc, void *elem) {
	/* Function to be used with reduce.
	The accumulator is a struct consisting of a
	list and an index. Index is incremented
	constantly. If it is even the strings are
	copied. */
	int idx = ((indexed_array *)acc)->idx;
	if (((indexed_array *)acc)->idx % 2 == 0) {
		((char **)(((indexed_array *)acc)->array_t.data))[idx / 2] =
			malloc(sizeof(char) * 20);
		strcpy(((char **)(((indexed_array *)acc)->array_t.data))[idx / 2], *(char **)elem);
	}
	((indexed_array *)acc)->idx++;
}

array_t get_even_indexed_strings(array_t list) {
	(void)list;
	indexed_array new_list;
	new_list.idx = 0;
	new_list.array_t.destructor = list.destructor;
	new_list.array_t.len = (list.len + 1) / 2;
	new_list.array_t.elem_size = list.elem_size;
	new_list.array_t.data = malloc(new_list.array_t.len * new_list.array_t.elem_size);
	/* Done setting up the new list */
	reduce(get_even_strings, &new_list, list);
	for_each(list.destructor, list);
	free(list.data);
	/* Dealocating the old list. The destructor
	is applied only to the elements that aren't
	on evend index. */
	return new_list.array_t;
}

void list_destructor(void *list) {
	free(((array_t *)list)->data);
}

void range_helper(void *acc, void *elem) {
	*(int *)elem = ((indexed_array *)acc)->idx++;
}

array_t range(int start, int len) {
	array_t list;
	list.len = len;
	list.data = calloc(len, sizeof(int));
	list.destructor = NULL;
	list.elem_size = sizeof(int);
	indexed_array arr;
	arr.idx = start;
	arr.array_t = list;
	reduce(range_helper, &arr, list);
	/* Generating a list with values ranging from
	start to start + len */
	return arr.array_t;
}

void get_matrix(void *acc, void *elem) {
	(void)elem;
	((array_t *)((((indexed_array *)acc)->array_t).data))[((indexed_array *)acc)->idx - 1] = range(((indexed_array *)acc)->idx, ((indexed_array *)acc)->array_t.len);
	((indexed_array *)acc)->idx++;
	/* Each lits is assigned a range that start from idx */
}

array_t generate_square_matrix(int n) {
	(void)n;
	indexed_array list;
	list.array_t.destructor = list_destructor;
	list.array_t.elem_size = sizeof(array_t);
	list.array_t.len = n;
	list.array_t.data = malloc(list.array_t.len * list.array_t.elem_size);
	list.idx = 1;
	reduce(get_matrix, &list, list.array_t);
	return list.array_t;
}
