/**
 * Part of the Exeme Language Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "../includes.c"

#include "./panic.c"

/**
 * Represents a string.
 */
struct String {
	char *_value;
	size_t length;
};

#define STRING_STRUCT_SIZE sizeof(struct String)

/**
 * Creates a new String struct.
 *
 * @return The created String struct.
 */
struct String *string_new(char *string) {
	struct String *self = malloc(STRING_STRUCT_SIZE);
	size_t stringSize = sizeof(string);

	if (!self) {
		panic("failed to create String struct");
	}

	self->length = 0;
	self->_value = malloc(stringSize);

	if (!self->_value) {
		panic("failed to malloc string");
	}

	for (size_t index = 0; index < stringSize - 1; index++) {
		self->_value[index] = string[index];
		self->length++;
	}

	self->_value[self->length] = '\0';

	return self;
}
