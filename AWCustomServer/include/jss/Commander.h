#pragma once
#include<jss/jssinclude.h>

namespace dTypes{
    struct CommanderType{
        std::string name;
        std::optional<std::vector<std::string>> passiveUnitEffectsD2d;
        std::optional<std::vector<std::string>> passiveTerrainEffectsD2d;
        std::optional<std::vector<std::string>> passiveGlobalEffectsD2d;
        std::optional<int64_t> copCost;
        std::optional<std::vector<std::string>> passiveUnitEffectsCop;
        std::optional<std::vector<std::string>> activeUnitEffectsCop;
        std::optional<std::vector<std::string>> passiveTerrainEffectsCop;
        std::optional<std::vector<std::string>> activeTerrainEffectsCop;
        std::optional<std::vector<std::string>> passiveGlobalEffectsCop;
        std::optional<std::vector<std::string>> activeGlobalEffectsCop;
        std::optional<int64_t> scopCost;
        std::optional<std::vector<std::string>> passiveUnitEffectsScop;
        std::optional<std::vector<std::string>> activeUnitEffectsScop;
        std::optional<std::vector<std::string>> passiveTerrainEffectsScop;
        std::optional<std::vector<std::string>> activeTerrainEffectsScop;
        std::optional<std::vector<std::string>> passiveGlobalEffectsScop;
        std::optional<std::vector<std::string>> activeGlobalEffectsScop;
        std::optional<int64_t> coMeterMultiplier;
        std::optional<bool> playable;

