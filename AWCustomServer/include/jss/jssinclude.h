#pragma once
#include<boost/json.hpp>
#include<string>
#include<string_view>
#include<optional>
#include<map>
#include<cinttypes>
#include<any>
#include<boost/describe.hpp>

namespace json = boost::json;

namespace jss {
  template<typename JType>
  concept JSONable = requires(JType obj, boost::json::value val) {
    {boost::json::value_to<JType>(val)} -> std::convertible_to<JType>;
    {boost::json::value_from(obj)} -> std::convertible_to<boost::json::value>;
  };
}