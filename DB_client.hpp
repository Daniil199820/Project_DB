#pragma once
#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include "DB_controller.hpp"
#include <chrono>
#include <memory>
#include "thread_pool.hpp"

class DB_client{
public:

    DB_client(){
        DB_ctrl = std::make_unique<DB_controller>();
    }

    void make_request(const std::string& command_str){
        pool_thread_DB.submit([&]() {
            DB_ctrl.get()->process(command_str);
        });
    }
private:
    std::unique_ptr<DB_controller> DB_ctrl;
    tp::Thread_pool pool_thread_DB;
};