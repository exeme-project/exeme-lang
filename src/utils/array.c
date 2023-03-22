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

void array_insert(struct array *self, void *value, size_t index) {
	if (index + 1 > self->length) {
		array___realloc(self, (index + 1) * ARRAY_STRUCT_ELEMENT_SIZE);
		self->length = index + 1;
	}

	self->_values[index] = value;
}

bool array_pop(struct array *self) {
	if (self->length < 1) {
		return false;
	} else if (self->length == 1) {
		self->length = 0;
		self->_values = realloc(self->_values, 1);
	} else {
		self->length--;
		self->_values =
			realloc(self->_values, self->length * ARRAY_STRUCT_ELEMENT_SIZE);
	}

	return true;
}

void array_free(struct array *self) {
	if (self) {
		free(self->_values);
		free(self);

		self = NULL;
	}
}
