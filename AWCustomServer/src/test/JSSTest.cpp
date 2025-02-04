#include<RestFunctions.h>

int main() {
  net::HTTPHeaders headers;
  headers.httpHeaders["packid"] = "1";
  std::print("{}", rest::resource::get_resource_pack(headers));
}