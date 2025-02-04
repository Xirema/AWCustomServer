#include<Calculator.h>
#include<ranges>
#include<algorithm>
#include<Id.h>
#include<print>
namespace ranges = std::ranges;
namespace views = std::views;
using namespace dTypes;
using namespace sTypes;
using coord::Coord;

namespace {
  template<typename Type>
  concept Named = requires(Type obj) {
    {obj.name} -> std::convertible_to<std::string_view>;
  };

  template<typename Range>
  requires Named<ranges::range_value_t<Range>>
  auto find(Range && range, std::string_view name) -> decltype(&*range.begin()) {
    auto candidate = ranges::find_if(range, [name](Named auto && obj) {return obj.name == name;});
    if(candidate == range.end()) {
      return nullptr;
    }
    return &*candidate;
  }

  template<typename Key, typename Type>
  auto find(std::unordered_map<Key, Type> const& map, std::decay_t<Key> const& name) -> decltype(&map.begin()->second) {
    auto it = map.find(name);
    if(it == map.end()) {
      return nullptr;
    }
    return &it->second;
  }

  template<typename Range, typename Names>
  requires Named<ranges::range_value_t<Range>> 
  && std::is_same_v<std::string, ranges::range_value_t<Names>>
  std::vector<ranges::range_value_t<Range> const*> findAll(Range && range, Names && names) {
    std::vector<ranges::range_value_t<Range> const*> ret;
    for(std::string const& name : names) {
      if(auto const* ptr = find(range, name)) {
        ret.emplace_back(ptr);
      }
    }
    return ret;
  }

  template<typename T>
  std::vector<T> append(std::vector<T> a, std::vector<T> const& b) {
    a.insert(a.end(), b.begin(), b.end());
    return a;
  }
}

int64_t calc::calculateUnitMovementRange(
  UnitState const& unit,
  game::Game const& game,
  ModData const& modData
) {
  UnitType const* unitType = find(modData.units, unit.name);
  if(!unitType) {
    throw std::runtime_error("Unable to find unit type '" + unit.name + "'");
  }
  auto impactingEffects = getAllPassiveUnitEffects(unit, game, modData, [](auto && effect) {return effect->movementMod.has_value();});
  auto movementRange = unitType->movementRange;
  movementRange += ranges::fold_left(impactingEffects | views::elements<0>, 0ll, [](int64_t sum, PassiveUnitEffect const* effect) {
    return sum + effect->movementMod.value_or(0);
  });
  movementRange = std::min(unit.fuel, movementRange);
  return movementRange;
}

int64_t calc::calculateUnitVisionRange(
  UnitState const& unit,
  game::Game const& game,
  ModData const& modData
) {
  UnitType const* unitType = find(modData.units, unit.name);
  if(!unitType) {
    throw std::runtime_error("Unable to find unit type '" + unit.name + "'");
  }
  auto impactingEffects = getAllPassiveUnitEffects(unit, game, modData, [](PassiveUnitEffect const* effect) {
    return effect->visionMod.has_value() || effect->visionVariantMods.has_value();
  });
  auto visionRange = unitType->visionRange;
  auto currentVariant = getCurrentVariant(game, modData);
  visionRange += ranges::fold_left(impactingEffects | views::elements<0>, 0ll, [&](int64_t sum, PassiveUnitEffect const* effect) {
    if(effect->visionVariantMods) {
      if(auto it = effect->visionVariantMods->find(currentVariant); it != effect->visionVariantMods->end()) {
        sum += it->second;
      }
    }
    return sum + effect->visionMod.value_or(0);
  });
  return visionRange;
}


