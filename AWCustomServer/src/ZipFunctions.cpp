#include<ZipFunctions.h>
#include<zip.h>
#include<stdexcept>
#include<memory>
#include<print>

namespace {
  struct zip_cleanup {
    void operator()(zip_file_t * ptr) const {
      zip_fclose(ptr);
    }
    void operator()(zip_t * ptr) const {
      zip_close(ptr);
    }
    void operator()(zip_source_t * ptr) const {
      zip_source_close(ptr);
    }
  };
}

ziputil::FileMap ziputil::readArchive(std::vector<uint8_t> const& archive) {
  FileMap ret;
  zip_error_t err;
  zip_error_init(&err);
  std::unique_ptr<zip_source_t, zip_cleanup> source{zip_source_buffer_create(archive.data(), archive.size(), 0, &err)};
  if(err.zip_err) {
    throw std::runtime_error("Unable to create source buffer: " + std::string{err.str});
  }

  std::unique_ptr<zip_t, zip_cleanup> zipFile{zip_open_from_source(source.get(), ZIP_RDONLY, &err)};
  if(err.zip_err) {
    throw std::runtime_error("Unable to open archive file: " + std::string{err.str});
  }
  auto numFiles = zip_get_num_entries(zipFile.get(), 0);
  for(int64_t index = 0; index < numFiles; index++) {
    std::string fileName = zip_get_name(zipFile.get(), index, 0);
    if(fileName.back() == '/') {
      continue;
    }
    std::unique_ptr<zip_file_t, zip_cleanup> file{zip_fopen_index(zipFile.get(), index, 0)};
    if(!file) {
      auto error = zip_get_error(zipFile.get());
      throw std::runtime_error(std::format("ERROR opening file: {}", error->str));
    }
    auto & fileData = ret[fileName];
    std::vector<uint8_t> buffer;
    constexpr size_t BUFFER_SIZE = 65'536;
    buffer.resize(BUFFER_SIZE);
    size_t bytesRead = 0;
    while((bytesRead = zip_fread(file.get(), buffer.data(), BUFFER_SIZE)) > 0) {
      fileData.insert(fileData.end(), buffer.begin(), buffer.begin() + bytesRead);
    }
  }
  return ret;
}

std::vector<uint8_t> ziputil::writeArchive(FileMap const& fileMap) {
  zip_error_t err;
  zip_error_init(&err);
  std::unique_ptr<zip_source_t, zip_cleanup> source{zip_source_buffer_create(nullptr, 0, 0, &err)};
  if(err.zip_err) {
    throw std::runtime_error("Unable to create source buffer: " + std::string{err.str});
  }
  zip_source_keep(source.get());
  {
    std::unique_ptr<zip_t, zip_cleanup> zipFile{zip_open_from_source(source.get(), ZIP_TRUNCATE, &err)};
    if(err.zip_err) {
      throw std::runtime_error("Unable to open Zip File from Source" + std::string{err.str});
    }
    for(auto const& [fileName, fileData] : fileMap) {
      std::unique_ptr<zip_source_t, zip_cleanup> fileSource{zip_source_buffer(zipFile.get(), fileData.data(), fileData.size(), 0)};
      zip_file_add(zipFile.get(), fileName.c_str(), fileSource.get(), ZIP_FL_ENC_UTF_8);
    }
  }
  std::vector<uint8_t> retBuffer;
  if(zip_source_open(source.get())) {
    auto reopenError = zip_source_error(source.get());
    throw std::runtime_error(std::format("Unable to reopen source for reading:'{}'", reopenError->zip_err));
  }
  zip_source_seek(source.get(), 0, SEEK_END);
  auto sourceSize = zip_source_tell(source.get());
  zip_source_seek(source.get(), 0, SEEK_SET);
  retBuffer.resize(static_cast<size_t>(sourceSize));
  zip_source_read(source.get(), retBuffer.data(), sourceSize);
  return retBuffer;
}