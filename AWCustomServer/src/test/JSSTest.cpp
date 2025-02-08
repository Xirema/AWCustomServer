#include<RestFunctions.h>

int main() {
  net::HTTPHeaders headers;
  // headers.httpHeaders["packid"] = "1";
  // std::print("{}", rest::resource::get_resource_pack(headers));
  headers.httpHeaders["gameid"] = "-1";
  std::print("{}\n", rest::state::get_gamestate(headers));
  //std::print("{}\n", rest::state::get_unitstates(headers));
  //std::print("{}\n", rest::state::get_terrainstates(headers));
  //std::print("{}\n", rest::state::get_playerstates(headers));
  //std::print("{}\n", rest::state::get_settingstate(headers));
  int64_t maxValue = std::numeric_limits<int64_t>::max();
  boost::json::object ret;
  ret["value"] = maxValue;
  std::print("{}\n", json::serialize(ret));
}