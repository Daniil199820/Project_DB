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
#include <any>

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
    int type_idx;
    int idx;
public:
    Column(int type_idx, std::string):type_idx(type_idx) { 
        if(type_idx == CUSTOM_INT){
            array = std::vector<int>();
        }

        if(type_idx == CUSTOM_DOUBLE){
            array = std::vector<double>();
        }

        if(type_idx == CUSTOM_STRING){
            array = std::vector<std::string>();
        }

    }
    
    bool name_is_same(const std::string& name_col){
        return (name_col==name_column);
    }

    void clear_values(){
        if(type_idx == CUSTOM_INT){
           std::get<std::vector<int>>(array).clear();
        }

        if(type_idx == CUSTOM_DOUBLE){
            std::get<std::vector<double>>(array).clear();
        }

        if(type_idx == CUSTOM_STRING){
            std::get<std::vector<std::string>>(array).clear();
        }
    }


    void add_value(std::variant<int,double,std::string> value){
        if(type_idx == CUSTOM_INT){
            std::get<std::vector<int>>(array).push_back(std::get<int>(value));
        }

        if(type_idx == CUSTOM_DOUBLE){
            std::get<std::vector<double>>(array).push_back(std::get<double>(value));
        }

        if(type_idx == CUSTOM_STRING){
            std::get<std::vector<std::string>>(array).push_back(std::get<std::string>(value));
        }
        
    }

};

class DB_data{
public:
    void Create_Table(const std::string& name){
        if(check_exist_table(name)){
            throw std::logic_error("Table exist yet.");
        }
        tables[name] = std::vector<Column>{};
    }

    void Add_Column(const std::string& name,const std::string& table_name, std::pair<int,std::string>& pair){
        if(!check_exist_table(name)){
            throw std::logic_error("Table doesn't exist.");
        }
        tables[table_name].emplace_back(pair.first, pair.second);
    }

    void Drop_Table(const std::string& table_name){
        if(!check_exist_table(table_name)){
            throw std::logic_error("Table doesn't exist.");
        }
        auto current_table = tables[table_name];
        for(auto& cur_col = current_table.begin(); cur_col != current_table.end(); ++ cur_col){
            cur_col->clear_values();
        }
    }

    void Delete_Column(std::string& table_name, std::string& column_name){
        if(!check_exist_table(table_name)){
            throw std::logic_error("Table doesn't exist.");
        }

        auto current_table = tables[table_name];
        bool logic_result = false;
        for(auto& cur_col = current_table.begin();cur_col != current_table.end(); ++ cur_col){
            if(cur_col->name_is_same(column_name)){
                logic_result = true;
                current_table.erase(cur_col);
            }
        }
        
        if(!logic_result){
            throw std::logic_error("Column doesn't exist.");
        }
    }
   // std::vector< std::variant<int,double,std::string> >value
    void Insert_Value(const std::string& table_name, std::vector<std::variant<int,double,std::string>> values){
        if(!check_exist_table(table_name)){
            throw std::logic_error("Table doesn't exist.");
        }
        if(values.size() != tables[table_name].size()){
            throw std::logic_error("Table and values size shuold be equal.");
        }
        auto current_table = tables[table_name];
        for(int i = 0; i < current_table.size();++i){
            current_table[i].add_value(values[i]);
        }
    }

private:
    using table = std::vector<Column>;
    
    bool check_exist_table(const std::string& name){
        auto finded_name = tables.find(name);
        if(finded_name != tables.end()){
            return true;
        }
        return false;
    }

    std::unordered_map<std::string, table, std::hash<std::string> > tables; 
};