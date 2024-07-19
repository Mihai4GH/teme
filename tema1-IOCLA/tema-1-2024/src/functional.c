#include "functional.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

void for_each(void (*func)(void *), array_t list)
{
	if (!func)
		return;
	for (int i = 0; i < list.len; i++)
		func(list.data + i * list.elem_size);
}

array_t map(void (*func)(void *, void *),
			int new_list_elem_size,
			void (*new_list_destructor)(void *),
			array_t list)
{	array_t new_list;
	new_list.destructor = new_list_destructor;
	new_list.elem_size = new_list_elem_size;
	new_list.len = list.len;
	new_list.data = malloc(new_list.len * new_list.elem_size);
	for (int i = 0; i < list.len; i++) {
		func(new_list.data + i * new_list.elem_size,
			 list.data + i * list.elem_size);
		if (list.destructor)
			list.destructor(list.data + i * list.elem_size);
	}
	free(list.data);
	return new_list;
}

array_t filter(boolean(*func)(void *), array_t list)
{
	array_t new_list;
	new_list.elem_size = list.elem_size;
	new_list.destructor = list.destructor;
	new_list.data = malloc(new_list.elem_size * list.len);
	int len = 0;
	for (int i = 0; i < list.len; i++) {
		if (func(list.data + i * list.elem_size) == 1) {
			memcpy(new_list.data + len * new_list.elem_size,
			       list.data + i * list.elem_size, new_list.elem_size);
			len++;
		} else {
			if (list.destructor)
				list.destructor(list.data + i * list.elem_size);
		}
	}
	if (list.elem_size > 0)
		free(list.data);
	new_list.len = len;
	new_list.data = realloc(new_list.data, len * new_list.elem_size);
	return new_list;
}

void *reduce(void (*func)(void *, void *), void *acc, array_t list)
{
	for (int i = 0; i < list.len; i++)
		func(acc, list.data + i * list.elem_size);
	return acc;
}

void for_each_multiple(void(*func)(void **), int varg_c, ...)
{
	va_list ptr;
	va_start(ptr, varg_c);
	array_t *array_of_lists = malloc(varg_c * sizeof(array_t));
	void **array_of_elements = malloc(varg_c * sizeof(void *));
	for (int i = 0; i < varg_c; i++)
		array_of_lists[i] = va_arg(ptr, array_t);
	va_end(ptr);
	int min_len = array_of_lists[0].len;
	for (int i = 1; i < varg_c; i++) {
		if (min_len > array_of_lists[i].len)
			min_len = array_of_lists[i].len;
	}
	for (int iter = 0; iter < min_len; iter++) {	// Iterez prin elemente
		for (int i = 0; i < varg_c; i++)
			array_of_elements[i] = array_of_lists[i].data +
			iter * array_of_lists[i].elem_size;
		func(array_of_elements);
	}
	free(array_of_lists);
	free(array_of_elements);
}

array_t map_multiple(void (*func)(void *, void **),
					 int new_list_elem_size,
					 void (*new_list_destructor)(void *),
					 int varg_c, ...)
{
	va_list ptr;
	array_t *array_of_lists = malloc(varg_c * sizeof(array_t)), new_list;
	void **array_of_elements = malloc(varg_c * sizeof(void *));
	new_list.elem_size = new_list_elem_size;
	new_list.destructor = new_list_destructor;
	va_start(ptr, varg_c);
	for (int i = 0; i < varg_c; i++)
		array_of_lists[i] = va_arg(ptr, array_t);
	va_end(ptr);
	int min_len = array_of_lists[0].len;
	for (int i = 1; i < varg_c; i++) {
		if (min_len > array_of_lists[i].len)
			min_len = array_of_lists[i].len;
	}
	new_list.len = min_len;
	new_list.data = NULL;
	new_list.data = malloc(new_list.len * new_list.elem_size);
	for (int iter = 0; iter < min_len; iter++) {
		for (int i = 0; i < varg_c; i++)
			array_of_elements[i] = array_of_lists[i].data +
			iter * array_of_lists[i].elem_size;
		func(new_list.data + iter * new_list.elem_size, array_of_elements);
	}
	for (int i = 0; i < varg_c; i++) {
		if (array_of_lists[i].destructor) {
			for (int iter = 0; iter < array_of_lists[i].len; iter++)
				array_of_lists[i].destructor(array_of_lists[i].data +
				iter * array_of_lists[i].elem_size);
		}
		if (array_of_lists[i].data)
			free(array_of_lists[i].data);
	}
	free(array_of_elements);
	free(array_of_lists);
	return new_list;
}

void *reduce_multiple(void(*func)(void *, void **), void *acc, int varg_c, ...)
{
	va_list ptr;
	va_start(ptr, varg_c);
	array_t *array_of_lists = malloc(varg_c * sizeof(array_t));
	void **array_of_elements = malloc(varg_c * sizeof(void *));
	for (int i = 0; i < varg_c; i++)
		array_of_lists[i] = va_arg(ptr, array_t);
	va_end(ptr);
	int min_len = array_of_lists[0].len;
	for (int i = 1; i < varg_c; i++) {
		if (min_len > array_of_lists[i].len)
			min_len = array_of_lists[i].len;
	}
	for (int iter = 0; iter < min_len; iter++) {	// Iterez prin elemente
		for (int i = 0; i < varg_c; i++)
			array_of_elements[i] = array_of_lists[i].data +
			iter * array_of_lists[i].elem_size;
		func(acc, array_of_elements);
	}
	free(array_of_lists);
	free(array_of_elements);
	return acc;
}
