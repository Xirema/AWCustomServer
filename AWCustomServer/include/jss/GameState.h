#pragma once
#include<jss/jssinclude.h>

namespace sTypes{
    struct GameState{
        std::string id;
        int64_t day;
        int64_t playerTurn;
        std::vector<std::string> playerOrder;
        std::string variant;
        bool active;

        void readFrom(json::object const& obj) {
            if(auto ptr = obj.if_contains("id")) {
	            if(auto tPtr = ptr->if_string()) {
   	                id = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'id' as std::string in GameState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'id' was missing in GameState");
    	    }
            if(auto ptr = obj.if_contains("day")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                day = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'day' as int64_t in GameState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'day' was missing in GameState");
    	    }
            if(auto ptr = obj.if_contains("playerTurn")) {
	            if(auto tPtr = ptr->if_int64()) {
   	                playerTurn = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'playerTurn' as int64_t in GameState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'playerTurn' was missing in GameState");
    	    }
            if(auto ptr = obj.if_contains("playerOrder")) {
	            if(auto tPtr = ptr->if_array()) {
    				std::vector<std::string> arr;
    		        for(auto const& val : *tPtr) {
    			        if(auto vPtr = val.if_string()) {
        			        arr.push_back(std::string{*vPtr});
    				    } else {
    				        throw std::runtime_error("Item in array 'playerOrder' in GameState was expected to be string, but was of wrong type.");
    				    }
    			    }
    			    playerOrder = std::move(arr);
    			} else {
    			    throw std::runtime_error("Expected 'playerOrder' as array in GameState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'playerOrder' was missing in GameState");
    	    }
            if(auto ptr = obj.if_contains("variant")) {
	            if(auto tPtr = ptr->if_string()) {
   	                variant = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'variant' as std::string in GameState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'variant' was missing in GameState");
    	    }
            if(auto ptr = obj.if_contains("active")) {
	            if(auto tPtr = ptr->if_bool()) {
   	                active = *tPtr;
    			} else {
       			    throw std::runtime_error("Expected 'active' as bool in GameState, but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
    	    else {
    	        throw std::runtime_error("'active' was missing in GameState");
    	    }
        }

        void writeTo(json::object & obj) const {
           	obj["id"] = id;
           	obj["day"] = day;
           	obj["playerTurn"] = playerTurn;
        	{
    		    json::array arr;
        	    for(auto const& val : playerOrder) {
        	        arr.push_back(json::string{val});
    		    }
    		    obj["playerOrder"] = std::move(arr);
    		}
           	obj["variant"] = variant;
           	obj["active"] = active;
        }
    };
}
