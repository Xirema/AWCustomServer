#pragma once
#include<map>
#include<string>
#include<vector>
#include<cstdint>

namespace ziputil {
  using FileMap = std::map<std::string, std::vector<uint8_t>>;

  FileMap openArchive(std::vector<uint8_t> const& archive);
}