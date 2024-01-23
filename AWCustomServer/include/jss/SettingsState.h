#pragma once
#include<jss/jssinclude.h>

namespace sTypes{
    struct SettingsState{
        std::string id;
        int64_t startingFunds;
        int64_t incomeMultiplier;
        bool fogOfWar;
        std::map<std::string, int64_t> variant;
        bool coPowers;
        bool teams;
        std::string modId;
        int64_t coMeterSize;
        int64_t coMeterMultiplier;
        std::optional<int64_t> unitLimit;
        std::optional<int64_t> captureLimit;
        std::optional<int64_t> dayLimit;

        void readFrom(json::object const& obj) {
            if(auto ptr = obj.if_contains("id")) {
	            if(auto tPtr = ptr->if_string()) {
   	                id = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'id' as std::string in SettingsState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'id' was missing in SettingsState");
    	    }
            if(auto ptr = obj.if_contains("startingFunds")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                startingFunds = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'startingFunds' as int64_t in SettingsState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'startingFunds' was missing in SettingsState");
    	    }
            if(auto ptr = obj.if_contains("incomeMultiplier")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                incomeMultiplier = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'incomeMultiplier' as int64_t in SettingsState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'incomeMultiplier' was missing in SettingsState");
    	    }
            if(auto ptr = obj.if_contains("fogOfWar")) {
	            if(auto tPtr = ptr->if_bool()) {
   	                fogOfWar = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'fogOfWar' as bool in SettingsState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'fogOfWar' was missing in SettingsState");
    	    }
            if(auto ptr = obj.if_contains("variant")) {
	            if(auto tPtr = ptr->if_object()) {
   				    std::map<std::string, int64_t> map;
   				    for(auto const& entry : *tPtr) {
   				        if(auto mPtr = entry.value().if_int64()) {
   				            map[entry.key_c_str()] = int64_t{*mPtr};
   				        } else {
   				            throw std::runtime_error("Entry in map 'variant' in SettingsState was expected to be int64, but was of wrong type.");
   				        }
   				    }
   				    variant = std::move(map);
    			} else {
   				    throw std::runtime_error("Expected 'variant' as object in SettingsState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'variant' was missing in SettingsState");
    	    }
            if(auto ptr = obj.if_contains("coPowers")) {
	            if(auto tPtr = ptr->if_bool()) {
   	                coPowers = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'coPowers' as bool in SettingsState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'coPowers' was missing in SettingsState");
    	    }
            if(auto ptr = obj.if_contains("teams")) {
	            if(auto tPtr = ptr->if_bool()) {
   	                teams = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'teams' as bool in SettingsState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'teams' was missing in SettingsState");
    	    }
            if(auto ptr = obj.if_contains("modId")) {
	            if(auto tPtr = ptr->if_string()) {
   	                modId = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'modId' as std::string in SettingsState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'modId' was missing in SettingsState");
    	    }
            if(auto ptr = obj.if_contains("coMeterSize")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                coMeterSize = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'coMeterSize' as int64_t in SettingsState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'coMeterSize' was missing in SettingsState");
    	    }
            if(auto ptr = obj.if_contains("coMeterMultiplier")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                coMeterMultiplier = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'coMeterMultiplier' as int64_t in SettingsState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'coMeterMultiplier' was missing in SettingsState");
    	    }
            if(auto ptr = obj.if_contains("unitLimit")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                unitLimit = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'unitLimit' as int64_t in SettingsState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("captureLimit")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                captureLimit = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'captureLimit' as int64_t in SettingsState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("dayLimit")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                dayLimit = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'dayLimit' as int64_t in SettingsState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
        }

        void writeTo(json::object & obj) const {
           	obj["id"] = id;
           	obj["startingFunds"] = startingFunds;
           	obj["incomeMultiplier"] = incomeMultiplier;
           	obj["fogOfWar"] = fogOfWar;
       		{
   			    json::object m;
       		    for(auto const& entry : variant) {
   			        m[entry.first] = entry.second;
   			    }
   			    obj["variant"] = std::move(m);
   			}
           	obj["coPowers"] = coPowers;
           	obj["teams"] = teams;
           	obj["modId"] = modId;
           	obj["coMeterSize"] = coMeterSize;
           	obj["coMeterMultiplier"] = coMeterMultiplier;
            if(unitLimit) {
          	    obj["unitLimit"] = *unitLimit;
          	}
            if(captureLimit) {
          	    obj["captureLimit"] = *captureLimit;
          	}
            if(dayLimit) {
          	    obj["dayLimit"] = *dayLimit;
          	}
        }
    };
}
