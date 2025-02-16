/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "array.h"
#include <stdbool.h>
#include <stdlib.h>

/**
 * Represents a string.
 */
struct String {
	char*  _value;
	size_t length;
};

#define STRING_STRUCT_SIZE sizeof(struct String)

/**
 * Creates a new String struct.
 *
 * @param p_string The initial string value.
 * @param copy Whether to copy the string or use the provided pointer.
 *
 * @return The created String struct.
 */
struct String* string_new(char* p_string, bool copy);

/**
 * Appends a char to the String.
 *
 * @param p_self The current String struct.
 * @param chr The char to append.
 */
void string_append_chr(struct String* p_self, char chr);

/**
 * Appends a string to the String.
 *
 * @param p_self The current String struct.
 * @param p_string The string to append.
 */
void string_append_str(struct String* p_self, const char* p_string);

/**
 * Removes all the elements from the String.
 *
 * @param p_self The current String struct.
 */
void string_clear(struct String* p_self);

/**
 * Frees a String struct.
 *
 * @param p_self The current String struct.
 */
void string_free(struct String** p_self);

/**
 * Concatenates the specified strings together.
 *
 * @param array The strings to concatenate.
 *
 * @return The concatenated string.
 */
char* concatenate_string_internal(struct Array array);

#define CONCATENATE_STRING(...) concatenate_string_internal(ARRAY_NEW_STACK(__VA_ARGS__))

/**
 * Duplicates a string.
 *
 * @param p_string The string to duplicate.
 * @return The duplicated string.
 */
char* duplicate_string(const char* p_string);

/**
 * Repeats the char the specified amount of times.
 *
 * @param CHR The char to repeat.
 * @param length The amount of times to repeat the char.
 *
 * @return The repeated string.
 */
char* repeat_chr(const char CHR, size_t length); // NOLINT(readability-avoid-const-params-in-decls)

/**
 * Safely copies a string.
 *
 * @param p_dest The destination string.
 * @param p_src The source string.
 */
void strcpy_safe(char* p_dest, const char* p_src);

/**
 * Gets the length of a string, handling null pointers and max length.
 *
 * @param p_string The string to get the length of.
 *
 * @return The length of the string.
 */
size_t strlen_safe(const char* p_string);
