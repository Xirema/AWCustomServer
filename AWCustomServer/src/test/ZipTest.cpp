#include<ZipFunctions.h>
#include<print>
#include<iostream>
#include<fstream>

int main(int argc, char const ** argv) {
  if(argc < 3) {
    std::print("Invalid use: provide an archive file path for reading and writing.\n");
    return -1;
  }
  std::ifstream file{argv[1], std::ios::binary};
  file.seekg(0, std::ios::end);
  auto length = file.tellg();
  file.seekg(0, std::ios::beg);
  std::vector<uint8_t> data;
  data.resize(length);
  file.read(reinterpret_cast<char*>(data.data()), length);
  auto fileMap = ziputil::readArchive(data);
  for(auto const& [fileName, fileData] : fileMap) {
    std::print("File '{}': {} bytes.\n", fileName, fileData.size());
  }
  auto newArchive = ziputil::writeArchive(fileMap);
  std::ofstream outFile{argv[2], std::ios::binary};
  outFile.write(reinterpret_cast<char*>(newArchive.data()), newArchive.size());
  return 0;
}