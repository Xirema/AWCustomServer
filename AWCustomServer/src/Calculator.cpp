#include<Calculator.h>
#include<ranges>
#include<algorithm>
#include<Id.h>
namespace ranges = std::ranges;
namespace views = std::views;
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
  sTypes::UnitState const& unit,
  game::Game const& game,
  dTypes::ModData const& modData
) {
  dTypes::UnitType const* unitType = find(modData.units, unit.name);
  if(!unitType) {
    throw std::runtime_error("Unable to find unit type '" + unit.name + "'");
  }
  sTypes::PlayerState const* owningPlayer = find(game.playersById, i64(unit.owner.value_or("-1")));
  if(!owningPlayer) {
    return 0;
  }
  auto impactingEffects = getAllPassiveUnitEffects(unit, game, modData, [](auto && effect) {return effect->movementMod.has_value();});
  auto movementRange = unitType->movementRange;
  movementRange += ranges::fold_left(impactingEffects, 0ll, [](int64_t sum, dTypes::PassiveUnitEffect const* effect) {
    return sum + effect->movementMod.value_or(0);
  });
  movementRange = std::min(unit.fuel, movementRange);
  return movementRange;
}

std::vector<dTypes::PassiveUnitEffect const*>
calc::getAllPassiveUnitEffects(
  sTypes::UnitState const& unit,
  game::Game const& game,
  dTypes::ModData const& modData,
  std::function<bool(dTypes::PassiveUnitEffect const*)> filter
) {
  dTypes::UnitType const* unitType = find(modData.units, unit.name);
  if(!unitType) {
    throw std::runtime_error("Unable to find unit type '" + unit.name + "'");
  }
  sTypes::PlayerState const* owningPlayer = find(game.playersById, i64(unit.owner.value_or("-1")));
  sTypes::TerrainState const* terrain = nullptr;
  if(auto it = game.terrainsByCoordinate.find(Coord{unit}); it != game.terrainsByCoordinate.end()) {
    terrain = it->second;
  }
  std::vector<dTypes::PassiveUnitEffect const*> allEffects;
  for(auto const& [id, player] : game.playersById) {
    bool sameAsPlayer = player.id == unit.owner;
    bool allyOfPlayer = player.id != unit.owner 
      && owningPlayer 
      && owningPlayer->team 
      && owningPlayer->team == player.team;
    bool enemyOfPlayer = player.id != unit.owner
      && owningPlayer
      && (!owningPlayer->team || owningPlayer->team != player.team);
    std::vector<dTypes::PassiveUnitEffect const*> playerEffects;
    dTypes::PlayerType const* owningPlayerType = find(modData.players, player.id);
    dTypes::CommanderType const* baselineType = find(modData.commanders, owningPlayerType ? owningPlayerType->commanderTypeMod.value_or("") : "");
    dTypes::CommanderType const* commanderType = find(modData.commanders, player.commanderName);
    if(!game.settings.coPowers) {
      commanderType = nullptr;
    }

    if(baselineType) {
      if(baselineType->passiveUnitEffectsD2d) {
        playerEffects = append(playerEffects, findAll(modData.passiveUnitEffects, *baselineType->passiveUnitEffectsD2d));
      }
      if(player.powerActive == "cop" && baselineType->passiveUnitEffectsCop) {
        playerEffects = append(playerEffects, findAll(modData.passiveUnitEffects, *baselineType->passiveUnitEffectsCop));
      }
      if(player.powerActive == "scop" && baselineType->passiveUnitEffectsScop) {
        playerEffects = append(playerEffects, findAll(modData.passiveUnitEffects, *baselineType->passiveUnitEffectsScop));
      }
    }
    if(commanderType) {
      if(commanderType->passiveUnitEffectsD2d) {
        playerEffects = append(playerEffects, findAll(modData.passiveUnitEffects, *commanderType->passiveUnitEffectsD2d));
      }
      if(player.powerActive == "cop" && commanderType->passiveUnitEffectsCop) {
        playerEffects = append(playerEffects, findAll(modData.passiveUnitEffects, *commanderType->passiveUnitEffectsCop));
      }
      if(player.powerActive == "scop" && commanderType->passiveUnitEffectsScop) {
        playerEffects = append(playerEffects, findAll(modData.passiveUnitEffects, *commanderType->passiveUnitEffectsScop));
      }
    }
    std::erase_if(playerEffects, [&](dTypes::PassiveUnitEffect const* effect) {
      bool targetsSelf = sameAsPlayer 
        && ranges::any_of(effect->targets.value_or(std::vector<std::string>{}), [](std::string const& target){return target == "own" || target == "self";});
      bool targetsAlly = allyOfPlayer 
        && ranges::any_of(effect->targets.value_or(std::vector<std::string>{}), [](std::string const& target){return target == "ally";});
      bool targetsEnemy = enemyOfPlayer
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
    allEffects = append(allEffects, playerEffects);
  }
  return allEffects;
}

bool calc::unitMatchesEffect(
  dTypes::UnitType const& unitType, 
  std::string_view terrainName, 
  dTypes::PassiveUnitEffect const& effect
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