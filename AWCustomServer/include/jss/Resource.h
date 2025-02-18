#pragma once
#include <jss/jssinclude.h>

namespace rTypes
{
    struct PackMetadata
    {
        std::string name;
        std::string version;
        std::optional<std::string> created;
        std::optional<std::string> expired;
        std::optional<int64_t> packId;
        std::optional<int64_t> protocol;
    };
    BOOST_DESCRIBE_STRUCT(PackMetadata, (), (
        name,
        version,
        created,
        expired,
        packId,
        protocol
    ))
    struct TextResource
    {
        std::string key;
        std::string type;
        std::string shortName;
        std::string longName;
        std::string description;
        std::optional<std::string> language;
    };
    BOOST_DESCRIBE_STRUCT(TextResource, (), (
        key,
        type,
        shortName,
        longName,
        description,
        language
    ))
    struct ImageResource
    {
        std::string key;
        std::string type;
        std::string smallImage;
        std::string largeImage;
        std::optional<std::string> armyColor;
        std::optional<int64_t> orientation;
        std::optional<std::string> variant;
    };
    BOOST_DESCRIBE_STRUCT(ImageResource, (), (
        key,
        type,
        smallImage,
        largeImage,
        armyColor,
        orientation,
        variant
    ))
    struct ResourcePack
    {
        PackMetadata packMetadata;
        std::vector<TextResource> textResources;
        std::vector<ImageResource> imageResources;
    };
    BOOST_DESCRIBE_STRUCT(ResourcePack, (), (
        packMetadata,
        textResources,
        imageResources
    ))
}
