#pragma once
#include<jss/jssinclude.h>

namespace dTypes{
    struct UnitType{
        std::string name;
        int64_t cost;
        int64_t maxFuel;
        int64_t maxAmmo;
        int64_t visionRange;
        int64_t movementRange;
        std::string movementClass;
        std::vector<std::string> classifications;
        std::optional<int64_t> fuelPerDay;
        std::optional<int64_t> fuelPerDayStealth;
        std::optional<std::vector<std::string>> weapons;
        std::optional<int64_t> supplyRepair;
        std::optional<int64_t> transportCapacity;
        std::optional<std::vector<std::string>> transportList;
        std::optional<int64_t> hitPoints;
        std::optional<int64_t> captureSpeed;
        std::optional<bool> ignoresVisionOcclusion;
        std::optional<std::string> stealthType;
        std::optional<bool> stationaryFire;

        void readFrom(json::object const& obj) {
            if(auto ptr = obj.if_contains("name")) {
	            if(auto tPtr = ptr->if_string()) {
   	                name = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'name' as std::string in UnitType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'name' was missing in UnitType");
    	    }
            if(auto ptr = obj.if_contains("cost")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                cost = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'cost' as int64_t in UnitType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'cost' was missing in UnitType");
    	    }
            if(auto ptr = obj.if_contains("maxFuel")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                maxFuel = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'maxFuel' as int64_t in UnitType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'maxFuel' was missing in UnitType");
    	    }
            if(auto ptr = obj.if_contains("maxAmmo")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                maxAmmo = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'maxAmmo' as int64_t in UnitType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'maxAmmo' was missing in UnitType");
    	    }
            if(auto ptr = obj.if_contains("visionRange")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                visionRange = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'visionRange' as int64_t in UnitType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'visionRange' was missing in UnitType");
    	    }
            if(auto ptr = obj.if_contains("movementRange")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                movementRange = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'movementRange' as int64_t in UnitType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'movementRange' was missing in UnitType");
    	    }
            if(auto ptr = obj.if_contains("movementClass")) {
	            if(auto tPtr = ptr->if_string()) {
   	                movementClass = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'movementClass' as std::string in UnitType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'movementClass' was missing in UnitType");
    	    }
            if(auto ptr = obj.if_contains("classifications")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'classifications' in UnitType was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    classifications = std::move(arr);
    			} else {
    			    throw std::runtime_error("Expected 'classifications' as array in UnitType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'classifications' was missing in UnitType");
    	    }
            if(auto ptr = obj.if_contains("fuelPerDay")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                fuelPerDay = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'fuelPerDay' as int64_t in UnitType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("fuelPerDayStealth")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                fuelPerDayStealth = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'fuelPerDayStealth' as int64_t in UnitType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("weapons")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'weapons' in UnitType was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    weapons = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'weapons' as array in UnitType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("supplyRepair")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                supplyRepair = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'supplyRepair' as int64_t in UnitType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("transportCapacity")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                transportCapacity = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'transportCapacity' as int64_t in UnitType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("transportList")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'transportList' in UnitType was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    transportList = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'transportList' as array in UnitType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("hitPoints")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                hitPoints = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'hitPoints' as int64_t in UnitType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("captureSpeed")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                captureSpeed = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'captureSpeed' as int64_t in UnitType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("ignoresVisionOcclusion")) {
	            if(auto tPtr = ptr->if_bool()) {
   	                ignoresVisionOcclusion = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'ignoresVisionOcclusion' as bool in UnitType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("stealthType")) {
	            if(auto tPtr = ptr->if_string()) {
   	                stealthType = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'stealthType' as std::string in UnitType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("stationaryFire")) {
	            if(auto tPtr = ptr->if_bool()) {
   	                stationaryFire = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'stationaryFire' as bool in UnitType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
        }

        void writeTo(json::object & obj) const {
           	obj["name"] = name;
           	obj["cost"] = cost;
           	obj["maxFuel"] = maxFuel;
           	obj["maxAmmo"] = maxAmmo;
           	obj["visionRange"] = visionRange;
           	obj["movementRange"] = movementRange;
           	obj["movementClass"] = movementClass;
        	{
    		    json::array arr;
        	    for(auto const& val : classifications) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["classifications"] = std::move(arr);
    		}
            if(fuelPerDay) {
          	    obj["fuelPerDay"] = *fuelPerDay;
          	}
            if(fuelPerDayStealth) {
          	    obj["fuelPerDayStealth"] = *fuelPerDayStealth;
          	}
       		if(weapons) {
    		    json::array arr;
       		    for(auto const& val : *weapons) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["weapons"] = std::move(arr);
    		}
            if(supplyRepair) {
          	    obj["supplyRepair"] = *supplyRepair;
          	}
            if(transportCapacity) {
          	    obj["transportCapacity"] = *transportCapacity;
          	}
       		if(transportList) {
    		    json::array arr;
       		    for(auto const& val : *transportList) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["transportList"] = std::move(arr);
    		}
            if(hitPoints) {
          	    obj["hitPoints"] = *hitPoints;
          	}
            if(captureSpeed) {
          	    obj["captureSpeed"] = *captureSpeed;
          	}
            if(ignoresVisionOcclusion) {
          	    obj["ignoresVisionOcclusion"] = *ignoresVisionOcclusion;
          	}
            if(stealthType) {
          	    obj["stealthType"] = *stealthType;
          	}
            if(stationaryFire) {
          	    obj["stationaryFire"] = *stationaryFire;
          	}
        }
    };
    struct WeaponType{
        std::string name;
        int64_t ammoConsumed;
        std::optional<std::map<std::string, int64_t>> baseDamage;
        int64_t maxRange;
        std::optional<int64_t> minRange;
        std::optional<bool> selfTarget;
        std::optional<bool> affectedByLuck;
        std::optional<bool> nonLethal;
        std::optional<int64_t> areaOfEffect;
        std::optional<std::vector<std::string>> targetsStealth;
        std::optional<int64_t> flatDamage;

        void readFrom(json::object const& obj) {
            if(auto ptr = obj.if_contains("name")) {
	            if(auto tPtr = ptr->if_string()) {
   	                name = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'name' as std::string in WeaponType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'name' was missing in WeaponType");
    	    }
            if(auto ptr = obj.if_contains("ammoConsumed")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                ammoConsumed = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'ammoConsumed' as int64_t in WeaponType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'ammoConsumed' was missing in WeaponType");
    	    }
            if(auto ptr = obj.if_contains("baseDamage")) {
	            if(auto tPtr = ptr->if_object()) {
   				    std::map<std::string, int64_t> map;
   				    for(auto const& entry : *tPtr) {
   				        if(auto mPtr = entry.value().if_int64()) {
   				            map[entry.key_c_str()] = int64_t{*mPtr};
   				        } else {
   				            throw std::runtime_error("Entry in map 'baseDamage' in WeaponType was expected to be int64, but was of wrong type.");
   				        }
   				    }
   				    baseDamage = std::move(map);
   				} else if(!ptr->is_null()) {
   				    throw std::runtime_error("Expected 'baseDamage' as object in WeaponType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("maxRange")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                maxRange = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'maxRange' as int64_t in WeaponType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'maxRange' was missing in WeaponType");
    	    }
            if(auto ptr = obj.if_contains("minRange")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                minRange = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'minRange' as int64_t in WeaponType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("selfTarget")) {
	            if(auto tPtr = ptr->if_bool()) {
   	                selfTarget = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'selfTarget' as bool in WeaponType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("affectedByLuck")) {
	            if(auto tPtr = ptr->if_bool()) {
   	                affectedByLuck = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'affectedByLuck' as bool in WeaponType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("nonLethal")) {
	            if(auto tPtr = ptr->if_bool()) {
   	                nonLethal = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'nonLethal' as bool in WeaponType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("areaOfEffect")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                areaOfEffect = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'areaOfEffect' as int64_t in WeaponType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("targetsStealth")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'targetsStealth' in WeaponType was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    targetsStealth = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'targetsStealth' as array in WeaponType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("flatDamage")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                flatDamage = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'flatDamage' as int64_t in WeaponType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
        }

        void writeTo(json::object & obj) const {
           	obj["name"] = name;
           	obj["ammoConsumed"] = ammoConsumed;
      		if(baseDamage) {
   			    json::object m;
      		    for(auto const& entry : *baseDamage) {
   			        m[entry.first] = entry.second;
   			    }
   			    obj["baseDamage"] = std::move(m);
   			}
           	obj["maxRange"] = maxRange;
            if(minRange) {
          	    obj["minRange"] = *minRange;
          	}
            if(selfTarget) {
          	    obj["selfTarget"] = *selfTarget;
          	}
            if(affectedByLuck) {
          	    obj["affectedByLuck"] = *affectedByLuck;
          	}
            if(nonLethal) {
          	    obj["nonLethal"] = *nonLethal;
          	}
            if(areaOfEffect) {
          	    obj["areaOfEffect"] = *areaOfEffect;
          	}
       		if(targetsStealth) {
    		    json::array arr;
       		    for(auto const& val : *targetsStealth) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["targetsStealth"] = std::move(arr);
    		}
            if(flatDamage) {
          	    obj["flatDamage"] = *flatDamage;
          	}
        }
    };
}
