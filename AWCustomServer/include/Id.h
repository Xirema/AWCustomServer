#include<cstdint>
#include<string>

constexpr uint64_t u64(std::string_view s) {
  return std::strtoull(&s.front(), nullptr, 10);
}

inline std::string str(uint64_t i) {
  return std::to_string(i);
}

constexpr int64_t i64(std::string_view s) {
  return std::strtoll(&s.front(), nullptr, 10);
}

inline std::string str(int64_t i) {
  return std::to_string(i);
}