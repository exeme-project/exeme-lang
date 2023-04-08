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
struct String *string_new(char *string, bool copy) {
	struct String *self = malloc(STRING_STRUCT_SIZE);
	size_t stringSize = sizeof(string);

	if (!self) {
		panic("failed to malloc String struct");
	}

	if (copy) {
		self->_value = malloc(stringSize);

		if (!self->_value) {
			panic("failed to malloc string");
		}

		for (size_t index = 0; index < stringSize - 1; index++) {
			self->_value[index] = string[index];
			self->length++;
		}

		self->_value[self->length] = '\0';
	} else {
		self->_value = string;
		self->length = strlen(self->_value);
	}

	return self;
}

/**
 * Reallocates the struct's string.
 *
 * @param self The current String struct.
 * @param size The new size of the string.
 */
void string___realloc(struct String *self, size_t size) {
	self->_value = realloc(self->_value, size);

	if (!self->_value) {
		panic("failed to realloc string");
	}
}

void string_append(struct String *self, char chr) {
	string___realloc(self, self->length + 1);

	self->_value[self->length++] = chr;
	self->_value[self->length] = '\0';
}

/**
 * Removes all the elements from the String.
 *
 * @param self The current String struct.
 */
void string_clear(struct String *self) {
	self->_value[0] = '\0';
	self->length = 1;
	string___realloc(self, 1);
}

/**
 * Frees an String struct.
 *
 * @param self The current String struct.
 */
void string_free(struct String *self) {
	if (self) {
		free(self->_value);
		free(self);

		self = NULL;
	} else {
		panic("String struct has already been freed");
	}
}

/**
 * Concatenates the specified amount of strings together.
 *
 * @param argumentsNumber The amount of strings to concatenate.
 *
 * @return char* - The concatenated string.
 */
char *stringConcatenate(size_t argumentsNumber, ...) {
	char *string;
	va_list arguments;
	va_start(arguments, argumentsNumber);

	string = malloc(1);

	if (!string) {
		panic("failed to malloc string");
	}

	for (size_t index = 0; index < argumentsNumber; index++) {
		char *appendString = va_arg(arguments, char *);

		string = realloc(string, sizeof(string) + strlen(appendString));

		if (!string) {
			panic("failed to realloc string");
		}

		strcat(string, appendString);
	}

	va_end(arguments);

	return string;
}

/**
 * Repeats the char the specified amount of times.
 *
 * @param chr    The char to repeat.
 * @param length The amount of times to repeat the char.
 *
 * @return char* - The repeated string.
 */
char *repeatChr(char chr, size_t length) {
	char *string = malloc(length + 1);

	if (!string) {
		panic("failed to malloc string");
	}

	for (size_t index = 0; index < length; index++) {
		string[index] = chr;
	}

	string[length] = '\0';

	return string;
}

/**
 * Convers the char into a string.
 *
 * @param chr The char to convert into a string.
 *
 * @return char* - The repeated string.
 */
char *chrToString(char chr) {
	char *string = malloc(2);

	if (!string) {
		panic("failed to malloc string");
	}

	string[0] = chr;
	string[1] = '\0';

	return string;
}
