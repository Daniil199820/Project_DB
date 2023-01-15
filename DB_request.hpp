#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <map>
#include <list>
#include <memory>
#include <cstdlib>
#include <queue>
#include "DB_details.hpp"
#include "DB_parcer.hpp"
#include "DB_data.hpp"
class Application;

class IHandlerState{
public:
    virtual std::string write(std::queue< std::string>& str_vec, Application* app) = 0;
    virtual ~IHandlerState() {};
protected:

};

using IHandlerStatePtr = std::unique_ptr<IHandlerState>;

class Application{
public:
    
    void set_current(IHandlerStatePtr hPtr){
        m_handler = std::move(hPtr);
    }
    std::string write(std::queue<std::string>& str){
        return m_handler->write(str,this);
    }

private:
    IHandlerStatePtr m_handler;
};


class Create_Table:public IHandlerState{
public:
    std::string write(std::queue<std::string>& str_vec, Application* app) override {

        if(set_table_name(str_vec.front())){
            str_vec.pop();
        }
        else{
            return std::string{"Error - Should be table name."};
        }
        
        if(tokens[str_vec.front()] != "ARG_START"){
            return std::string{"No start arguments - (."};
        }
        str_vec.pop();
        
        while(tokens[str_vec.front()] == "ARG_FINISH" || !str_vec.empty()){
            if(set_column_type(str_vec.front())){
                str_vec.pop();
            }
            else{
                return std::string{"Unknow type."};
            }

            if(set_column_name(str_vec.front())){
                str_vec.pop();
            }
            else{
                return std::string{"No column name."};
            }
        }
        
        if(tokens[str_vec.front()] == "ARG_FINISH"){
            database.Create_Table(table_name);
            for(auto& value:values){
                database.Add_Column(table_name,value);
            }
        }
        else{
            return std::string{"No end of arguments - )."}; 
        }
    }

private:
    std::string table_name;
    DB_data database;
    std::vector<std::pair<int,std::string>> values;
    int column_type;
    bool set_table_name(const std::string& name_table){
        if(!name_table.empty()){
            table_name = std::move(name_table);
            return true;
        }
        return false;
    }

    bool set_column_type( std::string& type){
        if(tokens[type] == "Type"){
            if(type == "CUSTOM_INT"){
                column_type = 0;
            }
            if(type == "CUSTOM_DOUBLE"){
                column_type = 1;
            }
            if(type == "CUSTOM_STRING"){
                column_type = 2;
            }
            return true; 
        }
        else{
            return false; 
        }
    }

    bool set_column_name(const std::string& value){
        if(value.empty()){
            return false;
        }
        values.emplace_back(std::make_pair(column_type,value));
    }
};


class Create_State:public IHandlerState{
public:
    std::string write(std::queue<std::string>& str_vec, Application* app) override {
        std::cout<<"we are in Create\n";
        if(str_vec.front() == "TABLE"){
            str_vec.pop();
            app->set_current(IHandlerStatePtr{new Create_Table()});
        }
        else{
            return std::string{"Error - unknown <CREATE> method."};
        }
    }
};

class Insert_Into:public IHandlerState{
public:
    std::string write(std::queue<std::string>& str_vec, Application* app) override{
        if(set_table_name(str_vec.front())){
            str_vec.pop();
        }
        else{
            return std::string{"Error - Should be table name."};
        }

        if(str_vec.front() == "VALUES"){
            str_vec.pop();
        }
        else{
            return std::string{""};
        }

        if(tokens[str_vec.front()] == "ARG_START"){
            str_vec.pop();
        }
        else{
            return std::string{"No start token - <(>."};
        }

        while(tokens[str_vec.front()] == "ARG_FINISH" || !str_vec.empty()){
            values.emplace_back(str_vec.front());
            str_vec.pop();
        }
        
        if(tokens[str_vec.front()] == "ARG_FINISH"){
            database.Insert_Value(table_name, values);
        }
        else{
            return std::string{"No end of arguments - <)>."}; 
        }

    }

private:
    std::string table_name;
    DB_data database;
    std::vector<std::string> values;
    bool set_table_name(const std::string& name_table){
        if(!name_table.empty()){
            table_name = std::move(name_table);
            return true;
        }
        return false;
    }
};

class Insert_State:public IHandlerState{
public:
    Insert_State() {std::cout<< "Insert\n";}
    std::string write(std::queue<std::string>& str_vec, Application* app) override{
        if(str_vec.front() == "INTO"){
            str_vec.pop();
            app->set_current(IHandlerStatePtr{new Insert_Into()});
        }
        else{
            return std::string{"Error - unknown <INSERT> method."};
        }
    }
};


class Delete_State:public IHandlerState{
public:
    Delete_State(){std::cout<< "Delete\n";}
    std::string write(std::queue<std::string>& str_vec, Application* app) override{
        if(str_vec.front()=="FROM"){
            str_vec.pop();
        }
        else{
            return std::string{"No <FROM> token."};
        }
        
        if(set_table_name(str_vec.front())){
            database.Drop_Table(str_vec.front());
            str_vec.pop();
        }
        else{
            return std::string{"No table_name."};
        }
        return std::string{""};
    }
private:
    std::string table_name;
    DB_data database;
    bool set_table_name(const std::string& name_table){
        if(!name_table.empty()){
            table_name = std::move(name_table);
            return true;
        }
        return false;
    }
};

class Select_State:public IHandlerState{
public:
    Select_State(){std::cout<< "Select\n";}
    std::string write(std::queue<std::string>& str_vec, Application* app) override{
        if(str_vec.front() == "FROM"){
            str_vec.pop();
        }
        else{
            return std::string{"No <FROM> token."};
        }
        if(str_vec.front() == "*"){
            str_vec.pop();
        }
        else{
            return std::string{"No <*> token."};
        }
        if(!str_vec.empty()){
            set_table_name(str_vec.front());
        }
        else{
            return std::string{"No argument in requirement."};
        }
        try{
            auto result = database.Show_Table_Data(table_name);
            std::string string_result;
            for(int j=0;j<result.front().size(); ++j){
                for(int i =0; i<result.size();++i){
                    string_result.append(result[i][j]);
                    string_result.append(" ");
                }
                string_result.append("\n");
            }
        }
        catch(std::exception& ex){
            return std::string{ex.what()};
        }
    }
private:
    DB_data database;
    std::string table_name;
    void set_table_name(const std::string& name_table){
        table_name = name_table;;
    }
};

class Drop_State:public IHandlerState{
public:
    Drop_State() {std::cout<< "Drop\n";}
    std::string write(std::queue<std::string>& str_vec, Application* app) override{
        //if(str_vec.front() )
       return std::string{""};
    }
     
};



class NullState:public IHandlerState{
public:
    
    std::string write(std::queue<std::string>& str, Application* app) override{
        auto it =  commands.find(str.front());
        if(it != commands.end())
        {
            auto st = str.front();
            str.pop();
            app->set_current(IHandlerStatePtr{commands[st]});
            //app->write(str);
        }
        return std::string{"No this command."};
    }

private:
    std::unordered_map<std::string,IHandlerState*, std::hash<std::string>> commands
     = {{"CREATE",new Create_State()},{"INSERT",new Insert_State()},
      {"SELECT",new Select_State()},{"DELETE",new Delete_State()}, {"DROP",new Drop_State()}};
};



