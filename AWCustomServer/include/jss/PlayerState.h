#pragma once
#include<jss/jssinclude.h>

namespace sTypes{
    struct PlayerState{
        std::string id;
        std::string owner;
        std::string commanderName;
        int64_t funds;
        int64_t powerCharge;
        std::string armyColor;
        bool alive;
        int64_t totalPowerUses;
        int64_t unitFacing;
        std::string playerType;
        std::optional<std::string> team;
        std::optional<std::string> powerActive;
        std::optional<int64_t> powerActiveDay;
        std::optional<std::vector<std::string>> bannedUnits;
        std::optional<int64_t> incomeMultiplier;
        std::optional<int64_t> coMeterMultiplier;

        void readFrom(json::object const& obj) {
            if(auto ptr = obj.if_contains("id")) {
	            if(auto tPtr = ptr->if_string()) {
   	                id = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'id' as std::string in PlayerState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'id' was missing in PlayerState");
    	    }
            if(auto ptr = obj.if_contains("owner")) {
	            if(auto tPtr = ptr->if_string()) {
   	                owner = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'owner' as std::string in PlayerState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'owner' was missing in PlayerState");
    	    }
            if(auto ptr = obj.if_contains("commanderName")) {
	            if(auto tPtr = ptr->if_string()) {
   	                commanderName = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'commanderName' as std::string in PlayerState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'commanderName' was missing in PlayerState");
    	    }
            if(auto ptr = obj.if_contains("funds")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                funds = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'funds' as int64_t in PlayerState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'funds' was missing in PlayerState");
    	    }
            if(auto ptr = obj.if_contains("powerCharge")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                powerCharge = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'powerCharge' as int64_t in PlayerState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'powerCharge' was missing in PlayerState");
    	    }
            if(auto ptr = obj.if_contains("armyColor")) {
	            if(auto tPtr = ptr->if_string()) {
   	                armyColor = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'armyColor' as std::string in PlayerState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'armyColor' was missing in PlayerState");
    	    }
            if(auto ptr = obj.if_contains("alive")) {
	            if(auto tPtr = ptr->if_bool()) {
   	                alive = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'alive' as bool in PlayerState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'alive' was missing in PlayerState");
    	    }
            if(auto ptr = obj.if_contains("totalPowerUses")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                totalPowerUses = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'totalPowerUses' as int64_t in PlayerState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'totalPowerUses' was missing in PlayerState");
    	    }
            if(auto ptr = obj.if_contains("unitFacing")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                unitFacing = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'unitFacing' as int64_t in PlayerState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'unitFacing' was missing in PlayerState");
    	    }
            if(auto ptr = obj.if_contains("playerType")) {
	            if(auto tPtr = ptr->if_string()) {
   	                playerType = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'playerType' as std::string in PlayerState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'playerType' was missing in PlayerState");
    	    }
            if(auto ptr = obj.if_contains("team")) {
	            if(auto tPtr = ptr->if_string()) {
   	                team = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'team' as std::string in PlayerState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("powerActive")) {
	            if(auto tPtr = ptr->if_string()) {
   	                powerActive = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'powerActive' as std::string in PlayerState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("powerActiveDay")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                powerActiveDay = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'powerActiveDay' as int64_t in PlayerState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("bannedUnits")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'bannedUnits' in PlayerState was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    bannedUnits = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'bannedUnits' as array in PlayerState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("incomeMultiplier")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                incomeMultiplier = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'incomeMultiplier' as int64_t in PlayerState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("coMeterMultiplier")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                coMeterMultiplier = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'coMeterMultiplier' as int64_t in PlayerState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
        }

        void writeTo(json::object & obj) const {
           	obj["id"] = id;
           	obj["owner"] = owner;
           	obj["commanderName"] = commanderName;
           	obj["funds"] = funds;
           	obj["powerCharge"] = powerCharge;
           	obj["armyColor"] = armyColor;
           	obj["alive"] = alive;
           	obj["totalPowerUses"] = totalPowerUses;
           	obj["unitFacing"] = unitFacing;
           	obj["playerType"] = playerType;
            if(team) {
          	    obj["team"] = *team;
          	}
            if(powerActive) {
          	    obj["powerActive"] = *powerActive;
          	}
            if(powerActiveDay) {
          	    obj["powerActiveDay"] = *powerActiveDay;
          	}
       		if(bannedUnits) {
    		    json::array arr;
       		    for(auto const& val : *bannedUnits) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["bannedUnits"] = std::move(arr);
    		}
            if(incomeMultiplier) {
          	    obj["incomeMultiplier"] = *incomeMultiplier;
          	}
            if(coMeterMultiplier) {
          	    obj["coMeterMultiplier"] = *coMeterMultiplier;
          	}
        }
    };
}
