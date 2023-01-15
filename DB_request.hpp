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
#include <mutex>
class Application;

class IHandlerState{
public:
    virtual std::string write(std::queue< std::string>&& str_vec,std::shared_ptr<DB_data>,TOKENS_TYPE& tokens, Application* app) = 0;
    virtual ~IHandlerState() {};
};

using IHandlerStatePtr = std::unique_ptr<IHandlerState>;

class Application{
public:

    Application() = delete;

    Application(std::shared_ptr<DB_data> databace):databace(databace){}
    
    void set_current(IHandlerStatePtr hPtr){
        m_handler = std::move(hPtr);
    }

    void set_tokens(TOKENS_TYPE&& n_tokens){
        tokens = std::move(n_tokens);
    }

    std::string write(std::queue<std::string>&& str){
        return m_handler->write(std::move(str),databace,tokens, this);
    }

private:
    IHandlerStatePtr m_handler;
    std::shared_ptr<DB_data> databace;
    TOKENS_TYPE tokens;
};


class Create_Table:public IHandlerState{
public:
    std::string write(std::queue<std::string>&& str_vec,std::shared_ptr<DB_data> databace,TOKENS_TYPE& tokens, Application* app) override {

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

            if(tokens[str_vec.front()]=="Type"){
                set_column_type(str_vec.front());
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
            mutex_db.lock();
            databace.get()->Create_Table(table_name);
            mutex_db.unlock();
            for(auto& value:values){
                mutex_db.lock();
                databace.get()->Add_Column(table_name,value);
                mutex_db.unlock();
            }
        }
        else{
            return std::string{"No end of arguments - <)>."}; 
        }
        return std::string{""};
    }


private:
    std::mutex mutex_db;
    std::string table_name;
    std::vector<std::pair<int,std::string>> values;
    int column_type;
    bool set_table_name(const std::string& name_table){
        if(!name_table.empty()){
            table_name = std::move(name_table);
            return true;
        }
        return false;
    }

    void set_column_type( std::string& type){
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

    bool set_column_name(const std::string& value){
        if(value.empty()){
            return false;
        }
        values.emplace_back(std::make_pair(column_type,value));
    }
};


class Create_State:public IHandlerState{
public:
    std::string write(std::queue<std::string>&& str_vec,std::shared_ptr<DB_data> databace,TOKENS_TYPE& tokens, Application* app) override {
        std::cout<<"we are in Create\n";
        if(str_vec.front() == "TABLE"){
            str_vec.pop();
            app->set_current(IHandlerStatePtr{new Create_Table()});
            app->write(std::move(str_vec));
        }
        else{
            return std::string{"Error - unknown <CREATE> method."};
        }
    }
};

class Insert_Into:public IHandlerState{
public:
    std::string write(std::queue<std::string>&& str_vec,std::shared_ptr<DB_data> databace,TOKENS_TYPE& tokens, Application* app) override{
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
            mutex_db.lock();
            databace.get()->Insert_Value(table_name, values);
            mutex_db.unlock();
        }
        else{
            return std::string{"No end of arguments - <)>."}; 
        }
        return std::string{""};
    }

private:
    std::mutex mutex_db;
    std::string table_name;
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
    std::string write(std::queue<std::string>&& str_vec,std::shared_ptr<DB_data> databace,TOKENS_TYPE& tokens, Application* app) override{
        if(str_vec.front() == "INTO"){
            str_vec.pop();
            app->set_current(IHandlerStatePtr{new Insert_Into()});
            app->write(std::move(str_vec));
        }
        else{
            return std::string{"Error - unknown <INSERT> method."};
        }
    }
};


class Delete_State:public IHandlerState{
public:
    std::string write(std::queue<std::string>&& str_vec,std::shared_ptr<DB_data> databace,TOKENS_TYPE& tokens, Application* app) override{
        if(str_vec.front()=="FROM"){
            str_vec.pop();
        }
        else{
            return std::string{"No <FROM> token."};
        }
        
        if(set_table_name(str_vec.front())){
            mutex_db.lock();
            databace.get()->Drop_Table(str_vec.front());
            mutex_db.unlock();
            str_vec.pop();
        }
        else{
            return std::string{"No table_name."};
        }
        return std::string{""};
    }
private:
    std::mutex mutex_db;
    std::string table_name;
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
    std::string write(std::queue<std::string>&& str_vec,std::shared_ptr<DB_data> databace,TOKENS_TYPE& tokens, Application* app) override{
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
            mutex_db.lock();
            auto result = databace.get()->Show_Table_Data(table_name);
            mutex_db.unlock();
            std::string string_result;
            for(int j=0;j<result.front().size(); ++j){
                for(int i =0; i<result.size();++i){
                    string_result.append(result[i][j]);
                    string_result.append(" ");
                }
                string_result.append("\n");
            }
            return string_result;
        }
        catch(std::exception& ex){
            return std::string{ex.what()};
        }
    }
private:
    std::mutex mutex_db;
    std::string table_name;
    void set_table_name(const std::string& name_table){
        table_name = name_table;;
    }
};

class NullState:public IHandlerState{
public:
    std::string write(std::queue<std::string>&& str,std::shared_ptr<DB_data> databace,TOKENS_TYPE& tokens, Application* app) override{
        auto it =  commands.find(str.front());
        if(it != commands.end())
        {
            auto st = str.front();
            str.pop();
            app->set_current(IHandlerStatePtr{commands[st]});
            app->write(std::move(str));
        }
        else{
            return std::string{"No command."};
        }
        return std::string{""};
    }

private:
    std::unordered_map<std::string,IHandlerState*, std::hash<std::string>> commands
     = {{"CREATE",new Create_State()},{"INSERT",new Insert_State()},
      {"SELECT",new Select_State()},{"DELETE",new Delete_State()}};
};



