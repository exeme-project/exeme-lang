/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "./array.h"
#include "./panic.h"
#include "./string.h"
#include <string.h> // NOLINT(readability-duplicate-include)

struct Array* array_new(void) {
	struct Array* lp_self = malloc(ARRAY_STRUCT_SIZE);

	if (!lp_self) {
		PANIC("failed to malloc Array struct");
	}

	lp_self->length	 = 0;
	lp_self->_values = calloc(1, ARRAY_STRUCT_ELEMENT_SIZE);

	if (!lp_self->_values) {
		PANIC("failed to malloc Array values");
	}

	return lp_self;
}

void array___realloc(struct Array* p_self, size_t newLength) {
	const void** lp_valuesTemp =
		realloc(p_self->_values, newLength == 0 ? 1 : (newLength * ARRAY_STRUCT_ELEMENT_SIZE));

	if (lp_valuesTemp == NULL) {
		PANIC("failed to realloc array");
	}

	p_self->_values = lp_valuesTemp;

	if (newLength > p_self->length) {
		memset(p_self->_values + p_self->length, 0,
			   (newLength - p_self->length)
				   * ARRAY_STRUCT_ELEMENT_SIZE); // Zero out the new memory. First parameter is the
												 // pointer for the array, starting from where we
												 // reallocated from. The third parameter is the
												 // size of the newly allocated memory.
	}

	if (!p_self->_values) {
		PANIC("failed to realloc array");
	}

	p_self->length = newLength;
}

void array_insert(struct Array* p_self, size_t index, const void* p_value) {
	if (index + 1 > p_self->length) {
		array___realloc(p_self, index + 1);
	}

	p_self->_values[index] = p_value;
}

void array_append(struct Array* p_self, const void* p_value) {
	array_insert(p_self, p_self->length, p_value);
}

void array_pop(struct Array* p_self) {
	if (p_self->length < 1) {
		PANIC("nothing to pop from array");
	} else if (p_self->length == 1) {
		array___realloc(p_self, 0);
	} else {
		array___realloc(p_self, p_self->length - 1);
	}
}

void array_clear(struct Array* p_self, void (*p_freeElement)(const void*)) {
	if (p_freeElement) {
		for (size_t index = 0; index < p_self->length; index++) {
			p_freeElement(p_self->_values[index]);
		}
	}

	array___realloc(p_self, 0);
}

const void* array_get(struct Array* p_self, size_t index) {
	if (index + 1 > p_self->length) {
		PANIC("array get index out of bounds");
	}

	return p_self->_values[index];
}

bool array___match_string(const void* p_element, const void* p_match) {
	return strcmp(p_element, p_match) == 0;
}

int array_find(struct Array* p_self, bool (*p_matcher)(const void*, const void*), void* p_match) {
	for (size_t index = 0; index < p_self->length; index++) {
		const void* lp_element = p_self->_values[index];

		if (p_matcher(lp_element, p_match)) {
			return (int)index;
		}
	}

	return -1;
}

bool array_contains(struct Array* p_self, bool (*p_matcher)(const void*, const void*),
					void*		  p_match) {
	return array_find(p_self, p_matcher, p_match) != -1;
}

bool array_index_occupied(struct Array* p_self, size_t index) {
	if (index + 1 > p_self->length) {
		return false;
	}

	return p_self->_values[index] != NULL;
}

char* array_join(struct Array* p_self, const char* p_separator, char* (*p_stringify)(const void*)) {
	char* lp_joined = duplicate_string(
		p_stringify(p_self->_values[0])); // Malloc a duplicate of the first element

	for (size_t index = 1; index < p_self->length; index++) {
		char* lp_temp = CONCATENATE_STRING(
			lp_joined, p_separator,
			p_stringify(p_self->_values[index])); // Concatenate the next element with the separator
												  // in-between the previous.
		free(lp_joined); // Since CONCATENATE_STRING mallocs a new string, we need to free the old
						 // one

		lp_joined = lp_temp;
	}

	return lp_joined;
}

void array_free(struct Array** p_self) {
	if (p_self && *p_self) {
		free((*p_self)->_values);

		free(*p_self);
		*p_self = NULL;
	} else {
		PANIC("Array struct has already been freed");
	}
}
