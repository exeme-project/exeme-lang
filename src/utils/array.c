// Part of the Exeme Language Project, under the MIT license. See '/LICENSE' for
// license information. SPDX-License-Identifier: MIT License.

#pragma once

#include "../includes.c"
#include "./panic.c"

struct array {
	size_t length;
	void **_values;
};

#define ARRAY_STRUCT_SIZE sizeof(struct array)
#define ARRAY_STRUCT_ELEMENT_SIZE sizeof(void *)

struct array *array_new(void) {
	struct array *self = malloc(ARRAY_STRUCT_SIZE);

	self->length = 0;
	self->_values = malloc(1);

	return self;
}

void array___realloc(struct array *self, size_t bufSize) {
	self->_values = realloc(self->_values, bufSize);

	if (!self->_values) {
		panic("failed to realloc array values");
	}
}

void array_insert(struct array *self, size_t index, void *value) {
	if (index + 1 > self->length) {
		array___realloc(self, (index + 1) * ARRAY_STRUCT_ELEMENT_SIZE);
		self->length = index + 1;
	}

	self->_values[index] = value;
}

bool array_pop(struct array *self) {
	if (self->length < 1) {
		printf(
			"%s%sinternal warning%s in %s (%s:%d) - nothing to pop from array",
			F_BRIGHT_RED, S_BOLD, S_RESET, __FUNCTION__, __FILE__, __LINE__);
		return false;
	} else if (self->length == 1) {
		self->length = 0;
		array___realloc(self, 1);
	} else {
		self->length--;
		array___realloc(self, self->length * ARRAY_STRUCT_ELEMENT_SIZE);
	}

	return true;
}

void *array_get(struct array *self, size_t index) {
	if (index + 1 > self->length) {
		printf("%s%sinternal warning%s in %s (%s:%d) - array get index out of "
			   "bounds",
			   F_BRIGHT_RED, S_BOLD, S_RESET, __FUNCTION__, __FILE__, __LINE__);
		return NULL;
	}

	return self->_values[index];
}

void array_free(struct array *self) {
	if (self) {
		free(self->_values);
		free(self);

		self = NULL;
	}
}
