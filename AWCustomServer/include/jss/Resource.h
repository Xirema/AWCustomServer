#pragma once
#include<jss/jssinclude.h>

namespace rTypes{
    struct PackMetadata{
        std::string name;
        std::string version;
        std::optional<std::string> created;
        std::optional<std::string> packId;

        void readFrom(json::object const& obj) {
            if(auto ptr = obj.if_contains("name")) {
	            if(auto tPtr = ptr->if_string()) {
   	                name = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'name' as std::string in PackMetadata, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'name' was missing in PackMetadata");
    	    }
            if(auto ptr = obj.if_contains("version")) {
	            if(auto tPtr = ptr->if_string()) {
   	                version = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'version' as std::string in PackMetadata, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'version' was missing in PackMetadata");
    	    }
            if(auto ptr = obj.if_contains("created")) {
	            if(auto tPtr = ptr->if_string()) {
   	                created = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'created' as std::string in PackMetadata, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
            if(auto ptr = obj.if_contains("packId")) {
	            if(auto tPtr = ptr->if_string()) {
   	                packId = *tPtr;
   				} else if(!ptr->is_null()) {
       			    throw std::runtime_error("Expected 'packId' as std::string in PackMetadata, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
        }

        void writeTo(json::object & obj) const {
           	obj["name"] = name;
           	obj["version"] = version;
            if(created) {
          	    obj["created"] = *created;
          	}
            if(packId) {
          	    obj["packId"] = *packId;
          	}
        }
    };
    struct TextResource {
        std::string key;
        std::string type;
        std::string shortName;
        std::string longName;
        std::string description;
        std::optional<std::string> language;

        void readFrom(json::object const& obj) {
            if (auto ptr = obj.if_contains("key")) {
                if (auto tPtr = ptr->if_string()) {
                    key = *tPtr;
                }
                else {
                    throw std::runtime_error("Expected 'key' as std::string in TextResource, but was of type " + std::string(to_string(ptr->kind())) + ".");
                }
            }
            else {
                throw std::runtime_error("'key' was missing in TextResource");
            }
            if (auto ptr = obj.if_contains("type")) {
                if (auto tPtr = ptr->if_string()) {
                    type = *tPtr;
                }
                else {
                    throw std::runtime_error("Expected 'type' as std::string in TextResource, but was of type " + std::string(to_string(ptr->kind())) + ".");
                }
            }
            else {
                throw std::runtime_error("'type' was missing in TextResource");
            }
            if (auto ptr = obj.if_contains("shortName")) {
                if (auto tPtr = ptr->if_string()) {
                    shortName = *tPtr;
                }
                else {
                    throw std::runtime_error("Expected 'shortName' as std::string in TextResource, but was of type " + std::string(to_string(ptr->kind())) + ".");
                }
            }
            else {
                throw std::runtime_error("'shortName' was missing in TextResource");
            }
            if (auto ptr = obj.if_contains("longName")) {
                if (auto tPtr = ptr->if_string()) {
                    longName = *tPtr;
                }
                else {
                    throw std::runtime_error("Expected 'longName' as std::string in TextResource, but was of type " + std::string(to_string(ptr->kind())) + ".");
                }
            }
            else {
                throw std::runtime_error("'longName' was missing in TextResource");
            }
            if (auto ptr = obj.if_contains("description")) {
                if (auto tPtr = ptr->if_string()) {
                    description = *tPtr;
                }
                else {
                    throw std::runtime_error("Expected 'description' as std::string in TextResource, but was of type " + std::string(to_string(ptr->kind())) + ".");
                }
            }
            else {
                throw std::runtime_error("'description' was missing in TextResource");
            }
            if (auto ptr = obj.if_contains("language")) {
                if (auto tPtr = ptr->if_string()) {
                    language = *tPtr;
                }
                else if (!ptr->is_null()) {
                    throw std::runtime_error("Expected 'language' as std::string in TextResource, but was of type " + std::string(to_string(ptr->kind())) + ".");
                }
            }
        }

        void writeTo(json::object& obj) const {
            obj["key"] = key;
            obj["type"] = type;
            obj["shortName"] = shortName;
            obj["longName"] = longName;
            obj["description"] = description;
            if (language) {
                obj["language"] = *language;
            }
        }
    };
    struct ImageResource {
        std::string key;
        std::string type;
        std::string smallImage;
        std::string largeImage;
        std::optional<std::string> armyColor;
        std::optional<int64_t> orientation;
        std::optional<std::string> variant;

        void readFrom(json::object const& obj) {
            if (auto ptr = obj.if_contains("key")) {
                if (auto tPtr = ptr->if_string()) {
                    key = *tPtr;
                }
                else {
                    throw std::runtime_error("Expected 'key' as std::string in ImageResource, but was of type " + std::string(to_string(ptr->kind())) + ".");
                }
            }
            else {
                throw std::runtime_error("'key' was missing in ImageResource");
            }
            if (auto ptr = obj.if_contains("type")) {
                if (auto tPtr = ptr->if_string()) {
                    type = *tPtr;
                }
                else {
                    throw std::runtime_error("Expected 'type' as std::string in ImageResource, but was of type " + std::string(to_string(ptr->kind())) + ".");
                }
            }
            else {
                throw std::runtime_error("'type' was missing in ImageResource");
            }
            if (auto ptr = obj.if_contains("smallImage")) {
                if (auto tPtr = ptr->if_string()) {
                    smallImage = *tPtr;
                }
                else {
                    throw std::runtime_error("Expected 'smallImage' as std::string in ImageResource, but was of type " + std::string(to_string(ptr->kind())) + ".");
                }
            }
            else {
                throw std::runtime_error("'smallImage' was missing in ImageResource");
            }
            if (auto ptr = obj.if_contains("largeImage")) {
                if (auto tPtr = ptr->if_string()) {
                    largeImage = *tPtr;
                }
                else {
                    throw std::runtime_error("Expected 'largeImage' as std::string in ImageResource, but was of type " + std::string(to_string(ptr->kind())) + ".");
                }
            }
            else {
                throw std::runtime_error("'largeImage' was missing in ImageResource");
            }
            if (auto ptr = obj.if_contains("armyColor")) {
                if (auto tPtr = ptr->if_string()) {
                    armyColor = *tPtr;
                }
                else if (!ptr->is_null()) {
                    throw std::runtime_error("Expected 'armyColor' as std::string in ImageResource, but was of type " + std::string(to_string(ptr->kind())) + ".");
                }
            }
            if (auto ptr = obj.if_contains("orientation")) {
                if (auto tPtr = ptr->if_int64()) {
                    orientation = *tPtr;
                }
                else if (!ptr->is_null()) {
                    throw std::runtime_error("Expected 'orientation' as int64_t in ImageResource, but was of type " + std::string(to_string(ptr->kind())) + ".");
                }
            }
            if (auto ptr = obj.if_contains("variant")) {
                if (auto tPtr = ptr->if_string()) {
                    variant = *tPtr;
                }
                else if (!ptr->is_null()) {
                    throw std::runtime_error("Expected 'variant' as std::string in ImageResource, but was of type " + std::string(to_string(ptr->kind())) + ".");
                }
            }
        }

        void writeTo(json::object& obj) const {
            obj["key"] = key;
            obj["type"] = type;
            obj["smallImage"] = smallImage;
            obj["largeImage"] = largeImage;
            if (armyColor) {
                obj["armyColor"] = *armyColor;
            }
            if (orientation) {
                obj["orientation"] = *orientation;
            }
            if (variant) {
                obj["variant"] = *variant;
            }
        }
    };
    struct ResourcePack{
        PackMetadata packMetadata;
        std::vector<TextResource> textResources;
        std::vector<ImageResource> imageResources;

        void readFrom(json::object const& obj) {
            if(auto ptr = obj.if_contains("packMetadata")) {
	            if(auto tPtr = ptr->if_object()) {
        			packMetadata.readFrom(*tPtr);
    			} else {
        			throw std::runtime_error("Expected 'packMetadata' as PackMetadata in ResourcePack, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'packMetadata' was missing in ResourcePack");
    	    }
            if(auto ptr = obj.if_contains("textResources")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<TextResource> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_object()) {
        					TextResource v;
        					v.readFrom(*vPtr);
        					arr.push_back(std::move(v));
    				    } else {
    				        throw std::runtime_error("Item in array 'textResources' in ResourcePack was expected to be object, but was of wrong type.");
    				    }
    			    }
    			    textResources = std::move(arr);
    			} else {
    			    throw std::runtime_error("Expected 'textResources' as array in ResourcePack, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'textResources' was missing in ResourcePack");
    	    }
            if(auto ptr = obj.if_contains("imageResources")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<ImageResource> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_object()) {
        					ImageResource v;
        					v.readFrom(*vPtr);
        					arr.push_back(std::move(v));
    				    } else {
    				        throw std::runtime_error("Item in array 'imageResources' in ResourcePack was expected to be object, but was of wrong type.");
    				    }
    			    }
    			    imageResources = std::move(arr);
    			} else {
    			    throw std::runtime_error("Expected 'imageResources' as array in ResourcePack, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'imageResources' was missing in ResourcePack");
    	    }
        }

        void writeTo(json::object & obj) const {
            {
        		json::object m;
            	packMetadata.writeTo(m);
        		obj["packMetadata"] = std::move(m);
        	}
        	{
    		    json::array arr;
        	    for(auto const& val : textResources) {
        			json::object obj;
        			val.writeTo(obj);
        			arr.push_back(std::move(obj));
    		    }
    		    obj["textResources"] = std::move(arr);
    		}
        	{
    		    json::array arr;
        	    for(auto const& val : imageResources) {
        			json::object obj;
        			val.writeTo(obj);
        			arr.push_back(std::move(obj));
    		    }
    		    obj["imageResources"] = std::move(arr);
    		}
        }
    };
}
