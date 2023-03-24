// Part of the Exeme Language Project, under the MIT license. See '/LICENSE' for
// license information. SPDX-License-Identifier: MIT License.

#pragma once

#include "../includes.c"

/**
 * Concatenates the specified amount of strings together.
 *
 * @return char* - The concatenated string.
 */
char *stringConcatenate(size_t argnumentsNumber, ...) {
	char *string;
	va_list argnuments;
	va_start(argnuments, argnumentsNumber);

	string = malloc(1);

	for (size_t index = 0; index < argnumentsNumber; index++) {
		char *appendString = va_arg(argnuments, char *);

		string = realloc(string, sizeof(string) + strlen(appendString));
		stringConcatenate(string, appendString);
	}

	va_end(argnuments);

	return string;
}

char *repeatChr(char chr, size_t length) {
	size_t index;
	char *string = malloc(length + 1);

	for (index = 0; index < length; index++) {
		string[index] = chr;
	}

	string[index] = '\0';

	return string;
}
