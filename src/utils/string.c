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
struct String *string_new(char *value) {
	struct String *self = malloc(STRING_STRUCT_SIZE);

	if (!self) {
		panic("failed to create String struct");
	}

	self->length = 0;
	self->_value = malloc(sizeof(value));

	if (!self->_value) {
		panic("failed to malloc string");
	}

	strcpy(self->_value, value);

	return self;
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
	va_list argnuments;
	va_start(argnuments, argumentsNumber);

	string = malloc(1);

	if (!string) {
		panic("failed to malloc string");
	}

	for (size_t index = 0; index < argumentsNumber; index++) {
		char *appendString = va_arg(argnuments, char *);

		string = realloc(string, sizeof(string) + strlen(appendString));
		strcat(string, appendString);
	}

	va_end(argnuments);

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
	size_t index;
	char *string = malloc(length + 1);

	if (!string) {
		panic("failed to malloc string");
	}

	for (index = 0; index < length; index++) {
		string[index] = chr;
	}

	string[index] = '\0';

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
