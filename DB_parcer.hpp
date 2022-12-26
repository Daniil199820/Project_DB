#pragma once

#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <any>
#include <variant>
#include <unordered_set>
#include <unordered_map>
struct Value_{
std::variant<int, double, std::string> val;
};

struct Type_{
std::string t;
};

struct Name_{
std::string nm;
};

std::unordered_set<std::string, std::hash<std::string>> sets_command = {"CREATE","TABLE"};


struct Command_{
std::string t1;
std::string t2;

void take(std::queue<std::string>& q_str){
    auto it  = sets_command.find(t1 = q_str.front());
    if(it == sets_command.end()){
        throw std::logic_error("Commands doesn't found.");
    }
    q_str.pop();
    
    t2 = q_str.front();
}
};

template<typename container = std::queue<std::string>>
class Parcer{
public:
    container process(const std::string& str){
        if(str.empty()){
            return container{};
        }
        std::vector<std::string> result_array;
        std::string temp_array;
        for(int i=0; i<str.size(); ++i){
            if(str[i]=='\t' || str[i]==' ' || str[i]=='\n' ){
                if(!temp_array.empty()){
                    result_array.emplace_back(temp_array);
                    temp_array.clear();
                }
            }
            else if(str[i] == '(' || str[i]== ')' || str[i]== ',' || str[i] == '\''){
                if(!temp_array.empty()){
                    result_array.emplace_back(temp_array);
                }
                result_array.emplace_back(std::string{str[i]});
                temp_array.clear();
            }
            else{
                temp_array += str[i];
            
            }
        }

        container return_container;
        for(int i = 0; i< result_array.size(); ++i){
            push_element(return_container,result_array[i]);
        }

        return return_container;
    }

private:
    template<typename T>
    decltype(std::declval<T>().push(std::string{""}),void())
    push_element(T& cont, std::string& str){
        cont.push(str);
    }

    template<typename T>
    decltype(std::declval<T>().push_back(std::string{""}),void())
    push_element(T& cont, std::string& str){
        cont.push_back(str);
    }
    
};


