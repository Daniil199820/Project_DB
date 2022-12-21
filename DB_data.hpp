#pragma once 
#include <string>
#include <exception>
#include <cstdlib>
#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>
#include <sstream>
#include "DB_details.hpp"


int to_integer(const std::string& arg){
  int temp;
  try{
    temp = std::stoi(arg);
  }
  catch(const std::invalid_argument& inv_er){
    throw std::invalid_argument("Argument is not an integer.");
  }
  return temp;
}


struct Column{
private:
    CUSTOM_TYPE MY_TYPE;
    std::string name_column;
    std::variant< std::vector<int>, std::vector<double>, std::vector<std::string>> array;
    int i;

      
public:
    Column(int idx, std::string):i(idx) { 
        if(i == CUSTOM_INT){
            
            array = std::vector<int>();
        }

        if(i == CUSTOM_DOUBLE){
            
            array = std::vector<double>();
        }

        if(i == CUSTOM_STRING){
            
            array = std::vector<std::string>();
            
        }
    }


    void add_value(std::variant<int,double,std::string> value){
        if(i == CUSTOM_INT){
            
            std::get<std::vector<int>>(array).push_back(std::get<int>(value));
        }

        if(i == CUSTOM_DOUBLE){
            
            std::get<std::vector<double>>(array).push_back(std::get<double>(value));
        }

        if(i == CUSTOM_STRING){
            
            std::get<std::vector<std::string>>(array).push_back(std::get<std::string>(value));
        }
        
    }

    void show_vector(){
        
        if(i == CUSTOM_INT){
            auto vec = std::get<0>(array);
            for(auto& it: vec){
                std::cout<< it<< " ";
            }
        }

        if(i == CUSTOM_DOUBLE){
            auto vec = std::get<1>(array);
            for(auto& it: vec){
                std::cout<< it<< " ";
            }
        }

        if(i == CUSTOM_STRING){
            auto vec = std::get<2>(array);
            for(auto& it: vec){
                std::cout<< it<< " ";
            }
        }
    }
};

class DB_data{
public:
    void Create_Table(const std::string& name, std::vector<std::pair<int,std::string>>& values){

    }
private:
    std::vector<Column> columns;
};