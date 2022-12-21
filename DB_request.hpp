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

class Application;

class IHandlerState{
public:
    virtual std::string write(std::queue< std::string>& str_vec, Application* app) = 0;
    //virtual ~IHandlerState() = 0;
};

using IHandlerStatePtr = std::shared_ptr<IHandlerState>;

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
        std::cout<<"we Create_Process_Arg_State\n";   
        while(true){
            std::cout<< "flag_1\n"; 
            auto type_index = find_type(str_vec.front());
            std::cout << "flag_2\n";
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
        std::cout<<"we Create_Start_Arg_State\n";
        if(str_vec.front() == "("){
            str_vec.pop();
            app->set_current(IHandlerStatePtr{new Create_Process_Arg_State()});
            std::cout<<" da\n";
            app->write(str_vec);    
        }
        else{
            return std::string{""};
            // write something
        }
    }
};

class Create_State:public IHandlerState{
public:
    Create_State(){std::cout<< "Create\n";}
    ~Create_State(){std::cout<<"Destructor Create\n";}
    std::string write(std::queue<std::string>& str_vec, Application* app) override {
        std::cout<<"we are in Create\n";
        if(str_vec.front() == "TABLE"){
            str_vec.pop();
            app->set_current(IHandlerStatePtr{new Create_Start_Arg_State()});
            app->write(str_vec);
        }
        else{
            return std::string{""};
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
            app->set_current(commands[st]);
            app->write(str);
        }
        return std::string{""};
    }

private:
    std::unordered_map<std::string,std::shared_ptr<IHandlerState>, std::hash<std::string>> commands
     = {{"CREATE",std::make_shared<Create_State>()},{"INSERT",std::make_shared<Insert_State>()},
      {"SELECT",std::make_shared<Select_State>()},{"DELETE",std::make_shared <Delete_State>()}, {"DROP",std::make_shared <Drop_State>()}};
};