int64_t calc::calculateUnitFirepower(
  UnitState const& unit,
  game::Game const& game,
  ModData const& modData,
  bool attacking
) {
  UnitType const* unitType = find(modData.units, unit.name);
  if(!unitType) {
    throw std::runtime_error("Unable to find unit type '" + unit.name + "'");
  }
  auto impactingEffects = getAllPassiveUnitEffects(unit, game, modData, [](PassiveUnitEffect const* effect) {
    return 
      effect->firepowerFromFunds 
      || effect->firepowerFromOwnedTerrain
      || effect->firepowerMod
      || effect->firepowerVariantMods
      || effect->terrainStarsFirepower
      || effect->counterfireMod
    ;
  });
  auto currentVariant = getCurrentVariant(game, modData);
  int64_t unitFirepower = ranges::fold_left(
    impactingEffects,
    100,
    [&](int64_t sum, std::pair<PassiveUnitEffect const*, PlayerState const*> const& pair) {
      auto [effect, player] = pair;
      sum += effect->firepowerMod.value_or(0);
      if(effect->firepowerVariantMods) {
        if(auto it = effect->firepowerVariantMods->find(currentVariant); it != effect->firepowerVariantMods->end()) {
          sum += it->second;
        }
      }
      if(!attacking && effect->counterfireMod) {
        sum += effect->counterfireMod.value_or(0);
      }

      if(effect->terrainStarsFirepower) {
        sum += effect->terrainStarsFirepower.value_or(0) * calculateUnitTerrainStars(unit, game, modData);
      }

      if(effect->firepowerFromOwnedTerrain) {
        for(auto const& [terrainName, mod] : *effect->firepowerFromOwnedTerrain) {
          sum += mod * countTerrainsOwnedByPlayer(player->id, {terrainName}, game, modData);
        }
      }
      if(effect->firepowerFromFunds) {
        sum += effect->firepowerFromFunds.value_or(0) * player->funds / 1'000;
      }
      return sum;
    }
  );
  return unitFirepower;
}

int64_t calc::calculateUnitDefense(
  UnitState const& unit,
  game::Game const& game,
  ModData const& modData,
  bool attackerIndirect
) {
  UnitType const* unitType = find(modData.units, unit.name);
  if(!unitType) {
    throw std::runtime_error("Unable to find unit type '" + unit.name + "'");
  }
  auto impactingEffects = getAllPassiveUnitEffects(unit, game, modData, [](PassiveUnitEffect const* effect) {
    return 
      effect->defenseFromFunds
      || effect->defenseFromOwnedTerrain
      || effect->defenseMod
      || effect->defenseVariantMods
      || effect->indirectDefenseMod
      || effect->terrainStarsDefense
    ;
  });
  auto currentVariant = getCurrentVariant(game, modData);
  int64_t unitDefense = ranges::fold_left(
    impactingEffects,
    100,
    [&](int64_t sum, std::pair<PassiveUnitEffect const*, PlayerState const*> const& pair) {
      auto [effect, player] = pair;
      sum += effect->defenseMod.value_or(0);
      if(effect->defenseVariantMods) {
        if(auto it = effect->defenseVariantMods->find(currentVariant); it != effect->defenseVariantMods->end()) {
          sum += it->second;
        }
      }
      if(attackerIndirect && effect->indirectDefenseMod) {
        sum += effect->indirectDefenseMod.value_or(0);
      }

      if(effect->terrainStarsDefense) {
        sum += effect->terrainStarsDefense.value_or(0) * calculateUnitTerrainStars(unit, game, modData) * flatHitPoints(unit.hitPoints.value_or(100)) / 100;
      }

      if(effect->defenseFromOwnedTerrain) {
        for(auto const& [terrainName, mod] : *effect->defenseFromOwnedTerrain) {
          sum += mod * countTerrainsOwnedByPlayer(player->id, {terrainName}, game, modData);
        }
      }
      if(effect->defenseFromFunds) {
        sum += effect->defenseFromFunds.value_or(0) * player->funds / 1'000;
      }
      return sum;
    }
  );
  return unitDefense;
}

int64_t calc::calculateUnitLuck(
  UnitState const& unit,
  game::Game const& game,
  ModData const& modData,
  bool goodLuck
) {
  UnitType const* unitType = find(modData.units, unit.name);
  if(!unitType) {
    throw std::runtime_error("Unable to find unit type '" + unit.name + "'");
  }
  auto impactingEffects = getAllPassiveUnitEffects(unit, game, modData, [](PassiveUnitEffect const* effect) {
    return 
      effect->goodLuckMod
      || effect->badLuckMod
    ;
  });
  return std::max(0l, ranges::fold_left(
    impactingEffects | views::elements<0>,
    0,
    [goodLuck](int64_t luck, PassiveUnitEffect const* effect) {
      if(goodLuck) {
        luck += effect->goodLuckMod.value_or(0);
      } else {
        luck += effect->badLuckMod.value_or(0);
      }
      return luck;
    }
  ));
}

