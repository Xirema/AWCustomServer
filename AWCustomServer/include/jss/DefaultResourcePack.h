#pragma once
#include<jss/jssinclude.h>

namespace dTypes{
    struct DefaultResourcePack{
        std::string name;
        std::optional<std::string> version;

        void readFrom(json::object const& obj) {
            if(auto ptr = obj.if_contains("name")) {
	            if(auto tPtr = ptr->if_string()) {
   	                name = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'name' as std::string in DefaultResourcePack, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'name' was missing in DefaultResourcePack");
    	    }
            if(auto ptr = obj.if_contains("version")) {
	            if(auto tPtr = ptr->if_string()) {
   	                version = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'version' as std::string in DefaultResourcePack, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
        }

        void writeTo(json::object & obj) const {
           	obj["name"] = name;
            if(version) {
          	    obj["version"] = *version;
          	}
        }
    };
}
