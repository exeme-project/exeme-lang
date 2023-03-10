// Part of the Exeme Language Project, under the MIT license. See '/LICENSE' for
// license information. SPDX-License-Identifier: MIT License.

#pragma once

#include <stdint.h>
#include <string_view>

/**
 * Calculates a hash for a string.
 *
 * @param data - The string to calculate the hash for.
 *
 * @return uint32_t - The calculated hash.
 */
constexpr uint32_t hash(const std::basic_string_view<char> data) noexcept {
	uint32_t hash = 5385;

	for (const auto &e : data) {
		hash = ((hash << 5) + hash) + static_cast<uint32_t>(e);
	}

	return hash;
}
