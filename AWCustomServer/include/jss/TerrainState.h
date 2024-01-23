#pragma once
#include<jss/jssinclude.h>

namespace sTypes{
    struct TerrainState{
        std::string id;
        int64_t x;
        int64_t y;
        std::string name;
        std::optional<int64_t> orientation;
        std::optional<int64_t> capturePoints;
        std::optional<std::string> owner;
        std::optional<int64_t> activationCount;
        std::optional<int64_t> hitPoints;

        void readFrom(json::object const& obj) {
            if(auto ptr = obj.if_contains("id")) {
	            if(auto tPtr = ptr->if_string()) {
   	                id = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'id' as std::string in TerrainState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'id' was missing in TerrainState");
    	    }
            if(auto ptr = obj.if_contains("x")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                x = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'x' as int64_t in TerrainState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'x' was missing in TerrainState");
    	    }
            if(auto ptr = obj.if_contains("y")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                y = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'y' as int64_t in TerrainState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'y' was missing in TerrainState");
    	    }
            if(auto ptr = obj.if_contains("name")) {
	            if(auto tPtr = ptr->if_string()) {
   	                name = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'name' as std::string in TerrainState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'name' was missing in TerrainState");
    	    }
            if(auto ptr = obj.if_contains("orientation")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                orientation = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'orientation' as int64_t in TerrainState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("capturePoints")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                capturePoints = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'capturePoints' as int64_t in TerrainState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("owner")) {
	            if(auto tPtr = ptr->if_string()) {
   	                owner = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'owner' as std::string in TerrainState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("activationCount")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                activationCount = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'activationCount' as int64_t in TerrainState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("hitPoints")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                hitPoints = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'hitPoints' as int64_t in TerrainState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
        }

        void writeTo(json::object & obj) const {
           	obj["id"] = id;
           	obj["x"] = x;
           	obj["y"] = y;
           	obj["name"] = name;
            if(orientation) {
          	    obj["orientation"] = *orientation;
          	}
            if(capturePoints) {
          	    obj["capturePoints"] = *capturePoints;
          	}
            if(owner) {
          	    obj["owner"] = *owner;
          	}
            if(activationCount) {
          	    obj["activationCount"] = *activationCount;
          	}
            if(hitPoints) {
          	    obj["hitPoints"] = *hitPoints;
          	}
        }
    };
}
