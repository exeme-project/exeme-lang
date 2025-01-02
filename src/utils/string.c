/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
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

	if (!self) {
		panic("failed to malloc String struct");
	}

	if (string[0] == '\0') {
		self->length = 0;

		if (copy) {
			self->_value = malloc(1);
			self->_value[self->length] = '\0';
		} else {
			self->_value = string;
		}
	} else if (copy) {
		self->length = strlen(string);
		self->_value = malloc(self->length + 1);

		if (!self->_value) {
			panic("failed to malloc string while initialising String struct");
		}

		for (size_t index = 0; index < self->length; index++) {
			self->_value[index] = string[index];
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

/**
 * Appends a char to the String.
 *
 * @param self The current String struct.
 * @param chr  The string to append.
 */
void string_appendChr(struct String *self, char chr) {
	string___realloc(self, self->length + 2); // 1 for new char and 1 for null terminator

	self->_value[self->length++] = chr;
	self->_value[self->length] = '\0';
}

void string_appendStr(struct String *self, const char *string) {
	size_t length = strlen(string);

	string___realloc(self, self->length + length + 1); // 1 for null terminator

	for (size_t index = 0; index < length; index++) {
		self->_value[self->length++] = string[index];
	}

	self->_value[self->length] = '\0';
}

/**
 * Removes all the elements from the String.
 *
 * @param self The current String struct.
 */
void string_clear(struct String *self) {
	self->_value[0] = '\0';
	self->length = 0;
	string___realloc(self, 1);
}

/**
 * Frees an String struct.
 *
 * @param self The current String struct.
 */
void string_free(struct String **self) {
	if (self && *self) {
		free((*self)->_value);

		free(*self);
		*self = NULL;
	} else {
		panic("String struct has already been freed");
	}
}

/**
 * Concatenates the specified strings together.
 *
 * @param firstString The first string to concatenate.
 * @param ... The strings to concatenate, terminated by a NULL sentinel.
 *
 * @return The concatenated string.
 */
char *stringConcatenate_(const char *firstString, ...) {
	if (!firstString) {
		return NULL;
	}

	va_list arguments;
	va_start(arguments, firstString);

	char *string = malloc(strlen(firstString) + 1); // 1 for null terminator

	if (!string) {
		panic("failed to malloc string while concatenating");
	}

	strcpy(string, firstString);

	const char *appendString;

	while ((appendString = va_arg(arguments, const char *))) { // Doesn't equal NULL
		string = realloc(string, strlen(string) + strlen(appendString) + 1);

		if (!string) {
			panic("failed to realloc string while concatenating");
		}

		strcat(string, appendString);
	}

	va_end(arguments);

	return string;
}

#define stringConcatenate(...) stringConcatenate_(__VA_ARGS__, NULL)

char *stringDuplicate(const char *string) {
	char *duplicate = malloc(strlen(string) + 1);

	if (!duplicate) {
		panic("failed to malloc string while duplicating");
	} else {
		strcpy(duplicate, string);
	}

	return duplicate;
}

/**
 * Repeats the char the specified amount of times.
 *
 * @param chr    The char to repeat.
 * @param length The amount of times to repeat the char.
 *
 * @return The repeated string.
 */
char *repeatChr(char chr, size_t length) {
	char *string = malloc(length + 1);

	if (!string) {
		panic("failed to malloc string while repeating car");
	}

	for (size_t index = 0; index < length; index++) {
		string[index] = chr;
	}

	string[length] = '\0';

	return string;
}
