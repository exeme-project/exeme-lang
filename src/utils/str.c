/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#include "./str.h"
#include "../globals.h"
#include "./panic.h"
#include <string.h>

struct String* string_new(char* p_string, bool copy) {
	struct String* lp_self = malloc(STRING_STRUCT_SIZE);

	if (!lp_self) {
		PANIC("failed to malloc String struct");
	}

	if (p_string[0] == '\0') {
		lp_self->length = 0;

		if (copy) {
			lp_self->_value					 = malloc(1);
			lp_self->_value[lp_self->length] = '\0';
		} else {
			lp_self->_value = p_string;
		}
	} else if (copy) {
		lp_self->length = strlen_safe(p_string);
		lp_self->_value = malloc(lp_self->length + 1);

		if (!lp_self->_value) {
			PANIC("failed to malloc string while initialising String struct");
		}

		for (size_t index = 0; index < lp_self->length; index++) {
			lp_self->_value[index] = p_string[index];
		}

		lp_self->_value[lp_self->length] = '\0';
	} else {
		lp_self->_value = p_string;
		lp_self->length = strlen_safe(lp_self->_value);
	}

	return lp_self;
}

/**
 * Reallocates the struct's string.
 *
 * @param p_self The current String struct.
 * @param size The new size of the string.
 */
void string___realloc(struct String* p_self, size_t size) {
	void* lp_reallocTemp = realloc(p_self->_value, size);

	if (!lp_reallocTemp) {
		PANIC("failed to realloc string");
	}

	p_self->_value = lp_reallocTemp;
}

void string_append_chr(struct String* p_self, char chr) {
	string___realloc(p_self, p_self->length + 2); // 1 for new char and 1 for null terminator

	p_self->_value[p_self->length++] = chr;
	p_self->_value[p_self->length]	 = '\0';
}

void string_append_str(struct String* p_self, const char* p_string) {
	size_t length = strlen_safe(p_string);

	string___realloc(p_self, p_self->length + length + 1); // 1 for null terminator

	for (size_t index = 0; index < length; index++) {
		p_self->_value[p_self->length++] = p_string[index];
	}

	p_self->_value[p_self->length] = '\0';
}

void string_clear(struct String* p_self) {
	p_self->_value[0] = '\0';
	p_self->length	  = 0;
	string___realloc(p_self, 1);
}

void string_free(struct String** p_self) {
	if (p_self && *p_self) {
		free((*p_self)->_value);

		free(*p_self);
		*p_self = NULL;
	} else {
		PANIC("String struct has already been freed");
	}
}

char* concatenate_string_internal(struct Array array) {
	if (array.length == 0 || !array._values[0]) {
		return NULL;
	}

	size_t totalLength = 0;

	for (size_t i = 0; i < array.length; i++) {
		if (array._values[i]) {
			totalLength += strlen_safe((const char*)array._values[i]);
		}
	}

	char* lp_string = malloc(totalLength + 1); // 1 for null terminator

	if (!lp_string) {
		PANIC("failed to concatenate string: string malloc failed");
	}

	lp_string[0] = '\0'; // Initialize the string

	size_t offset = 0;

	for (size_t stringIndex = 0; stringIndex < array.length; stringIndex++) {
		if (array._values[stringIndex]) {
			for (size_t charIndex = 0;
				 charIndex < strlen_safe((const char*)array._values[stringIndex]); charIndex++) {
				lp_string[offset++] = ((const char*)array._values[stringIndex])[charIndex];
			}
		}
	}

	return lp_string;
}

char* duplicate_string(const char* p_string) {
	char* lp_duplicate = malloc(strlen_safe(p_string) + 1);

	if (!lp_duplicate) {
		PANIC("failed to duplicate string: string malloc failed");
	} else {
		strcpy_safe(lp_duplicate, p_string);
	}

	return lp_duplicate;
}

char* repeat_chr(const char CHR, size_t length) {
	char* lp_string = malloc(length + 1);

	if (!lp_string) {
		PANIC("failed to repeat chr: string malloc failed");
	}

	for (size_t index = 0; index < length; index++) {
		lp_string[index] = CHR;
	}

	lp_string[length] = '\0';

	return lp_string;
}

void strcpy_safe(char* p_dest, const char* p_src) {
	if (!p_dest || !p_src) {
		PANIC("failed to safely strcpy: null pointer passed");
	} else if (strlen_safe(p_src) != strlen_safe(p_dest)) {
		PANIC("failed to safely strcpy: source and destination strings are not the same length");
	}

	size_t index = 0;

	while (p_src[index] != '\0') {
		p_dest[index] = p_src[index];

		index++;
	}

	p_dest[index] = '\0';
}

size_t strlen_safe(const char* p_string) {
	if (!p_string) {
		// Handle null pointer case, return 0 or an error code
		return 0;
	}

	size_t length = 0;

	while (length < MAX_STRING_LENGTH && p_string[length] != '\0') {
		length++;
	}

	// If we hit the max length, we can assume the string is potentially not null-terminated
	if (length == MAX_STRING_LENGTH) {
		PANIC("failed to safely strlen: string is too long to obtain length");
	}

	return length;
}
