#pragma once
#include "jssinclude.h"
#include <Coord.h>

namespace act
{
  using coord::Coord;
  struct Move
  {
    std::string type;
    std::optional<Coord> from;
    std::optional<Coord> to;
  };
  BOOST_DESCRIBE_STRUCT(Move, (), (type, from, to))

  struct GameAction
  {
    int64_t currentGameHash;
    std::string actionType;
    std::string sourceType; // unit, terrain, player
    int64_t sourceId;
    std::optional<std::string> targetType;
    std::optional<int64_t> targetId;
    std::optional<std::vector<Move>> moves;
  };
  BOOST_DESCRIBE_STRUCT(GameAction, (), (currentGameHash, actionType, sourceType, sourceId, targetType, targetId, moves))

  struct MoveResult
  {
    std::string type;
    std::optional<Coord> from;
    std::optional<Coord> to;
    std::optional<int64_t> damage;
  };
  BOOST_DESCRIBE_STRUCT(MoveResult, (), (type, from, to, damage))

  struct GameActionResult
  {
    std::string result;
    std::optional<std::vector<MoveResult>> moves;
    std::optional<int64_t> newGameHash;
  };
  BOOST_DESCRIBE_STRUCT(GameActionResult, (), (result, moves, newGameHash))
}