#pragma once
#include<jss/jssinclude.h>

namespace sTypes{
    struct UnitState{
        std::string id;
        int64_t x;
        int64_t y;
        std::string name;
        int64_t ammo;
        int64_t fuel;
        bool active;
        std::optional<int64_t> stunned;
        std::optional<std::vector<std::string>> transporting;
        std::optional<std::string> owner;
        std::optional<int64_t> hitPoints;
        std::optional<bool> stealthed;
        std::optional<int64_t> currentLuck;

        void readFrom(json::object const& obj) {
            if(auto ptr = obj.if_contains("id")) {
	            if(auto tPtr = ptr->if_string()) {
   	                id = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'id' as std::string in UnitState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'id' was missing in UnitState");
    	    }
            if(auto ptr = obj.if_contains("x")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                x = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'x' as int64_t in UnitState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'x' was missing in UnitState");
    	    }
            if(auto ptr = obj.if_contains("y")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                y = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'y' as int64_t in UnitState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'y' was missing in UnitState");
    	    }
            if(auto ptr = obj.if_contains("name")) {
	            if(auto tPtr = ptr->if_string()) {
   	                name = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'name' as std::string in UnitState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'name' was missing in UnitState");
    	    }
            if(auto ptr = obj.if_contains("ammo")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                ammo = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'ammo' as int64_t in UnitState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'ammo' was missing in UnitState");
    	    }
            if(auto ptr = obj.if_contains("fuel")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                fuel = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'fuel' as int64_t in UnitState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'fuel' was missing in UnitState");
    	    }
            if(auto ptr = obj.if_contains("active")) {
	            if(auto tPtr = ptr->if_bool()) {
   	                active = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'active' as bool in UnitState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'active' was missing in UnitState");
    	    }
            if(auto ptr = obj.if_contains("stunned")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                stunned = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'stunned' as int64_t in UnitState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("transporting")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'transporting' in UnitState was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    transporting = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'transporting' as array in UnitState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("owner")) {
	            if(auto tPtr = ptr->if_string()) {
   	                owner = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'owner' as std::string in UnitState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("hitPoints")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                hitPoints = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'hitPoints' as int64_t in UnitState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("stealthed")) {
	            if(auto tPtr = ptr->if_bool()) {
   	                stealthed = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'stealthed' as bool in UnitState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("currentLuck")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                currentLuck = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'currentLuck' as int64_t in UnitState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
        }

        void writeTo(json::object & obj) const {
           	obj["id"] = id;
           	obj["x"] = x;
           	obj["y"] = y;
           	obj["name"] = name;
           	obj["ammo"] = ammo;
           	obj["fuel"] = fuel;
           	obj["active"] = active;
            if(stunned) {
          	    obj["stunned"] = *stunned;
          	}
       		if(transporting) {
    		    json::array arr;
       		    for(auto const& val : *transporting) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["transporting"] = std::move(arr);
    		}
            if(owner) {
          	    obj["owner"] = *owner;
          	}
            if(hitPoints) {
          	    obj["hitPoints"] = *hitPoints;
          	}
            if(stealthed) {
          	    obj["stealthed"] = *stealthed;
          	}
            if(currentLuck) {
          	    obj["currentLuck"] = *currentLuck;
          	}
        }
    };
}
