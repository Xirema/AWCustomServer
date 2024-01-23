#pragma once
#include<jss/jssinclude.h>

namespace dTypes{
    struct TerrainType{
        std::string name;
        int64_t stars;
        std::optional<int64_t> maxCapturePoints;
        std::optional<std::string> sameAs;
        std::optional<std::vector<std::string>> buildList;
        std::optional<int64_t> income;
        std::optional<int64_t> repair;
        std::optional<std::vector<std::string>> repairList;
        std::optional<bool> occludesVision;
        std::optional<int64_t> hitPoints;
        std::optional<std::string> destroyed;
        std::optional<int64_t> destroyedOrientation;
        std::optional<std::string> damagedLike;
        std::optional<std::vector<std::string>> activateList;
        std::optional<std::vector<std::string>> activateActiveUnitEffects;
        std::optional<std::vector<std::string>> activateActiveTerrainEffects;
        std::optional<std::vector<std::string>> activateActiveGlobalEffects;
        std::optional<int64_t> activationMax;
        std::optional<std::string> activationChange;
        std::optional<bool> loseIfCaptured;
        std::optional<bool> loseIfAllCaptured;

        void readFrom(json::object const& obj) {
            if(auto ptr = obj.if_contains("name")) {
	            if(auto tPtr = ptr->if_string()) {
   	                name = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'name' as std::string in TerrainType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'name' was missing in TerrainType");
    	    }
            if(auto ptr = obj.if_contains("stars")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                stars = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'stars' as int64_t in TerrainType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'stars' was missing in TerrainType");
    	    }
            if(auto ptr = obj.if_contains("maxCapturePoints")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                maxCapturePoints = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'maxCapturePoints' as int64_t in TerrainType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("sameAs")) {
	            if(auto tPtr = ptr->if_string()) {
   	                sameAs = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'sameAs' as std::string in TerrainType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("buildList")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'buildList' in TerrainType was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    buildList = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'buildList' as array in TerrainType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("income")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                income = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'income' as int64_t in TerrainType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("repair")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                repair = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'repair' as int64_t in TerrainType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("repairList")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'repairList' in TerrainType was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    repairList = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'repairList' as array in TerrainType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("occludesVision")) {
	            if(auto tPtr = ptr->if_bool()) {
   	                occludesVision = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'occludesVision' as bool in TerrainType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("hitPoints")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                hitPoints = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'hitPoints' as int64_t in TerrainType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("destroyed")) {
	            if(auto tPtr = ptr->if_string()) {
   	                destroyed = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'destroyed' as std::string in TerrainType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("destroyedOrientation")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                destroyedOrientation = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'destroyedOrientation' as int64_t in TerrainType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("damagedLike")) {
	            if(auto tPtr = ptr->if_string()) {
   	                damagedLike = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'damagedLike' as std::string in TerrainType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("activateList")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'activateList' in TerrainType was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    activateList = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'activateList' as array in TerrainType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("activateActiveUnitEffects")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'activateActiveUnitEffects' in TerrainType was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    activateActiveUnitEffects = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'activateActiveUnitEffects' as array in TerrainType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("activateActiveTerrainEffects")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'activateActiveTerrainEffects' in TerrainType was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    activateActiveTerrainEffects = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'activateActiveTerrainEffects' as array in TerrainType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("activateActiveGlobalEffects")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'activateActiveGlobalEffects' in TerrainType was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    activateActiveGlobalEffects = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'activateActiveGlobalEffects' as array in TerrainType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("activationMax")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                activationMax = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'activationMax' as int64_t in TerrainType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("activationChange")) {
	            if(auto tPtr = ptr->if_string()) {
   	                activationChange = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'activationChange' as std::string in TerrainType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("loseIfCaptured")) {
	            if(auto tPtr = ptr->if_bool()) {
   	                loseIfCaptured = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'loseIfCaptured' as bool in TerrainType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("loseIfAllCaptured")) {
	            if(auto tPtr = ptr->if_bool()) {
   	                loseIfAllCaptured = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'loseIfAllCaptured' as bool in TerrainType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
        }

        void writeTo(json::object & obj) const {
           	obj["name"] = name;
           	obj["stars"] = stars;
            if(maxCapturePoints) {
          	    obj["maxCapturePoints"] = *maxCapturePoints;
          	}
            if(sameAs) {
          	    obj["sameAs"] = *sameAs;
          	}
       		if(buildList) {
    		    json::array arr;
       		    for(auto const& val : *buildList) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["buildList"] = std::move(arr);
    		}
            if(income) {
          	    obj["income"] = *income;
          	}
            if(repair) {
          	    obj["repair"] = *repair;
          	}
       		if(repairList) {
    		    json::array arr;
       		    for(auto const& val : *repairList) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["repairList"] = std::move(arr);
    		}
            if(occludesVision) {
          	    obj["occludesVision"] = *occludesVision;
          	}
            if(hitPoints) {
          	    obj["hitPoints"] = *hitPoints;
          	}
            if(destroyed) {
          	    obj["destroyed"] = *destroyed;
          	}
            if(destroyedOrientation) {
          	    obj["destroyedOrientation"] = *destroyedOrientation;
          	}
            if(damagedLike) {
          	    obj["damagedLike"] = *damagedLike;
          	}
       		if(activateList) {
    		    json::array arr;
       		    for(auto const& val : *activateList) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["activateList"] = std::move(arr);
    		}
       		if(activateActiveUnitEffects) {
    		    json::array arr;
       		    for(auto const& val : *activateActiveUnitEffects) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["activateActiveUnitEffects"] = std::move(arr);
    		}
       		if(activateActiveTerrainEffects) {
    		    json::array arr;
       		    for(auto const& val : *activateActiveTerrainEffects) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["activateActiveTerrainEffects"] = std::move(arr);
    		}
       		if(activateActiveGlobalEffects) {
    		    json::array arr;
       		    for(auto const& val : *activateActiveGlobalEffects) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["activateActiveGlobalEffects"] = std::move(arr);
    		}
            if(activationMax) {
          	    obj["activationMax"] = *activationMax;
          	}
            if(activationChange) {
          	    obj["activationChange"] = *activationChange;
          	}
            if(loseIfCaptured) {
          	    obj["loseIfCaptured"] = *loseIfCaptured;
          	}
            if(loseIfAllCaptured) {
          	    obj["loseIfAllCaptured"] = *loseIfAllCaptured;
          	}
        }
    };
}
