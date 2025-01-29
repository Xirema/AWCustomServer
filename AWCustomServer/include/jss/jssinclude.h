#pragma once
#include<boost/json.hpp>
#include<string>
#include<optional>
#include<map>
#include<cinttypes>

namespace json = boost::json;

namespace jss {
  // template<typename Type>
  // concept Serializable = requires(Type t, json::value v) {
  //   v = json::value_from(t);
  //   t = json::value_to<Type>(v);
  // };
  // template<typename Type>
  // concept Unserializable = requires(Type t, json::value v) {
  //   t = v;
  // };

  // inline void unserialize(int64_t& i, json::value const& v) {
  //   if(!v.is_number()) {
  //     throw std::runtime_error("json value expected to be of type number, but was " + std::string{to_string(v.kind())} + " instead.");
  //   }
  //   i = v.to_number<int64_t>();
  // }

  // inline void unserialize(uint64_t& i, json::value const& v) {
  //   if(!v.is_number()) {
  //     throw std::runtime_error("json value expected to be of type number, but was " + std::string{to_string(v.kind())} + " instead.");
  //   }
  //   i = v.to_number<uint64_t>();
  // }

  // inline void unserialize(double& i, json::value const& v) {
  //   if(!v.is_number()) {
  //     throw std::runtime_error("json value expected to be of type number, but was " + std::string{to_string(v.kind())} + " instead.");
  //   }
  //   i = v.to_number<double>();
  // }

  // inline void unserialize(float& i, json::value const& v) {
  //   if(!v.is_number()) {
  //     throw std::runtime_error("json value expected to be of type number, but was " + std::string{to_string(v.kind())} + " instead.");
  //   }
  //   i = v.to_number<float>();
  // }

  // inline void unserialize(int32_t& i, json::value const& v) {
  //   if(!v.is_number()) {
  //     throw std::runtime_error("json value expected to be of type number, but was " + std::string{to_string(v.kind())} + " instead.");
  //   }
  //   i = v.to_number<int32_t>();
  // }

  // inline void unserialize(uint32_t& i, json::value const& v) {
  //   if(!v.is_number()) {
  //     throw std::runtime_error("json value expected to be of type number, but was " + std::string{to_string(v.kind())} + " instead.");
  //   }
  //   i = v.to_number<uint32_t>();
  // }

  // inline void unserialize(std::string& i, json::value const& v) {
  //   if(!v.is_string()) {
  //     throw std::runtime_error("json value expected to be of type string, but was " + std::string{to_string(v.kind())} + " instead.");
  //   }
  //   i = std::string{v.as_string()};
  // }

  // inline void unserialize(bool& i, json::value const& v) {
  //   if(!v.is_bool()) {
  //     throw std::runtime_error("json value expected to be of type boolean, but was " + std::string{to_string(v.kind())} + " instead.");
  //   }
  //   i = v.as_bool();
  // }

  // template<typename Type>
  // requires Serializable<Type>
  // void unserialize(std::vector<Type> & vector, json::value const& v) {
  //   if(!v.is_array()) {
  //     throw std::runtime_error("json value expected to be of type array, but was " + std::string{to_string(v.kind())} + " instead.");
  //   }
  //   vector = {};
  //   for(auto const& val : v.as_array()) {
  //     auto & ret = vector.emplace_back();
  //     unserialize(ret, val);
  //   }
  // }

  // template<typename Type>
  // requires Serializable<Type>
  // void unserialize(std::optional<Type> & opt, json::value const& v) {
  //   if(v.is_null()) {
  //     opt = {};
  //     return;
  //   }
  //   auto& ret = opt.emplace();
  //   unserialize(ret, v);
  // }
}