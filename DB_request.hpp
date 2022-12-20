#pragma once

#include "Ditchr_Data.h"
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <map>
#include <list>
#include <memory>
#include <cstdlib>
#include "Observer.h"

enum Type{TYPE_int,TYPE_string};

struct Column{
    Type type;
    std::string name;
};


class ICommand {
public:
    virtual std::string execute(std::list<std::string>& args) = 0;
    virtual ~ICommand() = default;
};

class Command : public ICommand {
public:
    virtual ~Command() = default;
protected:
    Command(std::shared_ptr<Data_storage> d):data_store(d){}
    std::shared_ptr<Data_storage> data_store;
};

class InsertCommand: public Command{
public:
    InsertCommand(std::shared_ptr<Data_storage> d): Command(d){}

    std::string execute(std::list<std::string>& args) override {
        if(args.size()==3){
            return data_store->insert(args);
        }
        else{
            return std::string{"Uncorrect number of arguments(Should be 3)\n"};
        }
    }
};

class TruncateCommand: public Command{
public:
    TruncateCommand(std::shared_ptr<Data_storage> d): Command(d){}

    std::string execute(std::list<std::string>& args) override{
        if(args.size()==1){
            return data_store->truncate(args);
        }
        else{
            return std::string{"Uncorrect number of arguments(Should be 1)\n"};
        }
    }
};

class IntersectionCommand: public Command{
public:
    IntersectionCommand(std::shared_ptr<Data_storage> d): Command(d){}

    std::string execute(std::list<std::string>& args) override{
        if(args.size()==0){
            return data_store->intersection(args);
        }
        else{
            return {"Uncorrect number of arguments(Should be 0)\n"};
        }
    }
};

class SymmetricCommand: public Command{
public:
    SymmetricCommand(std::shared_ptr<Data_storage> d): Command(d){}

    std::string execute(std::list<std::string>& args) override{
        if(args.size()==0){
           return data_store->symmetric_difference(args);
        }
        else{
            return std::string {"Uncorrect number of arguments(Should be 0)\n"};
        }
    }
};



class Request_manager:public Observable{
public:
    Request_manager(std::shared_ptr<Data_storage> dd):data_(dd){}

    void set_request(const std::string& str_command){
        std::vector<Command*> commands;
        auto list_words =  parce_function(str_command);
        if(!list_words.empty()){
            commands.push_back(commands_dict.at(list_words.front()));
            list_words.pop_front();
            for(auto& it:commands){
                notifyUpdate(it->execute(list_words));
            }
        }
        else{
            notifyUpdate("Use spaces for splitting.\n"); 
        }
    }

private:
    
    std::shared_ptr<Data_storage> data_;

    std::unordered_map<const std::string, Command*,std::hash<std::string>> commands_dict = {
        {"INSERT", new InsertCommand(data_)},
        {"TRUNCATE", new TruncateCommand(data_)},
        {"INTERSECTION", new IntersectionCommand(data_)},
        {"SYMMETRIC_DIFFERENCE", new SymmetricCommand(data_)}
    };
    

    std::list<std::string> parce_function(const std::string& str_){
        return split(str_,' ');
    }

    std::list<std::string> split(const std::string &str, char d)
    {
	    std::list<std::string> r;

	    std::string::size_type start = 0;
	    std::string::size_type stop = str.find_first_of(d);
	    while (stop != std::string::npos)
	    {
		    r.push_back(str.substr(start, stop - start));

		    start = stop + 1;
		    stop = str.find_first_of(d, start);
	    }

	    r.push_back(str.substr(start));

	    return r;
    }

};


class Application;

class IHandlerState{
public:
    virtual std::string write(std::queue< std::string>& str_vec, Application* app) = 0;
    virtual ~IHandlerState() = 0;
};

using IHandlerStatePtr = std::unique_ptr<IHandlerState>;

class Create_State:public IHandlerState{
public:
    std::string write(std::queue<std::string>& str_vec) override {
        std::cout<< "Create\n";
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
    std::string write(std::queue<std::string>& str_vec) override{
        std::cout<< "Insert\n";
        if(str_vec.front() == "INTO"){

        }
        else{
            return std::string{""};
        }
    }
};

class Insert_State_Table:public IHandlerState{
public:
    std::string write(std::queue<std::string>& str_vec) override{
        // if we have this table 
        if(str_vec.front())
    }
};

class Delete_State:public IHandlerState{
public:
    std::cout<< "Delete\n";
    std::string write(std::queue<std::string>& str_vec) override{
        // if we have this table 
        if(str_vec.front() )
    }
};

class Select_State:public IHandlerState{
public:
    std::cout<< "Select\n";
    std::string write(std::queue<std::string>& str_vec) override{
        // if we have this table 
        if(str_vec.front() )
    }
};

class Drop_State:public IHandlerState{
public:
    std::cout<< "Drop\n";
    std::string write(std::queue<std::string>& str_vec) override{
        std::cout<< ""
        if(str_vec.front() )
    }
};



class NullState:public IHandlerState{
public:
    std::string write(std::queue<std::string>& str_vec) override{
        auto it =  commands.find(str_vec.front());
        if(it != commands.end())
        {
            auto str = str_vec.front();
            str_vec.pop();


        }
    }

private:
    std::unordered_map<std::string, IHandlerState, std::hash<std::string>> commands = {{"CREATE", Create_State},{"INSERT", Insert_State}, {"SELECT", Select_State}, {"DELETE", Delete_State}, {"DROP", Drop_State}};

};

