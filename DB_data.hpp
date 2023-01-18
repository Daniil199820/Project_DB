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
#include <variant>

struct Column{
private:
    CUSTOM_TYPE MY_TYPE;
    std::string name_column;
    int type_idx;
    int idx;
    std::vector< std::variant<int,double,std::string>> array;
public:
    Column(int type_idx, const std::string& name_col):type_idx(type_idx), name_column(name_col){ 
        int x=0;
        x=21;
    }
    
    bool name_is_same(const std::string& name_col){
        return (name_col==name_column);
    }

    void clear_values(){
        array.clear();
    }

    std::vector<std::string> get_array(){
        std::vector<std::string> result_array;
        result_array.emplace_back(name_column);
        if(type_idx == CUSTOM_INT){
            for(int i = 0; i< array.size(); ++i){
                result_array.emplace_back(std::to_string(std::get<int> (array[i])));
            }
        }
        if(type_idx == CUSTOM_DOUBLE){
            for(int i = 0; i< array.size(); ++i){
                result_array.emplace_back(std::to_string(std::get<double>(array[i])));
            }
        }
        if(type_idx == CUSTOM_STRING){
            for(int i = 0; i< array.size(); ++i){
                result_array.emplace_back(std::get<std::string>(array[i]));
            }
        }
        return result_array;
    }

    void add_value(const std::string& value){
        if(type_idx == CUSTOM_INT){
            int int_value = 0;
            try{
                int_value = atoi(value.c_str());
                array.push_back(int_value);  
            }
            catch(...){
                throw std::invalid_argument("Error couldn't " + value + "to CUSTOM_INT");
            }
        }

        if(type_idx == CUSTOM_DOUBLE){
            double double_value = 0.0;
            try{
                double_value = atof(value.c_str());
                array.push_back(double_value);
            }
            catch(...){
                throw std::invalid_argument("Error couldn't " + value + "to CUSTOM_DOUBLE");
            }
        }

        if(type_idx == CUSTOM_STRING){
            array.push_back(value);
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

    void Add_Column(const std::string& table_name, std::pair<int,std::string>& pair){
        if(!check_exist_table(table_name)){
            throw std::logic_error("Table doesn't exist.");
        }
        tables[table_name].emplace_back(pair.first, pair.second);
    }

    void Drop_Table(const std::string& table_name){
        if(!check_exist_table(table_name)){
            throw std::logic_error("Table doesn't exist.");
        }
        auto& current_table = tables[table_name];
        for(auto cur_col = current_table.begin(); cur_col != current_table.end(); ++ cur_col){
            cur_col->clear_values();
        }
    }
    

    void Delete_Column(std::string& table_name, std::string& column_name){
        if(!check_exist_table(table_name)){
            throw std::logic_error("Table doesn't exist.");
        }

        auto& current_table = tables[table_name];
        bool logic_result = false;
        for(auto cur_col = current_table.begin();cur_col != current_table.end(); ++ cur_col){
            if(cur_col->name_is_same(column_name)){
                logic_result = true;
                current_table.erase(cur_col);
            }
        }
        
        if(!logic_result){
            throw std::logic_error("Column doesn't exist.");
        }
    }

    void Insert_Value(const std::string& table_name,std::vector<std::string>& values){
        if(!check_exist_table(table_name)){
            throw std::logic_error("Table doesn't exist.");
        }
        if(values.size() != tables[table_name].size()){
            throw std::logic_error("Table and values size should be equal.");
        }
        auto& current_table = tables[table_name];
        for(int i = 0; i < current_table.size();++i){
            current_table[i].add_value(values[i]);
        }
    }

    std::vector<std::vector<std::string>> Show_Table_Data(const std::string& table_name){
        if(!check_exist_table(table_name)){
            throw std::logic_error("Table doesn't exist.");
        }
        std::vector<std::vector<std::string>> result_data;
        auto& current_table = tables[table_name];
        for(int i = 0; i < current_table.size();++i){
            result_data.push_back(current_table[i].get_array());
        }
        return result_data;
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
