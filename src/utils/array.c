/**
 * Part of the Exeme Language Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "../includes.c"

#include "./panic.c"

/**
 * Represents an array.
 */
struct Array {
	size_t length;
	void **_values;
};

#define ARRAY_STRUCT_SIZE sizeof(struct Array)
#define ARRAY_STRUCT_ELEMENT_SIZE sizeof(void *)

/**
 * Creates a new Array struct.
 *
 * @return The created Array struct.
 */
struct Array *array_new(void) {
	struct Array *self = malloc(ARRAY_STRUCT_SIZE);

	if (!self) {
		panic("failed to malloc Array struct");
	}

	self->length = 0;
	self->_values = malloc(1);

	return self;
}

/**
 * Reallocates the struct's array.
 *
 * @param self The current Array struct.
 * @param size The new size of the array.
 */
void array___realloc(struct Array *self, size_t size) {
	self->_values = realloc(self->_values, size);

	if (!self->_values) {
		panic("failed to realloc array");
	}
}

/**
 * Inserts a value at the specified index. Will expand the array's size if
 * needed.
 *
 * @param self  The current Array struct.
 * @param index The index at which to insert the value.
 * @param value The value to insert.
 */
void array_insert(struct Array *self, size_t index, void *value) {
	if (index + 1 > self->length) {
		array___realloc(self, (index + 1) * ARRAY_STRUCT_ELEMENT_SIZE);
		self->length = index + 1;
	}

	self->_values[index] = value;
}

/**
 * Removes the last element from the array.
 *
 * @param self The current Array struct.
 */
void array_pop(struct Array *self) {
	if (self->length < 1) {
		panic("nothing to pop from array");
	} else if (self->length == 1) {
		self->length = 0;
		array___realloc(self, 1);
	} else {
		self->length--;
		array___realloc(self, self->length * ARRAY_STRUCT_ELEMENT_SIZE);
	}
}

/**
 * Removes all the elements from the array.
 *
 * @param self The current Array struct.
 */
void array_clear(struct Array *self) {
	self->length = 0;
	array___realloc(self, 1);
}

/**
 * Returns the element at the specified index in the array.
 *
 * @param self  The current Array struct.
 * @param index The index from which to get the element.
 *
 * @return The retreived element.
 */
void *array_get(struct Array *self, size_t index) {
	if (index + 1 > self->length) {
		panic("array get index out of bounds");
	}

	return self->_values[index];
}

/**
 * Iterates through the array, using the passed function to check for matches.
 *
 * @param self    The current Array struct.
 * @param matcher The function to check for matches.
 *
 * @return If a match was found.
 */
bool array_find(struct Array *self, bool (*matcher)(void *, void *),
				void *match) {
	for (size_t index = 0; index < self->length; index++) {
		void *element = self->_values[index];

		if (matcher(element, match)) {
			return true;
		}
	}

	return false;
}

/**
 * Frees an Array struct.
 *
 * @param self The current Array struct.
 */
void array_free(struct Array *self) {
	if (self) {
		free(self->_values);
		free(self);

		self = NULL;
	} else {
		panic("Array struct has already been freed");
	}
}
