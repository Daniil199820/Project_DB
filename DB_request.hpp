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