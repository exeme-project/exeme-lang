/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "errors.h"
#include "./utils/array.h"
#include "./utils/panic.h"

const struct Array g_ERRORIDENTIFIER_NAMES =
	ARRAY_NEW_STACK("A0001", "A0002", "A0003", "A0004", "L0001", "L0002", "L0003", "L0004", "L0005",
					"L0006", "L0007", "P0001", "P0002", "P0003");

const char* error_get(const enum ErrorIdentifiers IDENTIFIER) {
	if ((size_t)IDENTIFIER + 1 > g_ERRORIDENTIFIER_NAMES.length) {
		PANIC("g_ErrorIdentifiers get index out of bounds");
	}

	return g_ERRORIDENTIFIER_NAMES._values[IDENTIFIER];
}
