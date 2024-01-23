#pragma once
#include<jss/jssinclude.h>

namespace dTypes{
    struct MapType{
        std::string name;
        std::optional<std::vector<std::string>> playerSlots;
        { initialTerrains;
        std::string terrainName;
        { coordinates;
        int64_t x;
        int64_t y;
        std::optional<int64_t> orientation;
        std::optional<std::string> playerSlot;
        std::optional<{[key:string]:any}> terrainMods;
        std::optional<{> initialUnits;
        std::string unitName;
        { coordinates;
        int64_t x;
        int64_t y;
        std::optional<std::string> playerSlot;
        std::optional<{[key:string]:any}> unitMods;

        void readFrom(json::object const& obj) {
            if(auto ptr = obj.if_contains("name")) {
	            if(auto tPtr = ptr->if_string()) {
   	                name = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'name' as std::string in MapType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'name' was missing in MapType");
    	    }
            if(auto ptr = obj.if_contains("playerSlots")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'playerSlots' in MapType was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    playerSlots = std::move(arr);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'playerSlots' as array in MapType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("initialTerrains")) {
	            if(auto tPtr = ptr->if_object()) {
        			initialTerrains.readFrom(*tPtr);
    			} else {
        			throw std::runtime_error("Expected 'initialTerrains' as { in MapType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'initialTerrains' was missing in MapType");
    	    }
            if(auto ptr = obj.if_contains("terrainName")) {
	            if(auto tPtr = ptr->if_string()) {
   	                terrainName = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'terrainName' as std::string in MapType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'terrainName' was missing in MapType");
    	    }
            if(auto ptr = obj.if_contains("coordinates")) {
	            if(auto tPtr = ptr->if_object()) {
        			coordinates.readFrom(*tPtr);
    			} else {
        			throw std::runtime_error("Expected 'coordinates' as { in MapType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'coordinates' was missing in MapType");
    	    }
            if(auto ptr = obj.if_contains("x")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                x = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'x' as int64_t in MapType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'x' was missing in MapType");
    	    }
            if(auto ptr = obj.if_contains("y")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                y = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'y' as int64_t in MapType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'y' was missing in MapType");
    	    }
            if(auto ptr = obj.if_contains("orientation")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                orientation = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'orientation' as int64_t in MapType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("playerSlot")) {
	            if(auto tPtr = ptr->if_string()) {
   	                playerSlot = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'playerSlot' as std::string in MapType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("terrainMods")) {
	            if(auto tPtr = ptr->if_object()) {
        			terrainMods.readFrom(*tPtr);
   				} else if(!ptr->is_null()) {
        			throw std::runtime_error("Expected 'terrainMods' as {[key:string]:any} in MapType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("initialUnits")) {
	            if(auto tPtr = ptr->if_object()) {
        			initialUnits.readFrom(*tPtr);
   				} else if(!ptr->is_null()) {
        			throw std::runtime_error("Expected 'initialUnits' as { in MapType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("unitName")) {
	            if(auto tPtr = ptr->if_string()) {
   	                unitName = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'unitName' as std::string in MapType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'unitName' was missing in MapType");
    	    }
            if(auto ptr = obj.if_contains("coordinates")) {
	            if(auto tPtr = ptr->if_object()) {
        			coordinates.readFrom(*tPtr);
    			} else {
        			throw std::runtime_error("Expected 'coordinates' as { in MapType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'coordinates' was missing in MapType");
    	    }
            if(auto ptr = obj.if_contains("x")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                x = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'x' as int64_t in MapType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'x' was missing in MapType");
    	    }
            if(auto ptr = obj.if_contains("y")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                y = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'y' as int64_t in MapType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'y' was missing in MapType");
    	    }
            if(auto ptr = obj.if_contains("playerSlot")) {
	            if(auto tPtr = ptr->if_string()) {
   	                playerSlot = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'playerSlot' as std::string in MapType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("unitMods")) {
	            if(auto tPtr = ptr->if_object()) {
        			unitMods.readFrom(*tPtr);
   				} else if(!ptr->is_null()) {
        			throw std::runtime_error("Expected 'unitMods' as {[key:string]:any} in MapType, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
        }

        void writeTo(json::object & obj) const {
           	obj["name"] = name;
       		if(playerSlots) {
    		    json::array arr;
       		    for(auto const& val : *playerSlots) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["playerSlots"] = std::move(arr);
    		}
            {
        		json::object m;
            	initialTerrains.writeTo(m);
        		obj["initialTerrains"] = std::move(m);
        	}
           	obj["terrainName"] = terrainName;
            {
        		json::object m;
            	coordinates.writeTo(m);
        		obj["coordinates"] = std::move(m);
        	}
           	obj["x"] = x;
           	obj["y"] = y;
            if(orientation) {
          	    obj["orientation"] = *orientation;
          	}
            if(playerSlot) {
          	    obj["playerSlot"] = *playerSlot;
          	}
            if(terrainMods) {
        		json::object m;
           		terrainMods->writeTo(m);
        		obj["terrainMods"] = std::move(m);
        	}
            if(initialUnits) {
        		json::object m;
           		initialUnits->writeTo(m);
        		obj["initialUnits"] = std::move(m);
        	}
           	obj["unitName"] = unitName;
            {
        		json::object m;
            	coordinates.writeTo(m);
        		obj["coordinates"] = std::move(m);
        	}
           	obj["x"] = x;
           	obj["y"] = y;
            if(playerSlot) {
          	    obj["playerSlot"] = *playerSlot;
          	}
            if(unitMods) {
        		json::object m;
           		unitMods->writeTo(m);
        		obj["unitMods"] = std::move(m);
        	}
        }
    };
}
