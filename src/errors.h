/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

/**
 * Used to identify errors.
 */
enum ErrorIdentifiers {
	// Argument parser
	A0001,
	A0002,
	A0003,
	A0004,

	// Lexer
	L0001,
	L0002,
	L0003,
	L0004,
	L0005,
	L0006,
	L0007,

	// Parser
	P0001,
	P0002,
	P0003,
};

/**
 * Contains the names of each of the error identifiers.
 */
extern const struct Array g_ERRORIDENTIFIER_NAMES;

/**
 * Gets the name of an error.
 *
 * @param IDENTIFIER The error's identifier.
 *
 * @return The name of the error.
 */
const char* error_get(
	const enum ErrorIdentifiers IDENTIFIER); // NOLINT(readability-avoid-const-params-in-decls)
