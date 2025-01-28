#pragma once
#include<cinttypes>
#include<string>

namespace dbs {
  struct UnitType {
    std::string name;
    std::int64_t cost;
    std::int64_t maxFuel;
    std::int64_t maxAmmo;
  };
}
    
