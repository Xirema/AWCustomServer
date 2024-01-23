#pragma once
#include<jss/jssinclude.h>

namespace dTypes{
    struct PassiveUnitEffect{
        std::string name;
        std::optional<std::vector<std::string>> targets;
        std::optional<std::vector<std::string>> unitTypeRequired;
        std::optional<std::vector<std::string>> classificationRequired;
        std::optional<std::vector<std::string>> terrainRequired;
        std::optional<int64_t> firepowerMod;
        std::optional<int64_t> defenseMod;
        std::optional<int64_t> indirectDefenseMod;
        std::optional<int64_t> minRangeMod;
        std::optional<int64_t> maxRangeMod;
        std::optional<int64_t> fuelUseMod;
        std::optional<int64_t> ammoUseMod;
        std::optional<int64_t> goodLuckMod;
        std::optional<int64_t> badLuckMod;
        std::optional<int64_t> movementMod;
        std::optional<int64_t> visionMod;
        std::optional<int64_t> terrainStarsMod;
        std::optional<int64_t> terrainStarsFlatMod;
        std::optional<int64_t> terrainStarsDefense;
        std::optional<int64_t> terrainStarsFirepower;
        std::optional<int64_t> counterfireMod;
        std::optional<bool> counterFirst;
        std::optional<int64_t> captureRateMod;
        std::optional<int64_t> unitCostMod;
        std::optional<std::vector<std::string>> hiddenHitPoints;
        std::optional<std::vector<std::string>> luckPointsVisible;
        std::optional<std::vector<std::string>> hpPartVisible;
        std::optional<int64_t> firepowerFromFunds;
        std::optional<int64_t> defenseFromFunds;
        std::optional<int64_t> fundsFromDamage;
        std::optional<std::map<std::string, int64_t>> firepowerFromOwnedTerrain;
        std::optional<std::map<std::string, int64_t>> defenseFromOwnedTerrain;
        std::optional<std::map<std::string, int64_t>> visionVariantMods;
        std::optional<std::map<std::string, int64_t>> firepowerVariantMods;
        std::optional<std::map<std::string, int64_t>> defenseVariantMods;
        std::optional<int64_t> coMeterChargeFromDealtDamage;
        std::optional<int64_t> coMeterChargeFromReceivedDamage;

