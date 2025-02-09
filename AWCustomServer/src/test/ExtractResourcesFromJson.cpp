#include<jss/Resource.h>
#include<SQLUtil.h>
#include<print>
#include<DBFunctions.h>
#include<Base64.h>
#include<fstream>
#include<filesystem>

int main(int argc, char const** argv) {
  if(argc != 3) {
    std::print("Error: Must specify resource pack id and output directory.\n");
    return -1;
  }
  auto resourcePack = db::get_resource_pack(std::strtol(argv[1], nullptr, 10));
  auto obj = json::value_from(resourcePack).as_object();

  auto imageResources = json::value_to<std::vector<rTypes::ImageResource>>(obj.at("imageResources"));
  obj.erase("imageResources");
  {
    std::ofstream textOut{"text.json"};
    textOut << json::serialize(obj);
  }
  std::filesystem::path dir = argv[2];
  for(auto const& imageResource : imageResources) {
    std::filesystem::path outPath = dir;
    outPath = outPath / imageResource.type;
    outPath = outPath / imageResource.key;
    outPath = outPath / imageResource.armyColor.value_or("neutral");
    if(imageResource.variant) {
      outPath = outPath / *imageResource.variant;
    }
    std::filesystem::create_directories(outPath);
    outPath = outPath / std::to_string(imageResource.orientation.value_or(0));
    if(imageResource.smallImage.size() > 300) {
      outPath += ".gif";
    } else {
      outPath += ".png";
    }
    auto data = base64::decode(imageResource.smallImage);
    std::ofstream outFile{outPath, std::ios::binary};
    outFile.write(reinterpret_cast<char*>(data.data()), data.size());
  }
}