int64_t calc::calculateUnitRange(
  UnitState const& unit,
  int64_t weaponIndex,
  game::Game const& game,
  ModData const& modData,
  bool maxRange
) {
  UnitType const* unitType = find(modData.units, unit.name);
  if(!unitType) {
    throw std::runtime_error("Unable to find unit type '" + unit.name + "'");
  }
  if(!unitType->weapons) {
    throw std::runtime_error("Unit '" + unitType->name + "' does not have any weapons.");
  }
  auto const& weaponName = unitType->weapons->at(weaponIndex);
  auto weapon = find(modData.weapons, weaponName);
  if(!weapon) {
    throw std::runtime_error("Unable to find weapon '" + weaponName + "'");
  }
  auto impactingEffects = getAllPassiveUnitEffects(unit, game, modData, [](PassiveUnitEffect const* effect) {
    return 
      effect->maxRangeMod
      || effect->minRangeMod
    ;
  });
  return std::max(0l, ranges::fold_left(
    impactingEffects | views::elements<0>,
    maxRange ? weapon->maxRange : weapon->minRange.value_or(1),
    [maxRange](int64_t range, PassiveUnitEffect const* effect) {
      if(maxRange) {
        range += effect->maxRangeMod.value_or(0);
      } else {
        range += effect->minRangeMod.value_or(0);
      }
      return range;
    }
  ));
}

calc::UnitIntelFlags calc::getUnitIntel(
  UnitState const& unit,
  PlayerState const* observingPlayer,
  game::Game const& game,
  ModData const& modData
) {
  UnitType const* unitType = find(modData.units, unit.name);
  if(!unitType) {
    throw std::runtime_error("Unable to find unit type '" + unit.name + "'");
  }
  auto impactingEffects = getAllPassiveUnitEffects(
    unit,
    game,
    modData,
    [](PassiveUnitEffect const* effect) {
      return 
        effect->hiddenHitPoints
        || effect->hpPartVisible
        || effect->luckPointsVisible
      ;
    }
  );
  return ranges::fold_left(
    impactingEffects,
    UnitIntelFlags::NONE,
    [&](UnitIntelFlags flags, std::pair<PassiveUnitEffect const*, PlayerState const*> const& pair) {
      auto [effect, player] = pair;
      auto alliance = getAlliance(observingPlayer, player);
      if(effect->hiddenHitPoints) {
        if(
          (alliance == Alliance::SELF && ranges::any_of(*effect->hiddenHitPoints, [](std::string const& affects){return affects == "self" || affects == "own";}))
          || (alliance == Alliance::ALLY && ranges::any_of(*effect->hiddenHitPoints, [](std::string const& affects){return affects == "ally";}))
          || (alliance == Alliance::NEUTRAL && ranges::any_of(*effect->hiddenHitPoints, [](std::string const& affects){return affects == "neutral";}))
          || (alliance == Alliance::ENEMY && ranges::any_of(*effect->hiddenHitPoints, [](std::string const& affects){return affects == "enemy";}))
        ) {
          flags |= UnitIntelFlags::HIDE_HITPOINTS;
        }
      }
      if(effect->hpPartVisible) {
        if(
          (alliance == Alliance::SELF && ranges::any_of(*effect->hpPartVisible, [](std::string const& affects){return affects == "self" || affects == "own";}))
          || (alliance == Alliance::ALLY && ranges::any_of(*effect->hpPartVisible, [](std::string const& affects){return affects == "ally";}))
          || (alliance == Alliance::NEUTRAL && ranges::any_of(*effect->hpPartVisible, [](std::string const& affects){return affects == "neutral";}))
          || (alliance == Alliance::ENEMY && ranges::any_of(*effect->hpPartVisible, [](std::string const& affects){return affects == "enemy";}))
        ) {
          flags |= UnitIntelFlags::EXACT_HITPOINTS;
        }
      }
      if(effect->luckPointsVisible) {
        if(
          (alliance == Alliance::SELF && ranges::any_of(*effect->luckPointsVisible, [](std::string const& affects){return affects == "self" || affects == "own";}))
          || (alliance == Alliance::ALLY && ranges::any_of(*effect->luckPointsVisible, [](std::string const& affects){return affects == "ally";}))
          || (alliance == Alliance::NEUTRAL && ranges::any_of(*effect->luckPointsVisible, [](std::string const& affects){return affects == "neutral";}))
          || (alliance == Alliance::ENEMY && ranges::any_of(*effect->luckPointsVisible, [](std::string const& affects){return affects == "enemy";}))
        ) {
          flags |= UnitIntelFlags::LUCK;
        }
      }
      return flags;
    }
  );
}