        void readFrom(json::object const& obj) {
            if(auto ptr = obj.if_contains("name")) {
	            if(auto tPtr = ptr->if_string()) {
   	                name = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'name' as std::string in PassiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'name' was missing in PassiveUnitEffect");
    	    }
            if(auto ptr = obj.if_contains("targets")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'targets' in PassiveUnitEffect was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    targets = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'targets' as array in PassiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("unitTypeRequired")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'unitTypeRequired' in PassiveUnitEffect was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    unitTypeRequired = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'unitTypeRequired' as array in PassiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("classificationRequired")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'classificationRequired' in PassiveUnitEffect was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    classificationRequired = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'classificationRequired' as array in PassiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("terrainRequired")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'terrainRequired' in PassiveUnitEffect was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    terrainRequired = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'terrainRequired' as array in PassiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("firepowerMod")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                firepowerMod = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'firepowerMod' as int64_t in PassiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("defenseMod")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                defenseMod = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'defenseMod' as int64_t in PassiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("indirectDefenseMod")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                indirectDefenseMod = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'indirectDefenseMod' as int64_t in PassiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("minRangeMod")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                minRangeMod = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'minRangeMod' as int64_t in PassiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("maxRangeMod")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                maxRangeMod = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'maxRangeMod' as int64_t in PassiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("fuelUseMod")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                fuelUseMod = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'fuelUseMod' as int64_t in PassiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("ammoUseMod")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                ammoUseMod = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'ammoUseMod' as int64_t in PassiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("goodLuckMod")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                goodLuckMod = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'goodLuckMod' as int64_t in PassiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("badLuckMod")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                badLuckMod = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'badLuckMod' as int64_t in PassiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("movementMod")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                movementMod = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'movementMod' as int64_t in PassiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("visionMod")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                visionMod = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'visionMod' as int64_t in PassiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("terrainStarsMod")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                terrainStarsMod = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'terrainStarsMod' as int64_t in PassiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("terrainStarsFlatMod")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                terrainStarsFlatMod = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'terrainStarsFlatMod' as int64_t in PassiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("terrainStarsDefense")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                terrainStarsDefense = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'terrainStarsDefense' as int64_t in PassiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("terrainStarsFirepower")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                terrainStarsFirepower = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'terrainStarsFirepower' as int64_t in PassiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("counterfireMod")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                counterfireMod = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'counterfireMod' as int64_t in PassiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("counterFirst")) {
	            if(auto tPtr = ptr->if_bool()) {
   	                counterFirst = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'counterFirst' as bool in PassiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("captureRateMod")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                captureRateMod = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'captureRateMod' as int64_t in PassiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("unitCostMod")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                unitCostMod = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'unitCostMod' as int64_t in PassiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("hiddenHitPoints")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'hiddenHitPoints' in PassiveUnitEffect was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    hiddenHitPoints = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'hiddenHitPoints' as array in PassiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("luckPointsVisible")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'luckPointsVisible' in PassiveUnitEffect was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    luckPointsVisible = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'luckPointsVisible' as array in PassiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("hpPartVisible")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'hpPartVisible' in PassiveUnitEffect was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    hpPartVisible = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'hpPartVisible' as array in PassiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("firepowerFromFunds")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                firepowerFromFunds = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'firepowerFromFunds' as int64_t in PassiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("defenseFromFunds")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                defenseFromFunds = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'defenseFromFunds' as int64_t in PassiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("fundsFromDamage")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                fundsFromDamage = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'fundsFromDamage' as int64_t in PassiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("firepowerFromOwnedTerrain")) {
	            if(auto tPtr = ptr->if_object()) {
   				    std::map<std::string, int64_t> map;
   				    for(auto const& entry : *tPtr) {
   				        if(auto mPtr = entry.value().if_int64()) {
   				            map[entry.key_c_str()] = int64_t{*mPtr};
   				        } else {
   				            throw std::runtime_error("Entry in map 'firepowerFromOwnedTerrain' in PassiveUnitEffect was expected to be int64, but was of wrong type.");
   				        }
   				    }
   				    firepowerFromOwnedTerrain = std::move(map);
   				} else if(!ptr->is_null()) {
   				    throw std::runtime_error("Expected 'firepowerFromOwnedTerrain' as object in PassiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("defenseFromOwnedTerrain")) {
	            if(auto tPtr = ptr->if_object()) {
   				    std::map<std::string, int64_t> map;
   				    for(auto const& entry : *tPtr) {
   				        if(auto mPtr = entry.value().if_int64()) {
   				            map[entry.key_c_str()] = int64_t{*mPtr};
   				        } else {
   				            throw std::runtime_error("Entry in map 'defenseFromOwnedTerrain' in PassiveUnitEffect was expected to be int64, but was of wrong type.");
   				        }
   				    }
   				    defenseFromOwnedTerrain = std::move(map);
   				} else if(!ptr->is_null()) {
   				    throw std::runtime_error("Expected 'defenseFromOwnedTerrain' as object in PassiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("visionVariantMods")) {
	            if(auto tPtr = ptr->if_object()) {
   				    std::map<std::string, int64_t> map;
   				    for(auto const& entry : *tPtr) {
   				        if(auto mPtr = entry.value().if_int64()) {
   				            map[entry.key_c_str()] = int64_t{*mPtr};
   				        } else {
   				            throw std::runtime_error("Entry in map 'visionVariantMods' in PassiveUnitEffect was expected to be int64, but was of wrong type.");
   				        }
   				    }
   				    visionVariantMods = std::move(map);
   				} else if(!ptr->is_null()) {
   				    throw std::runtime_error("Expected 'visionVariantMods' as object in PassiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("firepowerVariantMods")) {
	            if(auto tPtr = ptr->if_object()) {
   				    std::map<std::string, int64_t> map;
   				    for(auto const& entry : *tPtr) {
   				        if(auto mPtr = entry.value().if_int64()) {
   				            map[entry.key_c_str()] = int64_t{*mPtr};
   				        } else {
   				            throw std::runtime_error("Entry in map 'firepowerVariantMods' in PassiveUnitEffect was expected to be int64, but was of wrong type.");
   				        }
   				    }
   				    firepowerVariantMods = std::move(map);
   				} else if(!ptr->is_null()) {
   				    throw std::runtime_error("Expected 'firepowerVariantMods' as object in PassiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("defenseVariantMods")) {
	            if(auto tPtr = ptr->if_object()) {
   				    std::map<std::string, int64_t> map;
   				    for(auto const& entry : *tPtr) {
   				        if(auto mPtr = entry.value().if_int64()) {
   				            map[entry.key_c_str()] = int64_t{*mPtr};
   				        } else {
   				            throw std::runtime_error("Entry in map 'defenseVariantMods' in PassiveUnitEffect was expected to be int64, but was of wrong type.");
   				        }
   				    }
   				    defenseVariantMods = std::move(map);
   				} else if(!ptr->is_null()) {
   				    throw std::runtime_error("Expected 'defenseVariantMods' as object in PassiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("coMeterChargeFromDealtDamage")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                coMeterChargeFromDealtDamage = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'coMeterChargeFromDealtDamage' as int64_t in PassiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("coMeterChargeFromReceivedDamage")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                coMeterChargeFromReceivedDamage = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'coMeterChargeFromReceivedDamage' as int64_t in PassiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
        }

        void writeTo(json::object & obj) const {
           	obj["name"] = name;
       		if(targets) {
    		    json::array arr;
       		    for(auto const& val : *targets) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["targets"] = std::move(arr);
    		}
       		if(unitTypeRequired) {
    		    json::array arr;
       		    for(auto const& val : *unitTypeRequired) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["unitTypeRequired"] = std::move(arr);
    		}
       		if(classificationRequired) {
    		    json::array arr;
       		    for(auto const& val : *classificationRequired) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["classificationRequired"] = std::move(arr);
    		}
       		if(terrainRequired) {
    		    json::array arr;
       		    for(auto const& val : *terrainRequired) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["terrainRequired"] = std::move(arr);
    		}
            if(firepowerMod) {
          	    obj["firepowerMod"] = *firepowerMod;
          	}
            if(defenseMod) {
          	    obj["defenseMod"] = *defenseMod;
          	}
            if(indirectDefenseMod) {
          	    obj["indirectDefenseMod"] = *indirectDefenseMod;
          	}
            if(minRangeMod) {
          	    obj["minRangeMod"] = *minRangeMod;
          	}
            if(maxRangeMod) {
          	    obj["maxRangeMod"] = *maxRangeMod;
          	}
            if(fuelUseMod) {
          	    obj["fuelUseMod"] = *fuelUseMod;
          	}
            if(ammoUseMod) {
          	    obj["ammoUseMod"] = *ammoUseMod;
          	}
            if(goodLuckMod) {
          	    obj["goodLuckMod"] = *goodLuckMod;
          	}
            if(badLuckMod) {
          	    obj["badLuckMod"] = *badLuckMod;
          	}
            if(movementMod) {
          	    obj["movementMod"] = *movementMod;
          	}
            if(visionMod) {
          	    obj["visionMod"] = *visionMod;
          	}
            if(terrainStarsMod) {
          	    obj["terrainStarsMod"] = *terrainStarsMod;
          	}
            if(terrainStarsFlatMod) {
          	    obj["terrainStarsFlatMod"] = *terrainStarsFlatMod;
          	}
            if(terrainStarsDefense) {
          	    obj["terrainStarsDefense"] = *terrainStarsDefense;
          	}
            if(terrainStarsFirepower) {
          	    obj["terrainStarsFirepower"] = *terrainStarsFirepower;
          	}
            if(counterfireMod) {
          	    obj["counterfireMod"] = *counterfireMod;
          	}
            if(counterFirst) {
          	    obj["counterFirst"] = *counterFirst;
          	}
            if(captureRateMod) {
          	    obj["captureRateMod"] = *captureRateMod;
          	}
            if(unitCostMod) {
          	    obj["unitCostMod"] = *unitCostMod;
          	}
       		if(hiddenHitPoints) {
    		    json::array arr;
       		    for(auto const& val : *hiddenHitPoints) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["hiddenHitPoints"] = std::move(arr);
    		}
       		if(luckPointsVisible) {
    		    json::array arr;
       		    for(auto const& val : *luckPointsVisible) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["luckPointsVisible"] = std::move(arr);
    		}
       		if(hpPartVisible) {
    		    json::array arr;
       		    for(auto const& val : *hpPartVisible) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["hpPartVisible"] = std::move(arr);
    		}
            if(firepowerFromFunds) {
          	    obj["firepowerFromFunds"] = *firepowerFromFunds;
          	}
            if(defenseFromFunds) {
          	    obj["defenseFromFunds"] = *defenseFromFunds;
          	}
            if(fundsFromDamage) {
          	    obj["fundsFromDamage"] = *fundsFromDamage;
          	}
      		if(firepowerFromOwnedTerrain) {
   			    json::object m;
      		    for(auto const& entry : *firepowerFromOwnedTerrain) {
   			        m[entry.first] = entry.second;
   			    }
   			    obj["firepowerFromOwnedTerrain"] = std::move(m);
   			}
      		if(defenseFromOwnedTerrain) {
   			    json::object m;
      		    for(auto const& entry : *defenseFromOwnedTerrain) {
   			        m[entry.first] = entry.second;
   			    }
   			    obj["defenseFromOwnedTerrain"] = std::move(m);
   			}
      		if(visionVariantMods) {
   			    json::object m;
      		    for(auto const& entry : *visionVariantMods) {
   			        m[entry.first] = entry.second;
   			    }
   			    obj["visionVariantMods"] = std::move(m);
   			}
      		if(firepowerVariantMods) {
   			    json::object m;
      		    for(auto const& entry : *firepowerVariantMods) {
   			        m[entry.first] = entry.second;
   			    }
   			    obj["firepowerVariantMods"] = std::move(m);
   			}
      		if(defenseVariantMods) {
   			    json::object m;
      		    for(auto const& entry : *defenseVariantMods) {
   			        m[entry.first] = entry.second;
   			    }
   			    obj["defenseVariantMods"] = std::move(m);
   			}
            if(coMeterChargeFromDealtDamage) {
          	    obj["coMeterChargeFromDealtDamage"] = *coMeterChargeFromDealtDamage;
          	}
            if(coMeterChargeFromReceivedDamage) {
          	    obj["coMeterChargeFromReceivedDamage"] = *coMeterChargeFromReceivedDamage;
          	}
        }
    };
    struct ActiveUnitEffect{
        std::string name;
        std::optional<std::vector<std::string>> targets;
        std::optional<std::vector<std::string>> unitTypeRequired;
        std::optional<std::vector<std::string>> classificationRequired;
        std::optional<std::vector<std::string>> terrainRequired;
        std::optional<int64_t> hitPointMod;
        std::optional<int64_t> roundHitPoints;
        std::optional<bool> resupply;
        std::optional<bool> halveFuel;
        std::optional<bool> makeActive;
        std::optional<int64_t> stunDuration;
        std::optional<int64_t> coChargeFactor;

        void readFrom(json::object const& obj) {
            if(auto ptr = obj.if_contains("name")) {
	            if(auto tPtr = ptr->if_string()) {
   	                name = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'name' as std::string in ActiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'name' was missing in ActiveUnitEffect");
    	    }
            if(auto ptr = obj.if_contains("targets")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'targets' in ActiveUnitEffect was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    targets = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'targets' as array in ActiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("unitTypeRequired")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'unitTypeRequired' in ActiveUnitEffect was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    unitTypeRequired = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'unitTypeRequired' as array in ActiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("classificationRequired")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'classificationRequired' in ActiveUnitEffect was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    classificationRequired = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'classificationRequired' as array in ActiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("terrainRequired")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'terrainRequired' in ActiveUnitEffect was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    terrainRequired = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'terrainRequired' as array in ActiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("hitPointMod")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                hitPointMod = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'hitPointMod' as int64_t in ActiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("roundHitPoints")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                roundHitPoints = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'roundHitPoints' as int64_t in ActiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("resupply")) {
	            if(auto tPtr = ptr->if_bool()) {
   	                resupply = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'resupply' as bool in ActiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("halveFuel")) {
	            if(auto tPtr = ptr->if_bool()) {
   	                halveFuel = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'halveFuel' as bool in ActiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("makeActive")) {
	            if(auto tPtr = ptr->if_bool()) {
   	                makeActive = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'makeActive' as bool in ActiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("stunDuration")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                stunDuration = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'stunDuration' as int64_t in ActiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("coChargeFactor")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                coChargeFactor = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'coChargeFactor' as int64_t in ActiveUnitEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
        }

        void writeTo(json::object & obj) const {
           	obj["name"] = name;
       		if(targets) {
    		    json::array arr;
       		    for(auto const& val : *targets) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["targets"] = std::move(arr);
    		}
       		if(unitTypeRequired) {
    		    json::array arr;
       		    for(auto const& val : *unitTypeRequired) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["unitTypeRequired"] = std::move(arr);
    		}
       		if(classificationRequired) {
    		    json::array arr;
       		    for(auto const& val : *classificationRequired) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["classificationRequired"] = std::move(arr);
    		}
       		if(terrainRequired) {
    		    json::array arr;
       		    for(auto const& val : *terrainRequired) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["terrainRequired"] = std::move(arr);
    		}
            if(hitPointMod) {
          	    obj["hitPointMod"] = *hitPointMod;
          	}
            if(roundHitPoints) {
          	    obj["roundHitPoints"] = *roundHitPoints;
          	}
            if(resupply) {
          	    obj["resupply"] = *resupply;
          	}
            if(halveFuel) {
          	    obj["halveFuel"] = *halveFuel;
          	}
            if(makeActive) {
          	    obj["makeActive"] = *makeActive;
          	}
            if(stunDuration) {
          	    obj["stunDuration"] = *stunDuration;
          	}
            if(coChargeFactor) {
          	    obj["coChargeFactor"] = *coChargeFactor;
          	}
        }
    };
    struct PassiveTerrainEffect{
        std::string name;
        std::optional<std::vector<std::string>> targets;
        std::optional<std::vector<std::string>> affects;
        std::optional<std::vector<std::string>> terrainRequired;
        std::optional<std::vector<std::string>> classificationRequired;
        std::optional<int64_t> incomeMod;
        std::optional<int64_t> incomeFlatMod;
        std::optional<std::vector<std::string>> buildListMod;
        std::optional<int64_t> repairMod;
        std::optional<bool> occludesVisionMod;
        std::optional<int64_t> visionModBoost;
        std::optional<int64_t> buildCostMod;

        void readFrom(json::object const& obj) {
            if(auto ptr = obj.if_contains("name")) {
	            if(auto tPtr = ptr->if_string()) {
   	                name = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'name' as std::string in PassiveTerrainEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'name' was missing in PassiveTerrainEffect");
    	    }
            if(auto ptr = obj.if_contains("targets")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'targets' in PassiveTerrainEffect was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    targets = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'targets' as array in PassiveTerrainEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("affects")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'affects' in PassiveTerrainEffect was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    affects = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'affects' as array in PassiveTerrainEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("terrainRequired")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'terrainRequired' in PassiveTerrainEffect was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    terrainRequired = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'terrainRequired' as array in PassiveTerrainEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("classificationRequired")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'classificationRequired' in PassiveTerrainEffect was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    classificationRequired = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'classificationRequired' as array in PassiveTerrainEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("incomeMod")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                incomeMod = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'incomeMod' as int64_t in PassiveTerrainEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("incomeFlatMod")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                incomeFlatMod = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'incomeFlatMod' as int64_t in PassiveTerrainEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("buildListMod")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'buildListMod' in PassiveTerrainEffect was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    buildListMod = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'buildListMod' as array in PassiveTerrainEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("repairMod")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                repairMod = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'repairMod' as int64_t in PassiveTerrainEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("occludesVisionMod")) {
	            if(auto tPtr = ptr->if_bool()) {
   	                occludesVisionMod = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'occludesVisionMod' as bool in PassiveTerrainEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("visionModBoost")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                visionModBoost = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'visionModBoost' as int64_t in PassiveTerrainEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("buildCostMod")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                buildCostMod = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'buildCostMod' as int64_t in PassiveTerrainEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
        }

        void writeTo(json::object & obj) const {
           	obj["name"] = name;
       		if(targets) {
    		    json::array arr;
       		    for(auto const& val : *targets) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["targets"] = std::move(arr);
    		}
       		if(affects) {
    		    json::array arr;
       		    for(auto const& val : *affects) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["affects"] = std::move(arr);
    		}
       		if(terrainRequired) {
    		    json::array arr;
       		    for(auto const& val : *terrainRequired) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["terrainRequired"] = std::move(arr);
    		}
       		if(classificationRequired) {
    		    json::array arr;
       		    for(auto const& val : *classificationRequired) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["classificationRequired"] = std::move(arr);
    		}
            if(incomeMod) {
          	    obj["incomeMod"] = *incomeMod;
          	}
            if(incomeFlatMod) {
          	    obj["incomeFlatMod"] = *incomeFlatMod;
          	}
       		if(buildListMod) {
    		    json::array arr;
       		    for(auto const& val : *buildListMod) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["buildListMod"] = std::move(arr);
    		}
            if(repairMod) {
          	    obj["repairMod"] = *repairMod;
          	}
            if(occludesVisionMod) {
          	    obj["occludesVisionMod"] = *occludesVisionMod;
          	}
            if(visionModBoost) {
          	    obj["visionModBoost"] = *visionModBoost;
          	}
            if(buildCostMod) {
          	    obj["buildCostMod"] = *buildCostMod;
          	}
        }
    };
    struct ActiveTerrainEffect{
        std::string name;
        std::optional<std::vector<std::string>> targets;
        std::optional<std::vector<std::string>> affects;
        std::optional<std::vector<std::string>> terrainRequired;
        std::optional<std::string> unitSummonedName;
        std::optional<int64_t> unitSummonedInitialDamage;
        std::optional<bool> unitSummonedActive;

        void readFrom(json::object const& obj) {
            if(auto ptr = obj.if_contains("name")) {
	            if(auto tPtr = ptr->if_string()) {
   	                name = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'name' as std::string in ActiveTerrainEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'name' was missing in ActiveTerrainEffect");
    	    }
            if(auto ptr = obj.if_contains("targets")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'targets' in ActiveTerrainEffect was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    targets = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'targets' as array in ActiveTerrainEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("affects")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'affects' in ActiveTerrainEffect was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    affects = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'affects' as array in ActiveTerrainEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("terrainRequired")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'terrainRequired' in ActiveTerrainEffect was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    terrainRequired = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'terrainRequired' as array in ActiveTerrainEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("unitSummonedName")) {
	            if(auto tPtr = ptr->if_string()) {
   	                unitSummonedName = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'unitSummonedName' as std::string in ActiveTerrainEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("unitSummonedInitialDamage")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                unitSummonedInitialDamage = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'unitSummonedInitialDamage' as int64_t in ActiveTerrainEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("unitSummonedActive")) {
	            if(auto tPtr = ptr->if_bool()) {
   	                unitSummonedActive = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'unitSummonedActive' as bool in ActiveTerrainEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
        }

        void writeTo(json::object & obj) const {
           	obj["name"] = name;
       		if(targets) {
    		    json::array arr;
       		    for(auto const& val : *targets) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["targets"] = std::move(arr);
    		}
       		if(affects) {
    		    json::array arr;
       		    for(auto const& val : *affects) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["affects"] = std::move(arr);
    		}
       		if(terrainRequired) {
    		    json::array arr;
       		    for(auto const& val : *terrainRequired) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["terrainRequired"] = std::move(arr);
    		}
            if(unitSummonedName) {
          	    obj["unitSummonedName"] = *unitSummonedName;
          	}
            if(unitSummonedInitialDamage) {
          	    obj["unitSummonedInitialDamage"] = *unitSummonedInitialDamage;
          	}
            if(unitSummonedActive) {
          	    obj["unitSummonedActive"] = *unitSummonedActive;
          	}
        }
    };
    struct PassiveGlobalEffect{
        std::string name;
        std::optional<std::vector<std::string>> targets;
        std::optional<std::string> variantMod;
        std::optional<std::map<std::string, int64_t>> variantHintMod;
        std::optional<std::string> movementClassVariantReplace;
        std::optional<std::string> movementClassVariantOverride;
        std::optional<int64_t> minimumVisionMod;

        void readFrom(json::object const& obj) {
            if(auto ptr = obj.if_contains("name")) {
	            if(auto tPtr = ptr->if_string()) {
   	                name = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'name' as std::string in PassiveGlobalEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'name' was missing in PassiveGlobalEffect");
    	    }
            if(auto ptr = obj.if_contains("targets")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'targets' in PassiveGlobalEffect was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    targets = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'targets' as array in PassiveGlobalEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("variantMod")) {
	            if(auto tPtr = ptr->if_string()) {
   	                variantMod = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'variantMod' as std::string in PassiveGlobalEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("variantHintMod")) {
	            if(auto tPtr = ptr->if_object()) {
   				    std::map<std::string, int64_t> map;
   				    for(auto const& entry : *tPtr) {
   				        if(auto mPtr = entry.value().if_int64()) {
   				            map[entry.key_c_str()] = int64_t{*mPtr};
   				        } else {
   				            throw std::runtime_error("Entry in map 'variantHintMod' in PassiveGlobalEffect was expected to be int64, but was of wrong type.");
   				        }
   				    }
   				    variantHintMod = std::move(map);
   				} else if(!ptr->is_null()) {
   				    throw std::runtime_error("Expected 'variantHintMod' as object in PassiveGlobalEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("movementClassVariantReplace")) {
	            if(auto tPtr = ptr->if_string()) {
   	                movementClassVariantReplace = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'movementClassVariantReplace' as std::string in PassiveGlobalEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("movementClassVariantOverride")) {
	            if(auto tPtr = ptr->if_string()) {
   	                movementClassVariantOverride = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'movementClassVariantOverride' as std::string in PassiveGlobalEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("minimumVisionMod")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                minimumVisionMod = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'minimumVisionMod' as int64_t in PassiveGlobalEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
        }

        void writeTo(json::object & obj) const {
           	obj["name"] = name;
       		if(targets) {
    		    json::array arr;
       		    for(auto const& val : *targets) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["targets"] = std::move(arr);
    		}
            if(variantMod) {
          	    obj["variantMod"] = *variantMod;
          	}
      		if(variantHintMod) {
   			    json::object m;
      		    for(auto const& entry : *variantHintMod) {
   			        m[entry.first] = entry.second;
   			    }
   			    obj["variantHintMod"] = std::move(m);
   			}
            if(movementClassVariantReplace) {
          	    obj["movementClassVariantReplace"] = *movementClassVariantReplace;
          	}
            if(movementClassVariantOverride) {
          	    obj["movementClassVariantOverride"] = *movementClassVariantOverride;
          	}
            if(minimumVisionMod) {
          	    obj["minimumVisionMod"] = *minimumVisionMod;
          	}
        }
    };
    struct ActiveGlobalEffect{
        std::string name;
        std::optional<std::vector<std::string>> targets;
        std::optional<int64_t> fundMod;
        std::optional<int64_t> fundFlatMod;
        std::optional<int64_t> powerBarMod;
        std::optional<int64_t> powerBarPerFunds;
        std::optional<int64_t> missileCount;
        std::optional<std::vector<std::string>> missileTargetMethod;
        std::optional<int64_t> missileDamage;
        std::optional<int64_t> missileAreaOfEffect;
        std::optional<int64_t> missileStunDuration;
        std::optional<int64_t> coChargeFactor;

        void readFrom(json::object const& obj) {
            if(auto ptr = obj.if_contains("name")) {
	            if(auto tPtr = ptr->if_string()) {
   	                name = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'name' as std::string in ActiveGlobalEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'name' was missing in ActiveGlobalEffect");
    	    }
            if(auto ptr = obj.if_contains("targets")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'targets' in ActiveGlobalEffect was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    targets = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'targets' as array in ActiveGlobalEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("fundMod")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                fundMod = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'fundMod' as int64_t in ActiveGlobalEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("fundFlatMod")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                fundFlatMod = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'fundFlatMod' as int64_t in ActiveGlobalEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("powerBarMod")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                powerBarMod = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'powerBarMod' as int64_t in ActiveGlobalEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("powerBarPerFunds")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                powerBarPerFunds = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'powerBarPerFunds' as int64_t in ActiveGlobalEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("missileCount")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                missileCount = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'missileCount' as int64_t in ActiveGlobalEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("missileTargetMethod")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'missileTargetMethod' in ActiveGlobalEffect was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    missileTargetMethod = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'missileTargetMethod' as array in ActiveGlobalEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("missileDamage")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                missileDamage = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'missileDamage' as int64_t in ActiveGlobalEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("missileAreaOfEffect")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                missileAreaOfEffect = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'missileAreaOfEffect' as int64_t in ActiveGlobalEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("missileStunDuration")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                missileStunDuration = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'missileStunDuration' as int64_t in ActiveGlobalEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("coChargeFactor")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                coChargeFactor = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'coChargeFactor' as int64_t in ActiveGlobalEffect, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
        }

        void writeTo(json::object & obj) const {
           	obj["name"] = name;
       		if(targets) {
    		    json::array arr;
       		    for(auto const& val : *targets) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["targets"] = std::move(arr);
    		}
            if(fundMod) {
          	    obj["fundMod"] = *fundMod;
          	}
            if(fundFlatMod) {
          	    obj["fundFlatMod"] = *fundFlatMod;
          	}
            if(powerBarMod) {
          	    obj["powerBarMod"] = *powerBarMod;
          	}
            if(powerBarPerFunds) {
          	    obj["powerBarPerFunds"] = *powerBarPerFunds;
          	}
            if(missileCount) {
          	    obj["missileCount"] = *missileCount;
          	}
       		if(missileTargetMethod) {
    		    json::array arr;
       		    for(auto const& val : *missileTargetMethod) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["missileTargetMethod"] = std::move(arr);
    		}
            if(missileDamage) {
          	    obj["missileDamage"] = *missileDamage;
          	}
            if(missileAreaOfEffect) {
          	    obj["missileAreaOfEffect"] = *missileAreaOfEffect;
          	}
            if(missileStunDuration) {
          	    obj["missileStunDuration"] = *missileStunDuration;
          	}
            if(coChargeFactor) {
          	    obj["coChargeFactor"] = *coChargeFactor;
          	}
        }
    };
}
