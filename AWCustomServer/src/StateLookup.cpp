#include <RestFunctions.h>
#include <jss/GameState.h>
#include <jss/UnitState.h>
#include <jss/PlayerState.h>
#include <jss/SettingsState.h>
#include <jss/TerrainState.h>

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
  return boost::json::serialize(json::value_from(gameState));
}

std::string rest::state::get_playerstates(net::HTTPHeaders const &headers)
{
  auto gameId = get_id(headers);
  auto ret = db::get_playerstates(gameId);
  return serialize(json::value_from(ret));
}

std::string rest::state::get_unitstates(net::HTTPHeaders const &headers)
{
  auto gameId = get_id(headers);
  auto ret = db::get_unitstates(gameId);
  return serialize(json::value_from(ret));
}

std::string rest::state::get_terrainstates(net::HTTPHeaders const &headers)
{
  auto gameId = get_id(headers);
  auto ret = db::get_terrainstates(gameId);
  return serialize(json::value_from(ret));
}

std::string rest::state::get_settingstate(net::HTTPHeaders const &headers)
{
  auto gameId = get_id(headers);
  auto ret = db::get_settingstate(gameId);
  return serialize(json::value_from(ret));
}
