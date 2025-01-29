#pragma once
#include <jss/jssinclude.h>

namespace rTypes
{
    struct PackMetadata
    {
        std::string name;
        std::string version;
        std::optional<std::string> created;
        std::optional<std::string> packId;

        void readFrom(json::object const &obj);
        void writeTo(json::object &obj) const;
    };
    struct TextResource
    {
        std::string key;
        std::string type;
        std::string shortName;
        std::string longName;
        std::string description;
        std::optional<std::string> language;

        void readFrom(json::object const &obj);
        void writeTo(json::object &obj) const;
    };
    struct ImageResource
    {
        std::string key;
        std::string type;
        std::string smallImage;
        std::string largeImage;
        std::optional<std::string> armyColor;
        std::optional<int64_t> orientation;
        std::optional<std::string> variant;

        void readFrom(json::object const &obj);
        void writeTo(json::object &obj) const;
    };
    struct ResourcePack
    {
        PackMetadata packMetadata;
        std::vector<TextResource> textResources;
        std::vector<ImageResource> imageResources;

        void readFrom(json::object const &obj);
        void writeTo(json::object &obj) const;
    };
}
