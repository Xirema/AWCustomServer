#include<ZipFunctions.h>
#include<zip.h>
#include<stdexcept>
#include<memory>

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

ziputil::FileMap ziputil::openArchive(std::vector<uint8_t> const& archive) {
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
    zip_fseek(file.get(), 0, SEEK_END);
    auto fileLength = zip_ftell(file.get());
    zip_fseek(file.get(), 0, SEEK_SET);
    auto & fileData = ret[fileName];
    fileData.resize(fileLength);
    zip_fread(file.get(), fileData.data(), fileLength);
  }
  return ret;
}