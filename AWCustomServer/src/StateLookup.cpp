#include <RestFunctions.h>
#include <jss/GameState.h>
#include <jss/UnitState.h>
#include <jss/PlayerState.h>
#include <jss/SettingsState.h>
#include <jss/TerrainState.h>
#include <random>
#include <boost/mysql.hpp>

namespace mysql = boost::mysql;
namespace
{
  int64_t get_id(net::HTTPHeaders const &headers)
  {
    if (auto ret = headers.httpHeaders.find("gameid"); ret != headers.httpHeaders.end())
    {
      return std::stoll(ret->second);
    }
    throw net::RestError("Game Id not specified", net::RestError::Type::INVALID_DATA);
  }
}

std::string rest::state::get_gamestate(net::HTTPHeaders const &headers)
{
  auto gameId = get_id(headers);
  sTypes::GameState gameState = db::get_gamestate(gameId);
  boost::json::object obj;
  gameState.writeTo(obj);
  return boost::json::serialize(obj);
}

std::string rest::state::get_playerstates(net::HTTPHeaders const &headers)
{
  auto gameId = get_id(headers);
  auto playerStates = db::get_playerstates(gameId);

  boost::json::array arr;
  for (auto const &player : playerStates)
  {
    boost::json::object obj;
    player.writeTo(obj);
    arr.push_back(std::move(obj));
  }
  return boost::json::serialize(arr);
}

std::string rest::state::get_unitstates(net::HTTPHeaders const &headers)
{
  auto gameId = get_id(headers);
  auto unitStates = db::get_unitstates(gameId);
  boost::json::array arr;
  for (auto const &unit : unitStates)
  {
    boost::json::object obj;
    unit.writeTo(obj);
    arr.push_back(std::move(obj));
  }
  return boost::json::serialize(arr);
}

std::string rest::state::get_terrainstates(net::HTTPHeaders const &headers)
{
  auto gameId = get_id(headers);
  auto terrainStates = db::get_terrainstates(gameId);
  boost::json::array arr;
  for (auto const &terrain : terrainStates)
  {
    boost::json::object obj;
    terrain.writeTo(obj);
    arr.push_back(std::move(obj));
  }
  return boost::json::serialize(arr);
}

std::string rest::state::get_settingstate(net::HTTPHeaders const &headers)
{
  auto gameId = get_id(headers);
  auto settingState = db::get_settingstate(gameId);
  boost::json::object obj;
  settingState.writeTo(obj);
  return boost::json::serialize(obj);
}