        void readFrom(json::object const& obj) {
            if(auto ptr = obj.if_contains("name")) {
	            if(auto tPtr = ptr->if_string()) {
   	                name = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'name' as std::string in CommanderType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'name' was missing in CommanderType");
    	    }
            if(auto ptr = obj.if_contains("passiveUnitEffectsD2d")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'passiveUnitEffectsD2d' in CommanderType was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    passiveUnitEffectsD2d = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'passiveUnitEffectsD2d' as array in CommanderType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("passiveTerrainEffectsD2d")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'passiveTerrainEffectsD2d' in CommanderType was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    passiveTerrainEffectsD2d = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'passiveTerrainEffectsD2d' as array in CommanderType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("passiveGlobalEffectsD2d")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'passiveGlobalEffectsD2d' in CommanderType was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    passiveGlobalEffectsD2d = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'passiveGlobalEffectsD2d' as array in CommanderType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("copCost")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                copCost = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'copCost' as int64_t in CommanderType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("passiveUnitEffectsCop")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'passiveUnitEffectsCop' in CommanderType was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    passiveUnitEffectsCop = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'passiveUnitEffectsCop' as array in CommanderType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("activeUnitEffectsCop")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'activeUnitEffectsCop' in CommanderType was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    activeUnitEffectsCop = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'activeUnitEffectsCop' as array in CommanderType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("passiveTerrainEffectsCop")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'passiveTerrainEffectsCop' in CommanderType was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    passiveTerrainEffectsCop = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'passiveTerrainEffectsCop' as array in CommanderType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("activeTerrainEffectsCop")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'activeTerrainEffectsCop' in CommanderType was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    activeTerrainEffectsCop = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'activeTerrainEffectsCop' as array in CommanderType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("passiveGlobalEffectsCop")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'passiveGlobalEffectsCop' in CommanderType was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    passiveGlobalEffectsCop = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'passiveGlobalEffectsCop' as array in CommanderType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("activeGlobalEffectsCop")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'activeGlobalEffectsCop' in CommanderType was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    activeGlobalEffectsCop = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'activeGlobalEffectsCop' as array in CommanderType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("scopCost")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                scopCost = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'scopCost' as int64_t in CommanderType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("passiveUnitEffectsScop")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'passiveUnitEffectsScop' in CommanderType was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    passiveUnitEffectsScop = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'passiveUnitEffectsScop' as array in CommanderType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("activeUnitEffectsScop")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'activeUnitEffectsScop' in CommanderType was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    activeUnitEffectsScop = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'activeUnitEffectsScop' as array in CommanderType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("passiveTerrainEffectsScop")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'passiveTerrainEffectsScop' in CommanderType was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    passiveTerrainEffectsScop = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'passiveTerrainEffectsScop' as array in CommanderType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("activeTerrainEffectsScop")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'activeTerrainEffectsScop' in CommanderType was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    activeTerrainEffectsScop = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'activeTerrainEffectsScop' as array in CommanderType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("passiveGlobalEffectsScop")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'passiveGlobalEffectsScop' in CommanderType was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    passiveGlobalEffectsScop = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'passiveGlobalEffectsScop' as array in CommanderType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("activeGlobalEffectsScop")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'activeGlobalEffectsScop' in CommanderType was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    activeGlobalEffectsScop = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'activeGlobalEffectsScop' as array in CommanderType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("coMeterMultiplier")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                coMeterMultiplier = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'coMeterMultiplier' as int64_t in CommanderType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("playable")) {
	            if(auto tPtr = ptr->if_bool()) {
   	                playable = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'playable' as bool in CommanderType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
        }

        void writeTo(json::object & obj) const {
           	obj["name"] = name;
       		if(passiveUnitEffectsD2d) {
    		    json::array arr;
       		    for(auto const& val : *passiveUnitEffectsD2d) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["passiveUnitEffectsD2d"] = std::move(arr);
    		}
       		if(passiveTerrainEffectsD2d) {
    		    json::array arr;
       		    for(auto const& val : *passiveTerrainEffectsD2d) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["passiveTerrainEffectsD2d"] = std::move(arr);
    		}
       		if(passiveGlobalEffectsD2d) {
    		    json::array arr;
       		    for(auto const& val : *passiveGlobalEffectsD2d) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["passiveGlobalEffectsD2d"] = std::move(arr);
    		}
            if(copCost) {
          	    obj["copCost"] = *copCost;
          	}
       		if(passiveUnitEffectsCop) {
    		    json::array arr;
       		    for(auto const& val : *passiveUnitEffectsCop) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["passiveUnitEffectsCop"] = std::move(arr);
    		}
       		if(activeUnitEffectsCop) {
    		    json::array arr;
       		    for(auto const& val : *activeUnitEffectsCop) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["activeUnitEffectsCop"] = std::move(arr);
    		}
       		if(passiveTerrainEffectsCop) {
    		    json::array arr;
       		    for(auto const& val : *passiveTerrainEffectsCop) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["passiveTerrainEffectsCop"] = std::move(arr);
    		}
       		if(activeTerrainEffectsCop) {
    		    json::array arr;
       		    for(auto const& val : *activeTerrainEffectsCop) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["activeTerrainEffectsCop"] = std::move(arr);
    		}
       		if(passiveGlobalEffectsCop) {
    		    json::array arr;
       		    for(auto const& val : *passiveGlobalEffectsCop) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["passiveGlobalEffectsCop"] = std::move(arr);
    		}
       		if(activeGlobalEffectsCop) {
    		    json::array arr;
       		    for(auto const& val : *activeGlobalEffectsCop) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["activeGlobalEffectsCop"] = std::move(arr);
    		}
            if(scopCost) {
          	    obj["scopCost"] = *scopCost;
          	}
       		if(passiveUnitEffectsScop) {
    		    json::array arr;
       		    for(auto const& val : *passiveUnitEffectsScop) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["passiveUnitEffectsScop"] = std::move(arr);
    		}
       		if(activeUnitEffectsScop) {
    		    json::array arr;
       		    for(auto const& val : *activeUnitEffectsScop) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["activeUnitEffectsScop"] = std::move(arr);
    		}
       		if(passiveTerrainEffectsScop) {
    		    json::array arr;
       		    for(auto const& val : *passiveTerrainEffectsScop) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["passiveTerrainEffectsScop"] = std::move(arr);
    		}
       		if(activeTerrainEffectsScop) {
    		    json::array arr;
       		    for(auto const& val : *activeTerrainEffectsScop) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["activeTerrainEffectsScop"] = std::move(arr);
    		}
       		if(passiveGlobalEffectsScop) {
    		    json::array arr;
       		    for(auto const& val : *passiveGlobalEffectsScop) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["passiveGlobalEffectsScop"] = std::move(arr);
    		}
       		if(activeGlobalEffectsScop) {
    		    json::array arr;
       		    for(auto const& val : *activeGlobalEffectsScop) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["activeGlobalEffectsScop"] = std::move(arr);
    		}
            if(coMeterMultiplier) {
          	    obj["coMeterMultiplier"] = *coMeterMultiplier;
          	}
            if(playable) {
          	    obj["playable"] = *playable;
          	}
        }
    };
    struct PlayerType{
        std::string name;
        std::optional<std::string> commanderTypeMod;
        std::optional<std::vector<std::string>> permittedPlayerSlots;
        std::optional<std::vector<std::string>> permittedCommanderTypes;
        std::optional<std::string> teamName;

        void readFrom(json::object const& obj) {
            if(auto ptr = obj.if_contains("name")) {
	            if(auto tPtr = ptr->if_string()) {
   	                name = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'name' as std::string in PlayerType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'name' was missing in PlayerType");
    	    }
            if(auto ptr = obj.if_contains("commanderTypeMod")) {
	            if(auto tPtr = ptr->if_string()) {
   	                commanderTypeMod = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'commanderTypeMod' as std::string in PlayerType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("permittedPlayerSlots")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'permittedPlayerSlots' in PlayerType was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    permittedPlayerSlots = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'permittedPlayerSlots' as array in PlayerType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("permittedCommanderTypes")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'permittedCommanderTypes' in PlayerType was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    permittedCommanderTypes = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'permittedCommanderTypes' as array in PlayerType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("teamName")) {
	            if(auto tPtr = ptr->if_string()) {
   	                teamName = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'teamName' as std::string in PlayerType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
        }

        void writeTo(json::object & obj) const {
           	obj["name"] = name;
            if(commanderTypeMod) {
          	    obj["commanderTypeMod"] = *commanderTypeMod;
          	}
       		if(permittedPlayerSlots) {
    		    json::array arr;
       		    for(auto const& val : *permittedPlayerSlots) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["permittedPlayerSlots"] = std::move(arr);
    		}
       		if(permittedCommanderTypes) {
    		    json::array arr;
       		    for(auto const& val : *permittedCommanderTypes) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["permittedCommanderTypes"] = std::move(arr);
    		}
            if(teamName) {
          	    obj["teamName"] = *teamName;
          	}
        }
    };
}
