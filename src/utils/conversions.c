/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#include "../globals.h"
#include "./panic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* chr_to_string(char chr) {
	char* lp_string = malloc(2);

	if (!lp_string) {
		PANIC("failed to malloc string");
	}

	lp_string[0] = chr;
	lp_string[1] = '\0'; // Null terminator

	return lp_string;
}

char* ul_to_string(size_t num) {
	size_t length = (size_t)snprintf(NULL, 0, "%zu", num);

	char* lp_str = malloc(length + 1); // + 1 for null terminator

	if (!lp_str) {
		PANIC("failed to malloc string");
	}

	int ret = snprintf(lp_str, length + 1, "%zu", num);

	if (ret < 0) {
		PANIC("failed to convert unsigned long to string - encoding error");
	} else if ((size_t)ret != length) {
		PANIC("failed to convert unsigned long to string - mismatched length (truncation)");
	}

	return lp_str;
}

void* convert_to_type(char* p_data, enum VariableType type) {
	switch (type) {
	case VARIABLE_TYPE_NONE:
		return NULL;
	case VARIABLE_TYPE_STRING:
		return p_data;
	case VARIABLE_TYPE_INT: {
		long* lp_result = malloc(LONG_SIZE);

		if (!lp_result) {
			PANIC("failed to malloc long while converting to int");
		}

		char* lp_endptr = NULL;
		*lp_result		= strtol(p_data, &lp_endptr, DENARY_BASE);

		if (lp_endptr == p_data || *lp_endptr != '\0') {
			free(lp_result);

			return NULL;
		}

		return lp_result;
	}
	case VARIABLE_TYPE_FLOAT: {
		float* lp_result = malloc(FLOAT_SIZE);

		if (!lp_result) {
			PANIC("failed to malloc float while converting to float");
		}

		char* lp_endptr = NULL;
		*lp_result		= strtof(p_data, &lp_endptr);

		if (lp_endptr == p_data || *lp_endptr != '\0') {
			free(lp_result);

			return NULL;
		}

		return lp_result;
	}
	case VARIABLE_TYPE_BOOL:
		return strcmp(p_data, "true") == 0 ? (void*)1 : (void*)0;
	default:
		PANIC("invalid variable type for conversion");
	}
}
