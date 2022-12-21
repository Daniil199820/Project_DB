#pragma once

#include <string>
enum CUSTOM_TYPE {CUSTOM_INT, CUSTOM_DOUBLE, CUSTOM_STRING};


int find_type(const std::string& str_type){
    if(str_type == "CUSTOM_INT"){
        return CUSTOM_INT;
    }
    if(str_type == "CUSTOM_DOUBLE"){
        return CUSTOM_DOUBLE;
    }
    if(str_type == "CUSTOM_STRING"){
        return CUSTOM_STRING;
    }
    // type doesn't found
    return -1;
}