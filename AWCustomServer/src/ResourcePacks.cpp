#include <RestFunctions.h>
#include<DBFunctions.h>

std::string rest::resource::get_pack_metadata(net::HTTPHeaders const &headers)
{
  std::optional<std::string_view> name, version;
  std::optional<int64_t> packId;
  auto const& headerMap = headers.httpHeaders;
  if(auto it = headerMap.find("name"); it != headerMap.end()) {
    name = it->second;
  }
  if(auto it = headerMap.find("version"); it != headerMap.end()) {
    version = it->second;
  }
  if(auto it = headerMap.find("packid"); it != headerMap.end()) {
    auto& packIdRef = packId.emplace();
    auto [ptr, ec] = std::from_chars(&*it->second.begin(), &*it->second.end(), packIdRef);
    if(ec != std::errc{}) {
      throw net::RestError(std::format("packid header not a number: '{}'", it->second), net::RestError::Type::BAD_REQUEST);
    }
  }
  rTypes::PackMetadata ret = db::get_pack_metadata(name, version, packId);
  return serialize(json::value_from(ret));
}

std::string rest::resource::get_resource_pack(net::HTTPHeaders const &headers)
{
  int64_t packId;
  if (auto it = headers.httpHeaders.find("packid"); it != headers.httpHeaders.end())
  {
    packId = std::stoll(it->second);
  }
  else
  {
    throw net::RestError("Pack Id not specified", net::RestError::Type::NOT_FOUND);
  }
  auto ret = db::get_resource_pack(packId);
  return serialize(json::value_from(ret));
}

// std::string rest::resource::get_resource_pack2(net::HTTPHeaders const &headers)
// {
//   int64_t packId;
//   if (auto it = headers.httpHeaders.find("packid"); it != headers.httpHeaders.end())
//   {
//     packId = std::stoll(it->second);
//   }
//   else
//   {
//     throw net::RestError("Pack Id not specified", net::RestError::Type::NOT_FOUND);
//   }
//   auto ret = db::get_resource_pack2(packId);
//   return serialize(json::value_from(ret));
// }

std::string rest::resource::upload_pack(net::HTTPHeaders const &headers, std::string body)
{
  try
  {
    boost::system::error_code ec;
    std::println("Parsing body");
    auto value = json::parse(body, ec);
    if(!ec) {
      auto pack = json::value_to<rTypes::ResourcePack>(value);
      auto ret = db::upload_pack(pack);
      if(ret) {
        return "Success";
      }
    } else {
      std::println("Body Not parsed as json, trying zip archive instead: {} bytes", body.size());
      std::vector<uint8_t> data;
      data.resize(body.size());
      std::ranges::transform(
        body, 
        data.begin(), 
        [](char c){ return static_cast<uint8_t>(c); }
      );
      for(int i = 0; i < 16; i++) {
        for(int j = 0; j < 16; j++) {
          size_t index = j + i * 16;
          if(index < data.size()) {
            std::print("{:02x} ", data.at(index));
          } else {
            std::print("-- ");
          }
        }
        std::println("");
      }
      auto ret = db::upload_pack2(data);
      if(ret) {
        return "Success";
      }
    }
    return "Failure";
  }
  catch (std::runtime_error const &e)
  {
    throw net::RestError("There was a problem parsing the Resource Pack: " + std::string(e.what()), net::RestError::Type::INVALID_DATA);
  }
}

// std::string rest::resource::upload_pack2(net::HTTPHeaders const &headers, std::string body)
// {
//   try
//   {
//     std::vector<uint8_t> data;
//     data.resize(body.size());
//     std::ranges::transform(
//       body, 
//       data.begin(), 
//       [](char c){ return static_cast<uint8_t>(c); }
//     );
//     auto ret = db::upload_pack2(data);
//     if(ret) {
//       return "Success";
//     }
//     return "Failure";
//   }
//   catch (std::runtime_error const &e)
//   {
//     throw net::RestError("There was a problem parsing the Resource Pack: " + std::string(e.what()), net::RestError::Type::INVALID_DATA);
//   }
// }

std::string rest::resource::list_packs(net::HTTPHeaders const &headers)
{
  bool includeOldPacks = false;
  if(auto incString = headers.getHeader("include-old-packs"); incString == "true") {
    includeOldPacks = true;
  }
  auto ret = db::list_packs(includeOldPacks);
  return serialize(json::value_from(ret));
}
