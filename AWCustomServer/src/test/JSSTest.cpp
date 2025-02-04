#include<RestFunctions.h>

int main() {
  net::HTTPHeaders headers;
  // headers.httpHeaders["packid"] = "1";
  // std::print("{}", rest::resource::get_resource_pack(headers));
  headers.httpHeaders["gameid"] = "-1";
  std::print("{}\n", rest::state::get_gamestate(headers));
  std::print("{}\n", rest::state::get_unitstates(headers));
  //std::print("{}\n", rest::state::get_terrainstates(headers));
  std::print("{}\n", rest::state::get_playerstates(headers));
  std::print("{}\n", rest::state::get_settingstate(headers));
}