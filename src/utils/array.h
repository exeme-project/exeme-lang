/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include <stdbool.h>
#include <stdlib.h>

/**
 * Represents an array.
 */
struct Array {
	size_t		 length;
	const void** _values;
};

#define ARRAY_STRUCT_SIZE		  sizeof(struct Array)
#define ARRAY_STRUCT_ELEMENT_SIZE sizeof(const void*)
#define ARRAY_NEW_STACK(...)                                                                       \
	((struct Array){.length	 = sizeof((const void*[]){__VA_ARGS__}) / ARRAY_STRUCT_ELEMENT_SIZE,   \
					._values = (const void*[]){__VA_ARGS__}}) // Variadic args to the rescue!
#define ARRAY_UPGRADE_STACK(array, _length)                                                        \
	((struct Array){.length = (_length),                                                           \
					._values =                                                                     \
						(array)}) // Changes a const void *[] to a struct Array on the stack

/**
 * Creates a new Array struct.
 *
 * @return The created Array struct.
 */
struct Array* array_new(void);

/**
 * Reallocates the struct's array.
 *
 * @param p_self       The current Array struct.
 * @param newLength The new length of the array.
 */
void array___realloc(struct Array* p_self, size_t newLength);

/**
 * Inserts a value at the specified index. Will expand the array's size if
 * needed.
 *
 * @param p_self  The current Array struct.
 * @param index The index at which to insert the value.
 * @param p_value The value to insert.
 */
void array_insert(struct Array* p_self, size_t index, const void* p_value);

/**
 * Appends a value to the end of the array.
 *
 * @param p_self  The current Array struct.
 * @param p_value The value to append.
 */
void array_append(struct Array* p_self, const void* p_value);

/**
 * Removes the last element from the array.
 *
 * @param p_self The current Array struct.
 */
void array_pop(struct Array* p_self);

/**
 * Removes all the elements from the array.
 *
 * @param p_self         The current Array struct.
 * @param p_freeElement The function to free the elements with.
 */
void array_clear(struct Array* p_self, void (*p_freeElement)(const void*));

/**
 * Returns the element at the specified index in the array.
 *
 * @param p_self  The current Array struct.
 * @param index The index from which to get the element.
 *
 * @return The retrieved element.
 */
const void* array_get(struct Array* p_self, size_t index);

/**
 * Used to check for string matches in the array.
 *
 * @param p_element The element to check.
 * @param p_match   The value to match against.
 *
 * @return If the element matches the match.
 */
bool array___match_string(const void* p_element, const void* p_match);

/**
 * Iterates through the array, using the passed function to check for matches. If a match is found
 * then the index is returned.
 *
 * @param p_self    The current Array struct.
 * @param p_matcher The function to check for matches.
 * @param p_match   The value to match against.
 *
 * @return The index of the match.
 */
int array_find(struct Array* p_self, bool (*p_matcher)(const void*, const void*), void* p_match);

/**
 * Checks if the array contains a value, internally using the array_find function.
 *
 * @param p_self    The current Array struct.
 * @param p_matcher The function to check for matches.
 * @param p_match   The value to match against.
 *
 * @return If a match was found.
 */
bool array_contains(struct Array* p_self, bool (*p_matcher)(const void*, const void*),
					void*		  p_match);

/**
 * Checks if the array contains a value at the specified index.
 *
 * @param p_self  The current Array struct.
 * @param index The index to check.
 *
 * @return If the index is occupied.
 */
bool array_index_occupied(struct Array* p_self, size_t index);

/**
 * Joins the elements of the array into a single string, separated by the specified separator.
 *
 * @param p_self      The current Array struct.
 * @param p_separator The separator to use between elements.
 * @param p_stringify The function to convert elements to strings.
 *
 * @return The joined string.
 */
char* array_join(struct Array* p_self, const char* p_separator, char* (*p_stringify)(const void*));

/**
 * Frees an Array struct.
 *
 * @param p_self The current Array struct.
 */
void array_free(struct Array** p_self);
