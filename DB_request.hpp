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



class Application;

class IHandlerState{
public:
    virtual std::string write(std::queue< std::string>& str_vec, Application* app) = 0;
    virtual ~IHandlerState() = 0;
};

using IHandlerStatePtr = std::unique_ptr<IHandlerState>;

class Create_State:public IHandlerState{
public:
    std::string write(std::queue<std::string>& str_vec, Application* app) override {
        Create_State(){std::cout<< "Create\n";}
        if(str_vec.front() == "TABLE"){
            str_vec.pop();
        }
        else{
            return std::string{""};
        }
    }
};

class Insert_State:public IHandlerState{
public:
    std::string write(std::queue<std::string>& str_vec, Application* app) override{
        Insert_State() {std::cout<< "Insert\n";}
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
    }
};

class Delete_State:public IHandlerState{
public:
    Delete_State(){std::cout<< "Delete\n";}
    std::string write(std::queue<std::string>& str_vec, Application* app) override{
        // if we have this table 
        //if(str_vec.front() )
    }
};

class Select_State:public IHandlerState{
public:
    Select_State(){std::cout<< "Select\n";}
    std::string write(std::queue<std::string>& str_vec, Application* app) override{
        // if we have this table 
        //if(str_vec.front() )
    }
};

class Drop_State:public IHandlerState{
public:
    Drop_State() {std::cout<< "Drop\n";}
    std::string write(std::queue<std::string>& str_vec, Application* app) override{
        //if(str_vec.front() )
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

        }
    }

private:
    std::unordered_map<std::string, std::unique_ptr<IHandlerState>, std::hash<std::string>> commands = {{"CREATE", std::make_shared<Create_State>()},{"INSERT",std::make_shared<Insert_State>()}, {"SELECT",std::make_shared<Select_State>()},{"DELETE",std::make_shared<Delete_State>() }, {"DROP",std::make_shared<Drop_State> ()}};
};

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

