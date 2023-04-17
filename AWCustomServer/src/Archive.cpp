#include<Archive.h>
#include<zlib.h>
#include<stdexcept>
#include<iterator>

namespace arch {
	namespace {
		constexpr size_t CHUNK_SIZE = 1ull << 20ull;
	}
	std::vector<uint8_t> inflate(std::vector<uint8_t> data) {
		std::vector<uint8_t> out;
		if (data.size() >= std::numeric_limits<uint32_t>::max()) {
			throw std::runtime_error("We cannot support archive files larger than 4GB!");
		}
		z_stream stream{};
		if (auto ret = inflateInit(&stream); ret != Z_OK) {
			throw std::runtime_error("Problem Unzipping Archive: " + std::to_string(ret));
		}

		do {
			stream.avail_in = static_cast<uint32_t>(data.size());
			if (stream.avail_in == 0) {
				break;
			}
			stream.next_in = data.data();
			do {
				stream.avail_out = static_cast<uint32_t>(CHUNK_SIZE);
				std::vector<uint8_t> temp(CHUNK_SIZE);
				stream.next_out = temp.data();
				if (auto ret = ::inflate(&stream, Z_NO_FLUSH); 
					ret == Z_NEED_DICT
					|| ret == Z_STREAM_ERROR
					|| ret == Z_DATA_ERROR
					|| ret == Z_MEM_ERROR) {
					::inflateEnd(&stream);
					throw std::runtime_error("Problem Unzipping Archive: " + std::to_string(ret));
				}
				auto bytesRead = stream.avail_out;
				std::copy(temp.begin(), temp.begin() + bytesRead, std::back_inserter(out));
				//TODO: Finish this....?
			} while (false);
		} while (false);
		return out;
	}
	std::vector<uint8_t> deflate(std::vector<uint8_t> data) {
		return data;
	}
}