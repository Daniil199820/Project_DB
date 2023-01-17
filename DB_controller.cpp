#include "DB_controller.hpp"

 DB_controller::DB_controller(){
        database = std::make_shared<DB_data>();
        app = std::make_unique<Application>(database);
        parcer = std::make_unique<Parcer>();
        splitter = std::make_unique<Splitter>();
    }
std::string DB_controller::process(std::string request) noexcept{
        app.get()->set_tokens(std::move(parcer.get()->parce(request)));
        auto result_splitter = splitter.get()->process(request);
        app.get()->set_null();
        auto res = app.get()->write(result_splitter);
        return res;
    }