int64_t calc::calculateUnitCapturePoints(
  UnitState const& unit,
  game::Game const& game,
  ModData const& modData
) {
  UnitType const* unitType = find(modData.units, unit.name);
  if(!unitType) {
    throw std::runtime_error("Unable to find unit type '" + unit.name + "'");
  }
  auto impactingEffects = getAllPassiveUnitEffects(
    unit,
    game,
    modData,
    [](PassiveUnitEffect const* effect) {
      return 
        effect->captureRateMod.has_value()
      ;
    }
  );
  return std::max(0l, ranges::fold_left(
    impactingEffects | views::elements<0>,
    100l,
    [](int64_t captureRate, PassiveUnitEffect const* effect) {
      return captureRate += effect->captureRateMod.value_or(0);
    }
  ) * unitType->captureSpeed.value_or(0) * flatHitPoints(unit.hitPoints.value_or(100)) / 10'000);
}

int64_t calc::calculateUnitCost(
  UnitState const& unit,
  game::Game const& game,
  ModData const& modData
) {
  UnitType const* unitType = find(modData.units, unit.name);
  if(!unitType) {
    throw std::runtime_error("Unable to find unit type '" + unit.name + "'");
  }
  auto impactingEffects = getAllPassiveUnitEffects(
    unit,
    game,
    modData,
    [](PassiveUnitEffect const* effect) {
      return 
        effect->unitCostMod.has_value()
      ;
    }
  );
  return std::max(0l, ranges::fold_left(
    impactingEffects | views::elements<0>,
    100l,
    [](int64_t cost, PassiveUnitEffect const* effect) {
      return cost + effect->unitCostMod.value_or(0);
    }
  )) * unitType->cost / 100;
}

std::optional<int64_t> calc::calculateMovementCost(
  UnitState const& unit,
  TerrainState const& terrain,
  game::Game const& game,
  ModData const& modData
) {
  UnitType const* unitType = find(modData.units, unit.name);
  if(!unitType) {
    throw std::runtime_error("Unable to find unit type '" + unit.name + "'");
  }
  MovementClass const* movementClass = find(modData.movements, unitType->movementClass);
  if(!movementClass) {
    throw std::runtime_error("Unable to find movement class '" + unitType->movementClass + "'");
  }
  PlayerState const* owningPlayer = find(game.playersById, unit.owner.value_or(-1));
  auto impactingUnitEffects = getAllPassiveGlobalEffects(owningPlayer, game, modData, [](PassiveGlobalEffect const* effect) {
    return 
      effect->movementClassVariantOverride 
      && effect->movementClassVariantReplace
    ;
  });
  auto currentVariant = getCurrentVariant(game, modData);
  for(auto effect : impactingUnitEffects | views::elements<0> | views::reverse) {
    if(currentVariant == effect->movementClassVariantReplace) {
      currentVariant = *effect->movementClassVariantOverride;
    }
  }
  std::optional<int64_t> movementCost;
  if(auto it = movementClass->movementCosts.find(terrain.name); it != movementClass->movementCosts.end()) {
    movementCost = it->second;
    if(auto mods = movementClass->variantMods) {
      if(auto it2 = mods->find(currentVariant); it2 != mods->end()) {
        if(auto it3 = it2->second.find(terrain.name); it3 != it2->second.end()) {
          *movementCost += it3->second;
        }
      }
    }
    if(movementCost < 0) {
      movementCost = 0;
    }
  }
  return movementCost;
}

std::string calc::getCurrentVariant(
  game::Game const& game,
  ModData const& modData
) {
  auto impactingEffects = getAllPassiveGlobalEffects(nullptr, game, modData, [](PassiveGlobalEffect const* effect) {return effect->variantMod.has_value();});
  if(impactingEffects.size() == 0) {
    return game.gameState.variant;
  }
  //TODO: Should this have a strict priority hierarchy, instead of always using the most recent player?
  return impactingEffects.back().first->variantMod.value_or("default");
}

PlayerState const* calc::getUnitOwner(
  UnitState const& unit, 
  game::Game const& game
) {
  return find(game.playersById, unit.owner.value_or(-1));
}

std::vector<std::pair<PassiveUnitEffect const*, PlayerState const*>>
calc::getAllPassiveUnitEffects(
  UnitState const& unit,
  game::Game const& game,
  ModData const& modData,
  std::function<bool(PassiveUnitEffect const*)> filter
) {
  UnitType const* unitType = find(modData.units, unit.name);
  if(!unitType) {
    throw std::runtime_error("Unable to find unit type '" + unit.name + "'");
  }
  PlayerState const* owningPlayer = find(game.playersById, unit.owner.value_or(-1));
  TerrainState const* terrain = nullptr;
  if(auto it = game.terrainsByCoordinate.find(Coord{unit}); it != game.terrainsByCoordinate.end()) {
    terrain = it->second;
  }
  std::vector<std::pair<PassiveUnitEffect const*, PlayerState const*>> allEffects;
  for(auto const& [id, player] : game.playersById) {
    auto alliance = getAlliance(&player, owningPlayer);
    std::vector<std::pair<PassiveUnitEffect const*, PlayerState const*>> playerEffects;
    PlayerType const* owningPlayerType = find(modData.players, player.playerType);
    CommanderType const* baselineType = find(modData.commanders, owningPlayerType ? owningPlayerType->commanderTypeMod.value_or("") : "");
    CommanderType const* commanderType = find(modData.commanders, player.commanderName);
    if(!game.settings.coPowers) {
      commanderType = nullptr;
    }

    auto getEffects = [&](std::optional<std::vector<std::string>> const& list) {
      if(list) {
        auto nextEffects = findAll(modData.passiveUnitEffects, *list);
        std::vector<std::pair<PassiveUnitEffect const*, PlayerState const*>> paired;
        ranges::transform(nextEffects, std::back_insert_iterator(paired), [&](PassiveUnitEffect const* effect) {
          return std::make_pair(effect, &player);
        });
        playerEffects.insert(playerEffects.end(), paired.begin(), paired.end());
      }
    };
    if(baselineType) {
      getEffects(baselineType->passiveUnitEffectsD2d);
      if(player.powerActive == "cop") {
        getEffects(baselineType->passiveUnitEffectsCop);
      }
      if(player.powerActive == "scop") {
        getEffects(baselineType->passiveUnitEffectsScop);
      }
    }
    if(commanderType) {
      getEffects(commanderType->passiveUnitEffectsD2d);
      if(player.powerActive == "cop") {
        getEffects(commanderType->passiveUnitEffectsCop);
      }
      if(player.powerActive == "scop") {
        getEffects(commanderType->passiveUnitEffectsScop);
      }
    }
    std::erase_if(playerEffects, [&](std::pair<PassiveUnitEffect const*, PlayerState const*> effectPair) {
      auto [effect, _player] = effectPair;
      bool targetsSelf = alliance == Alliance::SELF 
        && ranges::any_of(effect->targets.value_or(std::vector<std::string>{}), [](std::string const& target){return target == "own" || target == "self";});
      bool targetsAlly = alliance == Alliance::ALLY
        && ranges::any_of(effect->targets.value_or(std::vector<std::string>{}), [](std::string const& target){return target == "ally";});
      bool targetsEnemy = alliance == Alliance::ENEMY
        && ranges::any_of(effect->targets.value_or(std::vector<std::string>{}), [](std::string const& target){return target == "enemy";});
      if(!(targetsSelf || targetsAlly || targetsEnemy)) {
        return true;
      }
      
      if(!unitMatchesEffect(*unitType, terrain ? terrain->name : "", *effect)) {
        return true;
      }
      if(!filter(effect)) {
        return true;
      }

      return false;
    });
    allEffects.insert(allEffects.end(), playerEffects.begin(), playerEffects.end());
  }
  return allEffects;
}

std::vector<std::pair<PassiveTerrainEffect const*, PlayerState const*>>
calc::getAllPassiveTerrainEffects(
  TerrainState const& terrain,
  game::Game const& game,
  ModData const& modData,
  std::function<bool(PassiveTerrainEffect const*)> filter
) {
  std::vector<std::pair<PassiveTerrainEffect const*, PlayerState const*>> allEffects;
  auto terrainType = find(modData.terrains, terrain.name);
  if(!terrainType) {
    throw std::runtime_error("Unable to find Terrain Type '" + terrain.name + "'");
  }
  auto activePlayerId = game.gameState.playerOrder.at(game.gameState.playerTurn);
  auto activePlayer = find(game.playersById, activePlayerId);
  if(!activePlayer) {
    throw std::runtime_error(std::format("Unable to find Active Player '{}'.", activePlayerId));
  }
  auto terrainPlayer = find(game.playersById, terrain.owner.value_or(-1));
  auto terrainAlliance = getAlliance(terrainPlayer, activePlayer);
  for(auto const& [id, player] : game.playersById) {
    std::vector<std::pair<PassiveTerrainEffect const*, PlayerState const*>> playerEffects;
    auto powerAlliance = getAlliance(&player, activePlayer);
    PlayerType const* playerType = find(modData.players, player.playerType);
    CommanderType const* baselineType = find(modData.commanders, playerType ? playerType->commanderTypeMod.value_or("") : "");
    CommanderType const* commanderType = find(modData.commanders, player.commanderName);
    if(!game.settings.coPowers) {
      commanderType = nullptr;
    }
    
    auto getEffects = [&](std::optional<std::vector<std::string>> const& list) {
      if(list) {
        auto nextEffects = findAll(modData.passiveTerrainEffects, *list);
        std::vector<std::pair<PassiveTerrainEffect const*, PlayerState const*>> paired;
        ranges::transform(nextEffects, std::back_insert_iterator(paired), [&](PassiveTerrainEffect const* effect) {
          return std::make_pair(effect, &player);
        });
        playerEffects.insert(playerEffects.end(), paired.begin(), paired.end());
      }
    };
    if(baselineType) {
      getEffects(baselineType->passiveTerrainEffectsD2d);
      if(player.powerActive == "cop") {
        getEffects(baselineType->passiveTerrainEffectsCop);
      }
      if(player.powerActive == "scop") {
        getEffects(baselineType->passiveTerrainEffectsScop);
      }
    }
    if(commanderType) {
      getEffects(commanderType->passiveTerrainEffectsD2d);
      if(player.powerActive == "cop") {
        getEffects(commanderType->passiveTerrainEffectsCop);
      }
      if(player.powerActive == "scop") {
        getEffects(commanderType->passiveTerrainEffectsScop);
      }
    }
    std::erase_if(playerEffects, [&](std::pair<PassiveTerrainEffect const*, PlayerState const*> effectPair) {
      auto [effect, _player] = effectPair;
      bool targetsSelf = terrainAlliance == Alliance::SELF 
        && ranges::any_of(effect->targets.value_or(std::vector<std::string>{}), [](std::string const& target){return target == "own" || target == "self";});
      bool targetsAlly = terrainAlliance == Alliance::ALLY
        && ranges::any_of(effect->targets.value_or(std::vector<std::string>{}), [](std::string const& target){return target == "ally";});
      bool targetsEnemy = terrainAlliance == Alliance::ENEMY
        && ranges::any_of(effect->targets.value_or(std::vector<std::string>{}), [](std::string const& target){return target == "enemy";});
      bool targetsNeutral = terrainAlliance == Alliance::NEUTRAL
        && ranges::any_of(effect->targets.value_or(std::vector<std::string>{}), [](std::string const& target){return target == "neutral";});
      if(!(targetsSelf || targetsAlly || targetsEnemy || targetsNeutral)) {
        return true;
      }

      bool affectsSelf = powerAlliance == Alliance::SELF
        && ranges::any_of(effect->affects.value_or(std::vector<std::string>{}), [](std::string const& affect){return affect == "own" || affect == "self";});
      bool affectsAlly = powerAlliance == Alliance::ALLY
        && ranges::any_of(effect->affects.value_or(std::vector<std::string>{}), [](std::string const& affect){return affect == "ally";});
      bool affectsEnemy = powerAlliance == Alliance::ENEMY
        && ranges::any_of(effect->affects.value_or(std::vector<std::string>{}), [](std::string const& affect){return affect == "enemy";});
      if(!(affectsSelf || affectsAlly || affectsEnemy)) {
        return true;
      }

      if(!terrainMatchesEffect(*terrainType, *effect)) {
        return true;
      }

      if(!filter(effect)) {
        return true;
      }

      return false;
    });
    allEffects.insert(allEffects.end(), playerEffects.begin(), playerEffects.end());
  }
  return allEffects;
}

std::vector<std::pair<PassiveGlobalEffect const*, PlayerState const*>>
calc::getAllPassiveGlobalEffects(
  PlayerState const* targetPlayer,
  game::Game const& game,
  ModData const& modData,
  std::function<bool(PassiveGlobalEffect const*)> filter
) {
  std::vector<std::pair<PassiveGlobalEffect const*, PlayerState const*>> allEffects;
  auto activePlayerId = game.gameState.playerOrder.at(game.gameState.playerTurn);
  auto activePlayer = find(game.playersById, activePlayerId);
  if(!activePlayer) {
    throw std::runtime_error(std::format("Unable to find Active Player '{}'.", activePlayerId));
  }
  if(targetPlayer) {
    activePlayer = targetPlayer;
  }
  std::vector<PlayerState const*> orderedPlayers;
  for(auto start = game.gameState.playerTurn; start < game.gameState.playerTurn + static_cast<int64_t>(game.gameState.playerOrder.size()); start++) {
    auto lastInserted = orderedPlayers.emplace_back(find(game.playersById, game.gameState.playerOrder.at(start % game.gameState.playerOrder.size())));
    if(lastInserted == nullptr) {
      throw std::runtime_error("Unable to find Player.");
    }
  }
  for(auto const& playerPtr : orderedPlayers) {
    auto const& player = *playerPtr;
    std::vector<std::pair<PassiveGlobalEffect const*, PlayerState const*>> playerEffects;
    auto alliance = getAlliance(&player, activePlayer);
    PlayerType const* playerType = find(modData.players, player.playerType);
    CommanderType const* baselineType = find(modData.commanders, playerType ? playerType->commanderTypeMod.value_or("") : "");
    CommanderType const* commanderType = find(modData.commanders, player.commanderName);
    if(!game.settings.coPowers) {
      commanderType = nullptr;
    }
    
    auto getEffects = [&](std::optional<std::vector<std::string>> const& list) {
      if(list) {
        auto nextEffects = findAll(modData.passiveGlobalEffects, *list);
        std::vector<std::pair<PassiveGlobalEffect const*, PlayerState const*>> paired;
        ranges::transform(nextEffects, std::back_insert_iterator(paired), [&](PassiveGlobalEffect const* effect) {
          return std::make_pair(effect, &player);
        });
        playerEffects.insert(playerEffects.end(), paired.begin(), paired.end());
      }
    };
    if(baselineType) {
      getEffects(baselineType->passiveGlobalEffectsD2d);
      if(player.powerActive == "cop") {
        getEffects(baselineType->passiveGlobalEffectsCop);
      }
      if(player.powerActive == "scop") {
        getEffects(baselineType->passiveGlobalEffectsScop);
      }
    }
    if(commanderType) {
      getEffects(commanderType->passiveGlobalEffectsD2d);
      if(player.powerActive == "cop") {
        getEffects(commanderType->passiveGlobalEffectsCop);
      }
      if(player.powerActive == "scop") {
        getEffects(commanderType->passiveGlobalEffectsScop);
      }
    }
    std::erase_if(playerEffects, [&](std::pair<PassiveGlobalEffect const*, PlayerState const*> effectPair) {
      auto [effect, _player] = effectPair;
      bool targetsSelf = alliance == Alliance::SELF
        && ranges::any_of(effect->targets.value_or(std::vector<std::string>{}), [](std::string const& target){return target == "own" || target == "self";});
      bool targetsAlly = alliance == Alliance::ALLY
        && ranges::any_of(effect->targets.value_or(std::vector<std::string>{}), [](std::string const& target){return target == "ally";});
      bool targetsEnemy = alliance == Alliance::ENEMY
        && ranges::any_of(effect->targets.value_or(std::vector<std::string>{}), [](std::string const& target){return target == "enemy";});
      if(!(targetsSelf || targetsAlly || targetsEnemy)) {
        return true;
      }
      
      if(!filter(effect)) {
        return true;
      }

      return false;
    });
    allEffects.insert(allEffects.end(), playerEffects.begin(), playerEffects.end());
  }
  return allEffects;
}

bool calc::unitMatchesEffect(
  UnitType const& unitType, 
  std::string_view terrainName, 
  PassiveUnitEffect const& effect
) {
  if(
    effect.unitTypeRequired 
    && effect.unitTypeRequired->size() > 0
    && ranges::none_of(*effect.unitTypeRequired, [&](std::string const& unitTypeReq) {return unitTypeReq == unitType.name;})
  ) {
    return false;
  }
  if(
    effect.terrainRequired
    && effect.terrainRequired->size() > 0
    && ranges::none_of(*effect.terrainRequired, [&](std::string const& terrainTypeReq) {return terrainTypeReq == terrainName;})
  ) {
    return false;
  }
  if(
    effect.classificationRequired
    && effect.classificationRequired->size() > 0
    && !ranges::all_of(*effect.classificationRequired, [&unitType](std::string const& classificationOriginal) {
      std::string_view classification = classificationOriginal;
      bool negate = classification.size() > 0 && classification.at(0) == '!';
      if(negate) {
        classification = classification.substr(1);
      }
      auto test = [classification](std::string const& unitClassification) {return classification == unitClassification;};
      if(negate) {
        return ranges::none_of(unitType.classifications, test);
      } 
      return ranges::any_of(unitType.classifications, test);
    })
  ) {
    return false;
  }
  return true;
}

bool calc::terrainMatchesEffect(
  TerrainType const& terrainType,
  PassiveTerrainEffect const& effect
) {
  if(
    effect.terrainRequired
    && effect.terrainRequired->size() > 0
    && ranges::none_of(*effect.terrainRequired, [&](std::string terrainNameReq){return terrainType.name == terrainNameReq;})
  ) {
    return false;
  }
  //TODO: Passive Terrain Effects specify "classifications required". Figure out what that means.
  return true;
}

int64_t calc::countTerrainsOwnedByPlayer(
  int64_t playerId,
  std::vector<std::string> const& terrainNames,
  game::Game const& game,
  ModData const& modData
) {
  return ranges::fold_left(
    game.terrainsById | views::elements<1>,
    0,
    [&terrainNames, playerId](int64_t sum, TerrainState const& terrain) {
      if(
        ranges::any_of(terrainNames, [&terrain, playerId](std::string const& terrainReq) {return terrain.name == terrainReq;})
        && terrain.owner == playerId
      ) {
        return sum + 1;
      }
      return sum;
    }
  );
}

int64_t calc::calculateUnitTerrainStars(
  UnitState const& unit,
  game::Game const& game,
  ModData const& modData
) {
  auto unitType = find(modData.units, unit.name);
  if(!unitType) {
    throw std::runtime_error("Unable to find unit type '" + unit.name + "'");
  }
  auto impactingEffects = getAllPassiveUnitEffects(
    unit, game, modData, 
    [](PassiveUnitEffect const* effect) {
      return
        effect->terrainStarsMod
        || effect->terrainStarsFlatMod
      ;
    }
  );
  TerrainState const* terrain = nullptr;
  if(auto it = game.terrainsByCoordinate.find(unit); it == game.terrainsByCoordinate.end()) {
    throw std::runtime_error("Unable to find terrain at " + std::format("{},{}", unit.x, unit.y));
  } else {
    terrain = it->second;
  }
  auto terrainType = find(modData.terrains, terrain->name);
  if(!terrainType) {
    throw std::runtime_error("Unable to find terrain type '" + terrain->name + "'");
  }
  int64_t terrainStars = terrainType->stars;
  int64_t flatMod = ranges::fold_left(
    impactingEffects | views::elements<0>,
    0,
    [](int64_t sum, PassiveUnitEffect const* effect) {
      return sum + effect->terrainStarsFlatMod.value_or(0);
    }
  );
  int64_t multiplier = ranges::fold_left(
    impactingEffects | views::elements<0>,
    0,
    [](int64_t sum, PassiveUnitEffect const* effect) {
      return sum + effect->terrainStarsMod.value_or(0);
    }
  );
  return (terrainStars + flatMod) * multiplier / 100;
}

calc::Alliance calc::getAlliance(
  PlayerState const* a,
  PlayerState const* b
) {
  if(!a || !b) {
    return Alliance::NEUTRAL;
  }
  if(a->id == b->id) {
    return Alliance::SELF;
  }
  if(a->team && a->team == b->team) {
    return Alliance::ALLY;
  }
  return Alliance::ENEMY;
}