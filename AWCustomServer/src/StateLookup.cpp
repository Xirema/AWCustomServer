#include <RestFunctions.h>
#include<GameManager.h>
#include<ranges>

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
  game::GameManager & manager = game::GameManager::instance();
  auto gamePtr = manager.getGame(gameId);
  std::unique_lock lock{gamePtr->gameMutex};
  return boost::json::serialize(json::value_from(gamePtr->object.gameState));
}

std::string rest::state::get_playerstates(net::HTTPHeaders const &headers)
{
  auto gameId = get_id(headers);
  game::GameManager & manager = game::GameManager::instance();
  auto gamePtr = manager.getGame(gameId);
  std::unique_lock lock{gamePtr->gameMutex};
  return serialize(json::value_from(gamePtr->object.playersById | std::views::elements<1>));
}

std::string rest::state::get_unitstates(net::HTTPHeaders const &headers)
{
  auto gameId = get_id(headers);
  game::GameManager & manager = game::GameManager::instance();
  auto gamePtr = manager.getGame(gameId);
  std::unique_lock lock{gamePtr->gameMutex};
  return serialize(json::value_from(gamePtr->object.unitsById | std::views::elements<1>));
}

std::string rest::state::get_terrainstates(net::HTTPHeaders const &headers)
{
  auto gameId = get_id(headers);
  game::GameManager & manager = game::GameManager::instance();
  auto gamePtr = manager.getGame(gameId);
  std::unique_lock lock{gamePtr->gameMutex};
  return serialize(json::value_from(gamePtr->object.terrainsById | std::views::elements<1>));
}

std::string rest::state::get_settingstate(net::HTTPHeaders const &headers)
{
  auto gameId = get_id(headers);
  game::GameManager & manager = game::GameManager::instance();
  auto gamePtr = manager.getGame(gameId);
  std::unique_lock lock{gamePtr->gameMutex};
  return serialize(json::value_from(gamePtr->object.settings));
}
