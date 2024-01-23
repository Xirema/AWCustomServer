#pragma once
#include<jss/jssinclude.h>

namespace dTypes{
    struct Settings{
        std::string name;
        std::optional<int64_t> startingFunds;
        std::optional<int64_t> incomeMultiplier;
        std::optional<bool> fogOfWar;
        std::optional<std::map<std::string, int64_t>> variant;
        std::optional<bool> coPowers;
        std::optional<bool> teams;
        std::optional<int64_t> unitLimit;
        std::optional<int64_t> captureLimit;
        std::optional<int64_t> dayLimit;
        std::optional<int64_t> coMeterSize;
        std::optional<int64_t> coMeterMultiplier;

        void readFrom(json::object const& obj) {
            if(auto ptr = obj.if_contains("name")) {
	            if(auto tPtr = ptr->if_string()) {
   	                name = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'name' as std::string in Settings, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'name' was missing in Settings");
    	    }
            if(auto ptr = obj.if_contains("startingFunds")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                startingFunds = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'startingFunds' as int64_t in Settings, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("incomeMultiplier")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                incomeMultiplier = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'incomeMultiplier' as int64_t in Settings, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("fogOfWar")) {
	            if(auto tPtr = ptr->if_bool()) {
   	                fogOfWar = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'fogOfWar' as bool in Settings, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("variant")) {
	            if(auto tPtr = ptr->if_object()) {
   				    std::map<std::string, int64_t> map;
   				    for(auto const& entry : *tPtr) {
   				        if(auto mPtr = entry.value().if_int64()) {
   				            map[entry.key_c_str()] = int64_t{*mPtr};
   				        } else {
   				            throw std::runtime_error("Entry in map 'variant' in Settings was expected to be int64, but was of wrong type.");
   				        }
   				    }
   				    variant = std::move(map);
   				} else if(!ptr->is_null()) {
   				    throw std::runtime_error("Expected 'variant' as object in Settings, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("coPowers")) {
	            if(auto tPtr = ptr->if_bool()) {
   	                coPowers = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'coPowers' as bool in Settings, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("teams")) {
	            if(auto tPtr = ptr->if_bool()) {
   	                teams = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'teams' as bool in Settings, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("unitLimit")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                unitLimit = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'unitLimit' as int64_t in Settings, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("captureLimit")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                captureLimit = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'captureLimit' as int64_t in Settings, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("dayLimit")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                dayLimit = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'dayLimit' as int64_t in Settings, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("coMeterSize")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                coMeterSize = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'coMeterSize' as int64_t in Settings, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("coMeterMultiplier")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                coMeterMultiplier = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'coMeterMultiplier' as int64_t in Settings, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
        }

        void writeTo(json::object & obj) const {
           	obj["name"] = name;
            if(startingFunds) {
          	    obj["startingFunds"] = *startingFunds;
          	}
            if(incomeMultiplier) {
          	    obj["incomeMultiplier"] = *incomeMultiplier;
          	}
            if(fogOfWar) {
          	    obj["fogOfWar"] = *fogOfWar;
          	}
      		if(variant) {
   			    json::object m;
      		    for(auto const& entry : *variant) {
   			        m[entry.first] = entry.second;
   			    }
   			    obj["variant"] = std::move(m);
   			}
            if(coPowers) {
          	    obj["coPowers"] = *coPowers;
          	}
            if(teams) {
          	    obj["teams"] = *teams;
          	}
            if(unitLimit) {
          	    obj["unitLimit"] = *unitLimit;
          	}
            if(captureLimit) {
          	    obj["captureLimit"] = *captureLimit;
          	}
            if(dayLimit) {
          	    obj["dayLimit"] = *dayLimit;
          	}
            if(coMeterSize) {
          	    obj["coMeterSize"] = *coMeterSize;
          	}
            if(coMeterMultiplier) {
          	    obj["coMeterMultiplier"] = *coMeterMultiplier;
          	}
        }
    };
}
