#pragma once
#include<RestServer.h>
#include<jss/jssinclude.h>

namespace net {
  template<typename JSTypeIn, typename JSTypeOut>
  requires jss::JSONable<JSTypeIn> && jss::JSONable<JSTypeOut>
  void insert(HTTPFunctionMap & map, HTTPFunctionDefinition definition, std::function<JSTypeOut(HTTPHeaders const&, JSTypeIn const&)> function) {
    map[definition] = [function](HTTPHeaders const& headers, std::string body) {
      return boost::json::serialize(
        boost::json::value_from(
          function(
            headers, 
            boost::json::value_to<JSTypeIn>(
              boost::json::parse(body)
            )
          )
        )
      );
    };
  }

  HTTPFunc convert(GETFunc func) {
    return [func](HTTPHeaders const& headers, std::string body) {
      return func(headers);
    };
  }

  HTTPFunctionDefinition define(HTTPVerb verb, std::string name) {
    return {.verb=verb, .name=std::move(name)};
  }
}