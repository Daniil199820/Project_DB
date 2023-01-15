#pragma once 
#include <vector>
#include <string>
class Observer{
public:
    virtual void update(const std::string&) = 0;
};


class Observable{
public:
    void addObserver(Observer* _observer){
        _observers.push_back(_observer);
    }

    void notifyUpdate(const std::string& arg){
        for(const auto& cur_observer:_observers){
            cur_observer->update(arg);
        }
    }
private:
    std::vector<Observer*> _observers;
};