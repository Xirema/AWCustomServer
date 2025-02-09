#pragma once
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <ranges>
#include<algorithm>
#include <type_traits>
#include<cstdint>
#include<string>
#include<vector>

namespace base64
{
  template <typename Range>
    requires std::is_same_v<std::ranges::range_value_t<Range>, uint8_t>
  std::string encode(Range &&blob)
  {
    using namespace boost::archive::iterators;
    using It = base64_from_binary<transform_width<typename std::decay_t<Range>::const_iterator, 6, 8>>;
    std::string ret;
    std::transform(
      It(std::begin(blob)), 
      It(std::end(blob)), 
      std::back_inserter(ret), 
      [](uint8_t v){ return static_cast<char>(v); }
    );
    return ret.append((3 - blob.size() % 3) % 3, '=');
  }

  template <typename Range>
    requires std::is_same_v<std::ranges::range_value_t<Range>, char>
  std::vector<uint8_t> decode(Range &&val)
  {
    using namespace boost::archive::iterators;
    using It = transform_width<binary_from_base64<typename std::decay_t<Range>::const_iterator>, 8, 6>;
    auto padding = count(val.begin(), val.end(), '=');
    std::vector<uint8_t> ret;
    std::transform(
      It(std::begin(val)),
      It(std::end(val)),
      std::back_inserter(ret),
      [](char c) {return static_cast<uint8_t>(c);}
    );
    ret.resize(ret.size() - padding);
    return ret;
  }
}