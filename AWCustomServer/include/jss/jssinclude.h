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
  // template<typename T>
  // constexpr bool Optional(T const&) {return false;}
  // template<typename T>
  // constexpr bool Optional(std::optional<T> const&) {return true;}
  // // template<typename T>
  // // constexpr bool Optional(boost::optional<T> const&) {return true;}

  // template<typename Type>
  // concept JSONType = requires(Type t, json::value v) {
  //   v = json::value_from(t);
  //   t = json::value_to<Type>(v);
  // };

  // template<unsigned Size>
  // struct FixedString {
  //   char buf[Size+1]{};
  //   constexpr FixedString(char const* s) {
  //     for(unsigned i = 0; i < Size; ++i) {
  //       buf[i] = s[i];
  //     }
  //   }
  //   constexpr operator char const*() const {return buf;}
  // };

  // template<unsigned Size> FixedString(char const(&)[Size]) -> FixedString<Size-1>;

  // template<JSONType Type, FixedString Name>
  // struct Property {
  //   Property() = default;
  //   Property(Type t) : value(std::move(t)) {}
  //   Type value{};
  //   void operator<<(json::value const& v) {
  //     if(auto const* obj = v.if_object()) {
  //       if(auto const* target = obj->if_contains(json::string_view{Name})) {
  //         try {
  //           value = json::value_to<Type>(*target);
  //         } catch (...) {
  //           throw std::runtime_error(std::format("Member {} in JSON object is wrong type: {}", static_cast<char const*>(Name), std::string{to_string(v.kind())}));
  //         }
  //       } else {
  //         if (!Optional(value)) {
  //           throw std::runtime_error(std::format("Expected member {} in JSON object, but was missing", static_cast<char const*>(Name)));
  //         }
  //       }
  //     } else {
  //       throw std::runtime_error(std::format("Expected JSON Object, but received {} instead.", to_string(v.kind())));
  //     }
  //   }
  //   void operator>>(json::value & v) const {
  //     if (auto *obj = v.if_object()) {
  //       (*obj)[json::string_view{Name}] = json::value_from(value);
  //     } else {
  //       throw std::runtime_error("JSON value needs to be an object to be written to");
  //     }
  //   }
  // };

  // template<typename Obj, typename Elem>
  // concept JSONObject = requires(Obj o) {
  //   {o.properties} -> std::ranges::range;
  //   std::is_same_v<std::ranges::range_value_t<decltype(o.properties)>, Elem>;
  // };

  // class ErasedProperty {
  //   void* prop;
  //   using ReadFunc = std::function<void(json::value const&)>;
  //   using WriteFunc = std::function<void(json::value &)>;
  //   ReadFunc read;
  //   WriteFunc write;
  // public:
  //   template<JSONType Type, FixedString Name>
  //   ErasedProperty(Property<Type, Name> & prop):
  //     prop(&prop), 
  //     read([this](json::value const& v) {(*reinterpret_cast<Property<Type, Name>*>(&this->prop)) << v;}),
  //     write([this](json::value & v) {(*reinterpret_cast<Property<Type, Name>*>(&this->prop)) >> v;})
  //   {}

  //   template<typename Obj>
  //   requires JSONObject<Obj, ErasedProperty>
  //   ErasedProperty(Obj && obj):
  //     prop(&obj),
  //     read([this](json::value const& v) {
  //       auto * ptr = reinterpret_cast<Obj*>(&this->prop);
  //       for(auto & property : ptr->properties) {
  //         property << v;
  //       }
  //     }),
  //     write([this](json::value & v) {
  //       auto const* ptr = reinterpret_cast<Obj*>(&this->prop);
  //       for(auto const& property : ptr->properties) {
  //         property >> v;
  //       }
  //     })
  //   {}

  //   void operator<<(json::value const& v) {
  //     read(v);
  //   }
  //   void operator>>(json::value & v) const {
  //     write(v);
  //   }
  // };
  // template<typename Obj, typename... Elems>
  // requires JSONObject<Obj, ErasedProperty>
  // void registerProperties(Obj & object, Elems && ... elements) {
  //   (object.properties.emplace_back(std::forward<Elems>(elements)), ...);
  // }

  // template<typename Obj>
  // requires JSONObject<Obj, ErasedProperty>
  // void read(Obj & obj, json::value const& v) {
  //   for(auto & property : obj.properties) {
  //     property << v;
  //   }
  // }

  // template<typename Obj>
  // requires JSONObject<Obj, ErasedProperty>
  // void write(Obj & obj, json::value & v) {
  //   for(auto const& property : obj.properties) {
  //     property >> v;
  //   }
  // }
}