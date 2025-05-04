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

  template<typename JSTypeOut>
  requires jss::JSONable<JSTypeOut>
  void insert(HTTPFunctionMap & map, HTTPFunctionDefinition definition, std::function<JSTypeOut(HTTPHeaders const&)> function) {
    map[definition] = [function](HTTPHeaders const& headers, std::string body) {
      return boost::json::serialize(
        boost::json::value_from(
          function(
            headers
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

  HTTPFunctionDefinition defineGet(std::string name) {
    return {.verb=HTTPVerb::GET, .name=std::move(name)};
  }
  HTTPFunctionDefinition definePost(std::string name) {
    return {.verb=HTTPVerb::POST, .name=std::move(name)};
  }
  HTTPFunctionDefinition definePut(std::string name) {
    return {.verb=HTTPVerb::PUT, .name=std::move(name)};
  }
  HTTPFunctionDefinition defineDelete(std::string name) {
    return {.verb=HTTPVerb::DELETE, .name=std::move(name)};
  }
}