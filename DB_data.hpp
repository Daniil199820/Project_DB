#pragma once 
#include <string>
#include <exception>
#include <cstdlib>
#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>
#include <sstream>
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

class Data_storage{
public:
    std::string insert(std::list<std::string>& args){
        
        auto arguments = args;  
        auto it = tables.find(arguments.front());
        if(it == tables.end()){
            return std::string{"This table doesn't exist.\n"};
        }
        arguments.pop_front();

        Table& current_table = it->second;

        int id;
        try {
            id = to_integer(arguments.front());
        }
        catch(const std::exception& ex){
            return std::string{"Error:Id is not integer.\n"};
        }

        auto it_second = current_table.find(id);
        if(it_second==current_table.end()){
            current_table[id] = arguments.back();
        }
        else{
            return std::string{"Duplicate key.\n"};
        }

        return {"Ok.\n"};
    }

    std::string truncate(std::list<std::string>& args){

        auto arguments = args;

        auto it =tables.find(arguments.front());
        if(it!=tables.end()){
            arguments.pop_front();
            Table& current_table = it->second;
            current_table.clear();
        }
        else{
            return {"Element isn't finded!\n"};
        }
        return {"Ok.\n"};
    }

    std::string intersection(std::list<std::string>& args){
        
        decltype(tables) temp_tables = tables;

        std::unordered_map<const int, int,std::hash<int>>table_counter;

        for(const auto& cur_table : tables){
            for(const auto& cur_key: cur_table.second){
                table_counter[cur_key.first] = table_counter.count(cur_key.first) + 1;
            }
        }
        
        std::vector<std::vector<std::string>> two_2_array;        

        for(const auto& it:table_counter){
            if(it.second==tables.size()){
                std::vector<std::string> temp_vector;
                temp_vector.push_back(std::to_string(it.first));
                for(const auto& cur_table : tables){
                    temp_vector.push_back(cur_table.second.at(it.first));
                }
                two_2_array.push_back(temp_vector);
            }
        }

        std::stringstream result_string;

        for(const auto& arr_2:two_2_array){
            for(const auto& arr:arr_2){
                result_string<<arr<<", ";
            }
            result_string<<"\n";
        }
        result_string<<"Ok.\n";
        
        return std::string(result_string.str());
    }

    std::string symmetric_difference(std::list<std::string>& args){
        
        decltype(tables) temp_tables = tables;

        std::unordered_map<const int, int,std::hash<int>>table_counter;

        for(const auto& cur_table : tables){
            for(const auto& cur_key: cur_table.second){
                table_counter[cur_key.first] = table_counter.count(cur_key.first) + 1;
            }
        }

        std::vector<std::vector<std::string>>two_2_array;

        for(const auto& it:table_counter){
            if(it.second==1){
                std::vector<std::string> temp_vector;
                temp_vector.push_back(std::to_string(it.first));
                for(const auto& cur_table: tables){
                    if(cur_table.second.find(it.first)!=cur_table.second.end()){
                        temp_vector.push_back(cur_table.second.at(it.first));
                    }
                }
                two_2_array.push_back(temp_vector);
            }
        }

        std::stringstream result_string;

        for(const auto& arr_2:two_2_array){
            for(const auto& arr:arr_2){
                result_string<<arr<<", ";
            }
            result_string<<"\n";
        }
        result_string<<"Ok.\n";

        return std::string(result_string.str());
    }

private:

    enum Type_num {int_type, double_type, string_type} type_n;

    //using Table = std::unordered_map<Type_num,std::string,std::hash<int>>;
    using Table = std::unordered_map<const int ,std::string,std::hash<int>>;
    Table A;
    Table B;
    std::unordered_map<const std::string, Table,std::hash<std::string>> tables = {{"A",A},{"B",B}};
};