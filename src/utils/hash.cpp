// Part of the Exeme Language Project, under the MIT license. See '/LICENSE' for
// license information. SPDX-License-Identifier: MIT License.

#pragma once

#include <stdint.h>
#include <string_view>

/**
 * Calculates a hash for a string.
 *
 * @param const std::string_view - The string to calculate the hash for.
 *
 * @return uint32_t - The calculated hash.
 */
constexpr uint32_t hash(const std::string_view data) noexcept {
	uint32_t hash = 5385;

	for (const auto &e : data) {
		hash = ((hash << 5) + hash) + e;
	}

	return hash;
}
