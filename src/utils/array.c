// Part of the Exeme Language Project, under the MIT license. See '/LICENSE' for
// license information. SPDX-License-Identifier: MIT License.

#pragma once

#include "../includes.c"

struct array {
	size_t length;
	const void **_values;
};

#define ARRAY_STRUCT_SIZE sizeof(struct array)
#define ARRAY_STRUCT_ELEMENT_SIZE sizeof(void *)

void array___realloc(struct array *self, size_t bufSize) {
	self->_values = realloc(self->_values, bufSize);

	if (!self->_values) {
		panic("failed to realloc array values");
	}
}

void array_insert(struct array *self, size_t index, const void *value) {
	if (index + 1 > self->length) {
		array___realloc(self, (index + 1) * ARRAY_STRUCT_ELEMENT_SIZE);
		self->length = index + 1;
	}

	self->_values[index] = value;
}

bool array_pop(struct array *self) {
	if (self->length < 1) {
		panic("nothing to pop from array");
	} else if (self->length == 1) {
		self->length = 0;
		array___realloc(self, 1);
	} else {
		self->length--;
		array___realloc(self, self->length * ARRAY_STRUCT_ELEMENT_SIZE);
	}

	return true;
}

const void *array_get(struct array *self, size_t index) {
	if (index + 1 > self->length) {
		panic("array get index out of bounds");
	}

	return self->_values[index];
}

void array_free(struct array *self) {
	if (self) {
		free(self->_values);
		free(self);

		self = NULL;
	} else {
		panic("array has already been freed");
	}
}

struct array *array_new(void) {
	struct array *self = malloc(ARRAY_STRUCT_SIZE);

	if (!self) {
		panic("failed to create array struct");
	}

	self->length = 0;
	self->_values = malloc(1);

	return self;
}
