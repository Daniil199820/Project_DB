#pragma once
#include "observer.hpp"


class Session_DB{
public:
    virtual void do_write(const std::string& arg) = 0;
};

class DB_responce:public Observer{
};