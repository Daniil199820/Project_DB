#pragma once
#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include "Ditchr_Requests.h"
#include <chrono>
#include "Ditchr_Data.h"
#include<memory>
#include "thread_pool.hpp"

class DB_client{
public:

    DB_client(){
        data_store = std::make_shared<Data_storage>();
        req_mngr = std::make_unique <Request_manager>(data_store);
    }

    std::shared_ptr<Data_storage> get_Data_Storage_ptr(){
        return data_store;
    }

    std::shared_ptr<Request_manager> get_Request_ptr(){
        return req_mngr;
    }

    void make_request(const std::string& command_str){
        pool_thread_DB.submit([&](){
            req_mngr->set_request(temp_command);
        })
    }


private:
    std::shared_ptr<Request_manager>req_mngr;
    std::shared_ptr<Data_storage> data_store;
    tp::Thread_pool pool_thread_DB;
};