#pragma once 

#include "DB_data.hpp"
#include "DB_parcer.hpp"
#include "DB_request.hpp"

class DB_controller{
public:
    DB_controller(){
        database = std::make_shared<DB_data>();
        app = std::make_unique<Application>(database);
        parcer = std::make_unique<Parcer>();
        splitter = std::make_unique<Splitter>();
    }
    std::string process(std::string request){
        app.get()->set_tokens(std::move(parcer.get()->parce(request)));
        auto result_splitter = splitter.get()->process(request);
        app.get()->set_null();
        auto res = app.get()->write(result_splitter);
        return res;
    }
private:
    std::shared_ptr<DB_data> database;
    std::unique_ptr<Application> app;
    std::unique_ptr<Parcer> parcer;
    std::unique_ptr<Splitter> splitter; 
};