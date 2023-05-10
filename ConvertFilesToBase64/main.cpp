#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/algorithm/string.hpp>

//#include<Utility/Generic/SimpleFile.h>
#include<future>
#include<iostream>
#include<fstream>
#include<filesystem>
//#include<Utility/Color/Color.h>

#include<boost/json.hpp>

std::string decode64(const std::string& val) {
    using namespace boost::archive::iterators;
    using It = transform_width<binary_from_base64<std::string::const_iterator>, 8, 6>;
    return boost::algorithm::trim_right_copy_if(std::string(It(std::begin(val)), It(std::end(val))), [](char c) {
        return c == '\0';
        });
}

std::string encode64(const std::string& val) {
    using namespace boost::archive::iterators;
    using It = base64_from_binary<transform_width<std::string::const_iterator, 6, 8>>;
    auto tmp = std::string(It(std::begin(val)), It(std::end(val)));
    return tmp.append((3 - val.size() % 3) % 3, '=');
}

int main() {
    auto inputFuture = std::async([]
        {
            return std::string{ "terrain" };
        }
    );

    auto rootDirectory = std::filesystem::path("G:/AWCustom/temp");
    std::vector<std::filesystem::path> searchPaths;
    {
        searchPaths.push_back(rootDirectory / "images");
        //searchPaths
    }
    auto output = rootDirectory / "out.json";
    std::vector<std::filesystem::path> paths;
    for (auto directory : searchPaths) {
        for (auto file : std::filesystem::directory_iterator(directory)) {
            if (
                file.is_regular_file()
                && file != output
                ) {
                paths.push_back(file.path());
            }
        }
    }
    auto type = inputFuture.get();
    bool splitData = false;
    std::stringstream ss;
    namespace json = boost::json;
    json::array arr;
    for (auto const& path : paths) {
        json::object ret;
        ret["key"] = path.stem().string();
        ret["type"] = type;
        ret["largeImage"] = "";
        std::vector<uint8_t> bytes;
        {
            std::ifstream in{ path, std::ios::binary };
            in.seekg(0, std::ios::end);
            auto length = in.tellg();
            bytes.resize(length);
            in.seekg(0, std::ios::beg);
            in.read(reinterpret_cast<char*>(bytes.data()), length);
        }
        std::string out;
        out.resize(bytes.size());
        std::copy(bytes.begin(), bytes.end(), out.begin());
        auto encoded = encode64(out);
        if (splitData) {
            for (int i = 0; i < encoded.size(); i += 512) {
                ret["smallImage" + std::to_string(i / 512)] = encoded.substr(i, 512);
            }
        }
        else {
            ret["smallImage"] = encoded;
        }
        arr.push_back(ret);
    }
    ss << json::serialize(arr);
    std::ofstream out{ output };
    out << ss.str();

    return 0;
}