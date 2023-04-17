#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/algorithm/string.hpp>

#include<Utility/Generic/SimpleFile.h>
#include<future>
#include<iostream>
#include<fstream>
#include<Utility/Color/Color.h>

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
    auto openFuture = std::async([]
        {
            return util::show_file_dialog(
                util::dialog_type::OPEN,
                "Files to Convert",
                util::dialog_options::FILES | util::dialog_options::MULTI_SELECT
            );
        }
    );
    auto saveFuture = std::async([]
        {
            return util::show_file_dialog(
                util::dialog_type::SAVE,
                "File to save Strings"
            );
        }
    );

    auto inputFuture = std::async([]
        {
            return std::string{ "interface" };
        }
    );

    struct color_adjust {
        float hue;
        float saturation;
        float brightness;
    };
    std::map<std::string, color_adjust> armyColors {
        { "os", color_adjust{.hue = 30. / 360, .saturation = 1, .brightness = 1 } }, //Orange Star
        { "bm", color_adjust{.hue = 180. / 360, .saturation = 1, .brightness = 1 } }, //Blue Moon
        { "ge", color_adjust{.hue = 120. / 360, .saturation = 1, .brightness = 1 } }, //Green Earth
        { "yc", color_adjust{.hue = 60. / 360, .saturation = 1, .brightness = 1 } }, //Yellow Comet
        { "bh", color_adjust{.hue = 270. / 360, .saturation = .33, .brightness = .4 } }, //Black Hole
        { "rf", color_adjust{.hue = 345. / 360, .saturation = 1, .brightness = .75 } }, //Red Fire
        { "gs", color_adjust{.hue = 0. / 360, .saturation = 0, .brightness = .5 } }, //Grey Sky
        { "bd", color_adjust{.hue = 20. / 360, .saturation = .85, .brightness = .6 } }, //Brown Desert
        { "ab", color_adjust{.hue = 40. / 360, .saturation = 1, .brightness = 1 } }, //Amber Blaze
        { "js", color_adjust{.hue = 90. / 360, .saturation = .2, .brightness = .75 } }, //Jade Sun
        { "ci", color_adjust{.hue = 225. / 360, .saturation = .9, .brightness = .45 } }, //Cobalt Ice
        { "pc", color_adjust{.hue = 320. / 360, .saturation = .6, .brightness = 1 } }, //Pink Cosmos
        { "tg", color_adjust{.hue = 175. / 360, .saturation = .7, .brightness = .8 } }, //Teal Galaxy
        { "pl", color_adjust{.hue = 280. / 360, .saturation = .85, .brightness = .6 } }, //Purple Lightning
        { "ar", color_adjust{.hue = 75. / 360, .saturation = .9, .brightness = .5 } }, //Acid Rain
        { "wn", color_adjust{.hue = 0. / 360, .saturation = .25, .brightness = .8 } }  //White Nova
    };
    auto paths = openFuture.get();
    auto type = inputFuture.get();
    std::stringstream ss;
    namespace json = boost::json;
    json::array arr;
    for (auto const& path : paths) {
        json::object ret;
        ret["key"] = path.stem().string();
        ret["type"] = type;
        ret["largeImage"] = "";
        auto bytes = util::read_binary_file(path);
        std::string out;
        out.resize(bytes.size());
        std::copy(bytes.begin(), bytes.end(), out.begin());
        //std::ifstream in{ path, std::ios::binary };
        //auto [metadata, image_v] = util::read_image(in);
        //if (auto image = std::get_if<util::Matrix<util::color_uint8>>(&image_v)) {
            //auto &image = std::get<util::layered_image>(image_v);
            //util::image_metadata_header paletteHeader{ .type = util::image_metadata_header::Type::ANIMATION, .key = "GlobalPalette" };
            //auto& lmetadata = image.layers.at(0).first;
            //auto& value = lmetadata.at(paletteHeader);
            //auto palette = static_cast<std::vector<util::color_uint8>>(value);
            //for (auto const& [armyColor, adjustment] : armyColors) {
            //    ret["armyColor"] = armyColor;
            //    auto paletteCopy = palette;
            //    for (auto& color : paletteCopy) {
            //        std::swap(color.red, color.blue);
            //        util::hsb hsb = static_cast<util::hsb>(static_cast<util::color>(color));
            //        float originalSaturation = hsb.saturation;
            //        if (originalSaturation >= 0.25) {
            //            hsb.hue = adjustment.hue;
            //            hsb.saturation = hsb.saturation * adjustment.saturation;
            //            hsb.brightness *= adjustment.brightness;
            //        }
            //        color = static_cast<util::color_uint8>(static_cast<util::color>(hsb));
            //        std::swap(color.red, color.blue);
            //    }
            //    value = util::metadata_value{ paletteCopy, util::value_type::PALETTE };
            //std::stringstream out;
            //util::write_image(out, *image, util::image_type::PNG, metadata);
            //    //auto newPath = path.parent_path() / ("zz" + armyColor + path.filename().string());
            //    //std::ofstream testOut{ newPath, std::ios::binary };
            //    //util::write_image(testOut, image, metadata);
            auto encoded = encode64(out);
            ret["smallImage"] = encoded;
            arr.push_back(ret);
            //}
        //}
    }
    ss << json::serialize(arr);
    auto out = saveFuture.get();
    if (out.size() != 1)
        return 1;
    util::write_text_file(out.front(), ss.str());
    return 0;
}