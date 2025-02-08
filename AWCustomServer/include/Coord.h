#pragma once
#include<cstdint>
#include<utility>
#include<compare>
#include<boost/container_hash/hash.hpp>
#include<boost/describe.hpp>

namespace coord {
  template<typename Type>
  concept Coordlike = requires(Type t) {
    {t.x} -> std::convertible_to<int64_t>;
    {t.y} -> std::convertible_to<int64_t>;
  };
  struct Coord {
    constexpr Coord() = default;
    constexpr Coord(int64_t x, int64_t y): x(x), y(y) {}
    constexpr Coord(Coordlike auto && obj): Coord(obj.x, obj.y) {}
    int64_t x, y;
    constexpr auto operator<=>(Coord const& o) const = default;
  };
  BOOST_DESCRIBE_STRUCT(Coord, (), (x, y))
}
template<>
struct std::hash<coord::Coord> {
  constexpr size_t operator()(coord::Coord const& c) const {
    size_t ret = static_cast<size_t>(c.x);
    boost::hash_combine(ret, c.y);
    return ret;
  }
};