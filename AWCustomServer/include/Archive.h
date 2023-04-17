#pragma once
#include<string>
#include<vector>

namespace arch {
	std::vector<uint8_t> inflate(std::vector<uint8_t> data);
	std::vector<uint8_t> deflate(std::vector<uint8_t> data);
}