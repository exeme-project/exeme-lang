#pragma once

#include <stdint.h>
#include <string_view>

constexpr uint32_t hash(const std::string_view data) noexcept {
	uint32_t hash = 5385;

	for (const auto &e : data) {
		hash = ((hash << 5) + hash) + e;
	}

	return hash;
}
