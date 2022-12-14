#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <atomic>
#include <memory>
#include <vector>
#include <chrono>
#include <string>
#include <functional>


namespace tp{
    
    template<typename T>
    class Threadsafe_queue{
    public:
        Threadsafe_queue() = default;
        
        void push(T value){
            std::lock_guard<std::mutex> lk(mut);
            data_queue.push(std::move(value));
        }

        bool empty(){
            std::lock_guard<std::mutex> lk(mut);
            return data_queue.empty();
        }

        bool try_pop(T& value){
            std::lock_guard<std::mutex> lk(mut);
            if(data_queue.empty()){
                return false;
            }
            value = std::move(data_queue.front());
            data_queue.pop();
            return true;
        }

    private:
        mutable std::mutex mut;
        std::queue<T> data_queue;
    };


    class Thread_pool{
    private:
        std::atomic<bool> done;
        std::vector<std::thread> threads;
        Threadsafe_queue<std::function<void()>> work_queue;

        void worker_thread(){
            while(!done){

                std::function<void()> task;
                if(work_queue.try_pop(task)){
                    task();
                }
                else{
                    std::this_thread::yield();
                }
            }
        }

        Thread_pool(Thread_pool&&) = delete;

        Thread_pool(const Thread_pool&) = delete;

    public:
        Thread_pool():done(false){
            size_t const thread_count = std::thread::hardware_concurrency();
            try{
                for(size_t i = 0; i<thread_count;++i){
                    threads.push_back(std::thread(&Thread_pool::worker_thread,this));
                }
            }

            catch(...){
                done = true;
                throw;
            }
        }

        ~Thread_pool(){
            done = true;
            for(auto& current_thread:threads){
                if(current_thread.joinable()){
                    current_thread.join();
                }
            }
        }

        template<typename FunctionType>
        void submit(FunctionType f){
            work_queue.push(std::function<void()>(f));
        }
    };

}

//int main(){

//    tp::Thread_pool pool_thread_DB;

//    return 0;
//}