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
#include <exception>

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
    int idx;
public:
    Column(int idx, std::string):idx(idx) { 
        if(idx == CUSTOM_INT){
            array = std::vector<int>();
        }

        if(idx == CUSTOM_DOUBLE){
            array = std::vector<double>();
        }

        if(idx == CUSTOM_STRING){
            array = std::vector<std::string>();
        }
    }


    void add_value(std::variant<int,double,std::string> value){
        if(idx == CUSTOM_INT){
            
            std::get<std::vector<int>>(array).push_back(std::get<int>(value));
        }

        if(idx == CUSTOM_DOUBLE){
            
            std::get<std::vector<double>>(array).push_back(std::get<double>(value));
        }

        if(idx == CUSTOM_STRING){
            
            std::get<std::vector<std::string>>(array).push_back(std::get<std::string>(value));
        }
        
    }

    void show_vector(){
        
        if(idx == CUSTOM_INT){
            auto vec = std::get<0>(array);
            for(auto& it: vec){
                std::cout<< it<< " ";
            }
        }

        if(idx == CUSTOM_DOUBLE){
            auto vec = std::get<1>(array);
            for(auto& it: vec){
                std::cout<< it<< " ";
            }
        }

        if(idx == CUSTOM_STRING){
            auto vec = std::get<2>(array);
            for(auto& it: vec){
                std::cout<< it<< " ";
            }
        }
    }
};

class DB_data{
public:
    void Create_Table(const std::string& name){
        try{
            check_exist_table(name);
        }
        catch(...){
            throw;
        }
        tables[name] = std::vector<Column>{};
    }

    void Add_Column(std::string& name, std::string& table_name, std::pair<int,std::string>& pair){
        try{
            check_exist_table(name);
        }
        catch(...){
            throw;
        }
        tables[table_name].emplace_back(pair.first, pair.second);
    }

    void Delete_Column(std::string& table_name, std::string& column_name){

    }

    void insert_value(const std::string& name, std::variant<int,double,std::string> value){
        auto finded_element = tables.find(name);
        if(finded_element != tables.end()){
            for(int i = 0; i < tables[name].size();++i){
                tables[name][i].add_value(value);       
            }
        }
    }
private:
    using table = std::vector<Column>;
    void check_exist_table(const std::string& name){
        auto finded_name = tables.find(name);
        if(finded_name != tables.end()){
            throw std::logic_error("This table exists yet.");
        }
    }
    std::unordered_map<std::string, table, std::hash<std::string> > tables; 
};