#pragma once

#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <any>
#include <variant>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include "DB_details.hpp"

std::unordered_map<std::string,std::string, std::hash<std::string>> sets_command = {{"CREATE","Command"},{"TABLE","Command"},{"CUSTOM_INT","Type"},
{"CUSTOM_DOUBLE", "Type"},{"CUSTOM_STRING", "Type"}, {"DELETE", "Command"}, {"FROM", "Command"},
 {"INSERT", "Command"}, {"INTO","Command"},{"VALUES","Command"}, {"SELECT", "Command"}, {"*", "Command"}};

using container = std::queue<std::string>;

class Splitter{
public:
    container process(const std::string& str){
        if(str.empty()){
            return container{};
        }
        std::vector<std::string> result_array;
        std::string temp_array;
        for(int i=0; i<str.size(); ++i){
            if((str[i]=='\t' || str[i]==' ' || str[i]=='\n' || str[i] == ',') && !string_value_flag){
                if(!temp_array.empty()){
                    result_array.emplace_back(temp_array);
                    temp_array.clear();
                }
            }
            else if( (str[i] == '(' || str[i]== ')') && !string_value_flag ){
                if(!temp_array.empty()){
                    result_array.emplace_back(temp_array);
                }
                result_array.emplace_back(std::string{str[i]});
                temp_array.clear();
            }
            else if(str[i]== '\''){
                if(string_value_flag){
                    string_value_flag = false;
                }
                else{
                    string_value_flag = true;
                }
            }
            else{
                temp_array += str[i];
                if(i == str.size()-1){
                    if(!temp_array.empty()){
                        result_array.emplace_back(temp_array);
                    }
                }
            }
        }

        container return_container;
        for(int i = 0; i< result_array.size(); ++i){
            push_element(return_container,result_array[i]);
        }
        string_value_flag = false;
        return return_container;
    }

private:
    bool string_value_flag = false;
    template<typename T>
    decltype(std::declval<T>().push(std::string{""}),void())
    push_element(T& cont, std::string& str){
        cont.push(str);
    }

    template<typename T>
    decltype(std::declval<T>().push_back(std::string{""}),void())
    push_element(T& cont, std::string& str){
        cont.push_back(str);
    }
    
};

class Parcer{
private:
std::vector<std::string> unknown_leksems;
TOKENS_TYPE tokens;
bool string_value_flag = false;
void pull_leksema(std::string&& leks){
    if(!leks.empty()){
        unknown_leksems.emplace_back(std::forward<std::string> (leks));
        leks.clear();
    }
}
public:

    TOKENS_TYPE parce(const std::string& str){
        tokens.clear();
        std::string leksema; 
        for(size_t i =0; i<str.size(); ++i){

            if(string_value_flag && !(str[i] == '\'')){
                leksema += str[i];
            }
            else{

                switch (str[i]){

                case '(':
                    tokens[std::string{str[i]}]= "ARG_START";
                    pull_leksema(std::move(leksema));
                    break;
                case ')':
                    tokens[std::string{str[i]}]="ARG_FINISH";
                    pull_leksema(std::move(leksema));
                    break;
                case ',':
                    tokens[std::string{str[i]}]="COMA";
                    pull_leksema(std::move(leksema));
                    break;   
                case '\'':
                    if(string_value_flag){
                        tokens[leksema] = "String_Value";
                        leksema.clear();
                        string_value_flag = false;
                    }
                    else{
                        string_value_flag = true;
                    }
                    break;
                case ' ':
                    pull_leksema(std::move(leksema));
                    break;
                case '\t':
                    pull_leksema(std::move(leksema));
                    break;
                case '\n':
                    pull_leksema(std::move(leksema));
                    break;
                default:
                    leksema += str[i];
                    if(i==str.size()-1){
                        pull_leksema(std::move(leksema));
                    }
                    break;
                }
            }
        }
        parce_unknown(std::move(unknown_leksems));
        string_value_flag = false;
        return tokens;
    }

    void parce_unknown(std::vector<std::string>&& vec_str){
        for(const auto& str: vec_str){
            auto it = sets_command.find(str);
            if(it != sets_command.end()){
                tokens[str] = sets_command[str];
            }
            else{
                tokens[str] = "Value";
            }
        }
    }
};
