#include<ZipFunctions.h>
#include<print>
#include<iostream>
#include<fstream>

int main(int argc, char const ** argv) {
  if(argc < 2) {
    std::print("Invalid use: provide an archive file path.\n");
    return -1;
  }
  std::ifstream file{argv[1], std::ios::binary};
  file.seekg(0, std::ios::end);
  auto length = file.tellg();
  file.seekg(0, std::ios::beg);
  std::vector<uint8_t> data;
  data.resize(length);
  file.read(reinterpret_cast<char*>(data.data()), length);
  auto fileMap = ziputil::openArchive(data);
  for(auto const& [fileName, fileData] : fileMap) {
    std::print("File '{}': {} bytes.\n", fileName, fileData.size());
  }
  return 0;
}