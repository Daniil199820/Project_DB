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

class Create_Process_Arg_State:public IHandlerState{
public: 
    std::string write(std::queue<std::string>& str_vec, Application* app) override {
        std::vector< std::pair<int,std::string> > vec_pairs;
        while(true){
            auto type_index = find_type(str_vec.front());
            if(type_index == -1){

                break;
               // return {""};/*some error*/
            }
            else{
                str_vec.pop();
            }
 
            if(str_vec.front() == "," || str_vec.front() == ")"){
                return {""};
                //wait name after type definition
            }
            else{
                vec_pairs.push_back(std::make_pair<int, std::string>(std::move(type_index),std::move(str_vec.front())));
                str_vec.pop();
            }
 
            if(str_vec.front() == ")"){
                break;
            }
 
            if(str_vec.front() == ","){
                str_vec.pop();
            }
        
        }
 
        return {""};
    }
};


class Create_Start_Arg_State:public IHandlerState{
public:
    Create_Start_Arg_State(){std::cout<<"Create_Start_Arg_State\n";}
    ~Create_Start_Arg_State(){std::cout<<"Destr Create_Start_Arg_State\n";}
    std::string write(std::queue<std::string>& str_vec, Application* app) override {
        
        if(str_vec.front() == "("){
            str_vec.pop();
            app->set_current(IHandlerStatePtr{new Create_Process_Arg_State()});
            //app->write(str_vec);    
        }
        else{
            return std::string{"Error - "};
            // write something
        }
    }
};

class Create_Table:public IHandlerState{
public:
    std::string write(std::queue<std::string>& str_vec, Application* app) override {

        set_table_name(str_vec.front());
        
        if(str_vec.empty()){
            str_vec.pop();
        }
        else{
            return std::string{"Error - Should be table_name"};
        }
        
        if(tokens[str_vec.front()] != "ARG_START"){
            //error - no ARG_Start
        }
        if(str_vec.empty()){
            str_vec.pop();
        }
        
        while(tokens[str_vec.front()] == "ARG_FINISH" || !str_vec.empty()){
            set_column_type(str_vec.front());
            str_vec.pop();
            set_column_name(str_vec.front());
            str_vec.pop();
        }
        
        if(tokens[str_vec.front()] == "ARG_FINISH"){
            database.Create_Table(table_name);
            for(auto& value:values){
                database.Add_Column(table_name,value);
            }
        }

        else{
            //Error - 
        }
    }

private:
    std::string table_name;
    DB_data database;
    std::vector<std::pair<int,std::string>> values;
    int column_type;
    void set_table_name(const std::string& name_table){
        table_name = name_table;
    }

    void set_column_type( std::string& type){
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
        }
        else{
            // error - unknown type 
        }
    }

    void set_column_name(const std::string& value){
        values.emplace_back(std::make_pair(column_type,value));
    }
};


class Create_State:public IHandlerState{
public:
    std::string write(std::queue<std::string>& str_vec, Application* app) override {
        std::cout<<"we are in Create\n";
        if(str_vec.front() == "TABLE"){
            if(!str_vec.empty()){
                str_vec.pop();
            }
            else{
                return std::string{"Error - no created method."};
            }
            app->set_current(IHandlerStatePtr{new Create_Start_Arg_State()});
        }
        else{
            return std::string{"Error - unknown CREATE method."};
        }
    }
};



class Insert_State:public IHandlerState{
public:
    Insert_State() {std::cout<< "Insert\n";}
    std::string write(std::queue<std::string>& str_vec, Application* app) override{
        
        if(str_vec.front() == "INTO"){

        }
        else{
            return std::string{""};
        }
    }
};

class Insert_State_Table:public IHandlerState{
public:
    std::string write(std::queue<std::string>& str_vec, Application* app) override{
        // if we have this table 
        //if(str_vec.front())
        return std::string{""};
    }
};

class Delete_State:public IHandlerState{
public:
    Delete_State(){std::cout<< "Delete\n";}
    std::string write(std::queue<std::string>& str_vec, Application* app) override{
        // if we have this table 
        //if(str_vec.front() )
        return std::string{""};
    }
};

class Select_State:public IHandlerState{
public:
    Select_State(){std::cout<< "Select\n";}
    std::string write(std::queue<std::string>& str_vec, Application* app) override{
        // if we have this table 
        //if(str_vec.front() )
        return std::string{""};
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



