#pragma once 

#include "DB_data.hpp"
#include "DB_parcer.hpp"
#include "DB_request.hpp"
#include "DB_details.hpp"

class DB_controller{
public:
    DB_controller();
    std::string process(std::string request) noexcept;
private:
    std::shared_ptr<DB_data> database;
    std::unique_ptr<Application> app;
    std::unique_ptr<Parcer> parcer;
    std::unique_ptr<Splitter> splitter; 
};