#include<cstdint>
#include<utility>
#include<compare>
#include<boost/container_hash/hash.hpp>

namespace coord {
  struct Coord {
    int64_t x, y;
    constexpr auto operator<=>(Coord const& o) const = default;
  };
}
template<>
struct std::hash<coord::Coord> {
  constexpr size_t operator()(coord::Coord const& c) const {
    size_t ret = static_cast<size_t>(c.x);
    boost::hash_combine(ret, c.y);
    return ret;
  }
};