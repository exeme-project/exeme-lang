/**
 * Part of the Exeme Language Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "../includes.c"

#include "./array.c"

/**
 * Used to identify errors.
 */
enum ErrorIdentifiers {
	E0001,
	E0002,
	E0003,
	E0004,
	E0005,
	E0006,
	E0007,
};

/**
 * Contains the names of each of the error identifiers.
 */
const struct Array ERRORIDENTIFIER_NAMES = {
	7,
	(const void *[]){
		"E0001",
		"E0002",
		"E0003",
		"E0004",
		"E0005",
		"E0006",
		"E0007",
	},
};
