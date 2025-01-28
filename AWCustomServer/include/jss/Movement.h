#pragma once
#include<jss/jssinclude.h>
#include<sstream>

namespace dTypes{
	struct MovementRule{
		std::string name;
		std::string type;
		int64_t maxRepeat;
		std::string confirmType;
		bool stopIfUsed;

		void readFrom(json::object const& obj) {
			if(auto ptr = obj.if_contains("name")) {
	            if(auto tPtr = ptr->if_string()) {
   	                name = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'name' as std::string in MovementRule, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'name' was missing in MovementRule");
    	    }
			if(auto ptr = obj.if_contains("type")) {
	            if(auto tPtr = ptr->if_string()) {
   	                type = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'type' as std::string in MovementRule, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'type' was missing in MovementRule");
    	    }
			if(auto ptr = obj.if_contains("maxRepeat")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                maxRepeat = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'maxRepeat' as int64 in MovementRule, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'maxRepeat' was missing in MovementRule");
    	    }
			if(auto ptr = obj.if_contains("confirmType")) {
	            if(auto tPtr = ptr->if_string()) {
   	                confirmType = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'confirmType' as std::string in MovementRule, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'confirmType' was missing in MovementRule");
    	    }
			if(auto ptr = obj.if_contains("stopIfUsed")) {
	            if(auto tPtr = ptr->if_bool()) {
   	                stopIfUsed = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'stopIfUsed' as bool in MovementRule, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'stopIfUsed' was missing in MovementRule");
    	    }
		}
		
        void writeTo(json::object & obj) const {
           	obj["name"] = name;
       		obj["type"] = type;
			obj["maxRepeat"] = maxRepeat;
			obj["confirmType"] = confirmType;
			obj["stopIfUsed"] = stopIfUsed;
        }
	};
    struct MovementClass{
        std::string name;
        std::map<std::string, int64_t> movementCosts;
        std::optional<std::map<std::string, std::map<std::string, int64_t>>> variantMods;
		std::vector<std::string> movementRules;

        void readFrom(json::object const& obj) {
            if(auto ptr = obj.if_contains("name")) {
	            if(auto tPtr = ptr->if_string()) {
   	                name = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'name' as std::string in MovementClass, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'name' was missing in MovementClass");
    	    }
            if(auto ptr = obj.if_contains("movementCosts")) {
	            if(auto tPtr = ptr->if_object()) {
   				    std::map<std::string, int64_t> map;
   				    for(auto const& entry : *tPtr) {
   				        if(auto mPtr = entry.value().if_int64()) {
   				            map[entry.key_c_str()] = int64_t{*mPtr};
   				        } else {
   				            throw std::runtime_error("Entry in map 'movementCosts' in MovementClass was expected to be int64, but was of wrong type.");
   				        }
   				    }
   				    movementCosts = std::move(map);
    			} else {
   				    throw std::runtime_error("Expected 'movementCosts' as object in MovementClass, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'movementCosts' was missing in MovementClass");
    	    }
            if(auto ptr = obj.if_contains("variantMods")) {
	            if(auto tPtr = ptr->if_object()) {
    			    std::map<std::string, std::map<std::string, int64_t>> map;
    			    for(auto const& entry : *tPtr) {
    			        if(auto mPtr = entry.value().if_object()) {
    			            std::map<std::string, int64_t> subMap;
    			            for(auto const& subEntry : *mPtr) {
    			                if(auto mmPtr = subEntry.value().if_int64()) {
    			                    subMap[subEntry.key_c_str()] = int64_t{*mmPtr};
    			                } else {
    			                    std::stringstream ss;
    			                    ss << "Entry in map 'variantMods[";
    			                    ss << entry.key_c_str();
    			                    ss << "] in MovementClass was expected to be int64, but was of wrong type.";
    			                    std::string exp = ss.str();
    			                    throw std::runtime_error(exp.c_str());
    			                }
    			            }
    			            map[entry.key_c_str()] = std::move(subMap);
    			        } else {
    			            throw std::runtime_error("Entry in map 'variantMods' in MovementClass was expected to be object, but was of wrong type.");
    			        }
    			    }
    			    variantMods = std::move(map);
   				} else if(!ptr->is_null()) {
    			    throw std::runtime_error("Expected 'variantMods' as object in MovementClass, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
			
            if(auto ptr = obj.if_contains("movementRules")) {
	            if(auto tPtr = ptr->if_array()) {
   				    for(auto const& entry : *tPtr) {
						if(auto ePtr = entry.if_string()) {
							movementRules.emplace_back(*ePtr);
						} else {
							throw std::runtime_error("Entry in array 'movementRules' was expected to be string, but was of wrong type.");
						}
					}
    			} else {
   				    throw std::runtime_error("Expected 'movementRules' as array in MovementClass, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'movementRules' was missing in MovementClass");
    	    }
        }

        void writeTo(json::object & obj) const {
           	obj["name"] = name;
       		{
   			    json::object m;
       		    for(auto const& entry : movementCosts) {
   			        m[entry.first] = entry.second;
   			    }
   			    obj["movementCosts"] = std::move(m);
   			}
       		if(variantMods) {
    		    json::object m;
       		    for(auto const& entry : *variantMods) {
    		        json::object mm;
    		        for(auto const& subEntry : entry.second) {
    		            mm[subEntry.first] = subEntry.second;
    		        }
    		        m[entry.first] = std::move(mm);
    		    }
    		    obj["variantMods"] = std::move(m);
    		}
			{
				json::array arr;
				for(auto const& entry : movementRules) {
					arr.push_back(json::string{entry});
				}
				obj["movementRules"] = std::move(arr);
			}
        }
    };
}
