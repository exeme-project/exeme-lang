/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "../includes.c"

#include "./panic.c"

char *ulToString(size_t num) {
	size_t length = (size_t)snprintf(NULL, 0, "%zu", num);

	char *str = malloc(length + 1);

	if (!str) {
		panic("failed to malloc string");
	}

	snprintf(str, length + 1, "%zu", num);

	return str;
}
