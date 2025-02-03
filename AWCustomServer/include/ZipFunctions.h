#pragma once
#include<map>
#include<string>
#include<vector>
#include<cstdint>

namespace ziputil {
  using FileMap = std::map<std::string, std::vector<uint8_t>>;

  FileMap readArchive(std::vector<uint8_t> const& archive);
  std::vector<uint8_t> writeArchive(FileMap const& fileMap);
}