#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <variant>
#include <queue>
#include <utility>
#include "DB_parcer.hpp"
#include "DB_request.hpp"
/*class Application;

class IHandlerState{
public:
    virtual std::string write(std::queue< std::string>& str_vec) = 0;
    //virtual ~IHandlerState() = 0;
};



class NullState:public IHandlerState{
public:
    std::string write(std::queue<std::string>& str_vec) override{

    }
};

class Create_State:public IHandlerState{
public:
    std::string write(std::queue<std::string>& str_vec) override {
        
    }
};

class Create_Start_Arg_State:public IHandlerState{
public:
    std::string write(std::queue<std::string>& str_vec) override {

        if(str_vec.front() == "("){
            str_vec.pop();    
        }
        else{
            // write something
        }
    }
};

enum CUSTOM_TYPE {CUSTOM_INT, CUSTOM_DOUBLE, CUSTOM_STRING};


int find_type(const std::string& str_type){
    if(str_type == "CUSTOM_INT"){
        return CUSTOM_INT;
    }
    if(str_type == "CUSTOM_DOUBLE"){
        return CUSTOM_DOUBLE;
    }
    if(str_type == "CUSTOM_STRING"){
        return CUSTOM_STRING;
    }
    // type doesn't found
    return -1;
}

struct Column{
private:
    CUSTOM_TYPE MY_TYPE;
    std::string name_column;
    std::variant< std::vector<int>, std::vector<double>, std::vector<std::string>> array;
    int i;

      
public:
    Column(int idx, std::string):i(idx) { 
        if(i == CUSTOM_INT){
            
            array = std::vector<int>();
        }

        if(i == CUSTOM_DOUBLE){
            
            array = std::vector<double>();
        }

        if(i == CUSTOM_STRING){
            
            array = std::vector<std::string>();
            
        }
    }


    void add_value(std::variant<int,double,std::string> value){
        if(i == CUSTOM_INT){
            
            std::get<std::vector<int>>(array).push_back(std::get<int>(value));
        }

        if(i == CUSTOM_DOUBLE){
            
            std::get<std::vector<double>>(array).push_back(std::get<double>(value));
        }

        if(i == CUSTOM_STRING){
            
            std::get<std::vector<std::string>>(array).push_back(std::get<std::string>(value));
        }
        
    }

    void show_vector(){
        
        if(i == CUSTOM_INT){
            auto vec = std::get<0>(array);
            for(auto& it: vec){
                std::cout<< it<< " ";
            }
        }

        if(i == CUSTOM_DOUBLE){
            auto vec = std::get<1>(array);
            for(auto& it: vec){
                std::cout<< it<< " ";
            }
        }

        if(i == CUSTOM_STRING){
            auto vec = std::get<2>(array);
            for(auto& it: vec){
                std::cout<< it<< " ";
            }
        }
    }
};

void Create_Table(const std::string& name, std::vector< std::pair<int, std::string> >& pairs){
    
    std::vector<Column> columns;

    for(auto& it: pairs){
        columns.emplace_back(it.first,it.second);
    }
}
/*
class Create_Process_Arg_State:public IHandlerState{
public: 
    std::string write(std::queue<std::string>& str_vec) override {
        std::vector< std::pair<int,std::string> > vec_pairs;
        std::vector<std::string> names;

        if(str_vec.empty()){
            return {"String  is empty."};
        }

        if(str_vec.back() != ")"){
            return {"Last symbol should be a )."};
        }

        while(true){

            auto type_index = find_type(str_vec.front());
            if(type_index == -1){
            
                return {""};/*some error
            }
            else{
                str_vec.pop();
            }

            if(str_vec.front() == "," || str_vec.front() == ")"){
                return {""};
                //wait name after type definition
            }
            else{
                vec_pairs.push_back(std::make_pair<int, std::string>(std::move(type_index),std::move(str_vec.front())));
                str_vec.pop();
            }

            if(str_vec.front() == ")"){
                break;
            }
            
            

            if(str_vec.front() == ","){
                str_vec.pop();
            }
        
        }
        if(!vec_pairs.empty()){
          
        }

        else{
            return {"No arguments."};
        }


    }
};
*/





template<typename T>
void my_print(T val){
    std::cout<<val<<"\n";
}





void parce(std::string& str){
    
    
}

template<typename T>
void print(T d){
    std::cout<<d;
} 

int main(){

    Parcer<std::vector<std::string>> c;
    auto vec =  c.process("Create TABLE name_table(CUSTOM_INT name_col_1, CUSTOM_DOUBLE name_col_2)");
    for(auto& it:vec){
        std::cout<< it<< "\n";
    }
    

   // std::variant<int,double> var = 1.2;
  /*  while(true){
    size_t a = 0;
    std::string dd;
    std::cin >> a;
    std::cin >> dd;

    Column c(a,"awe");
    c.add_value_2(dd);
    c.show_vector();

    }

*/
  //  auto vec = Parce_str("Create TABLE name_table( CUSTOM_INT name_col_1, CUSTOM_DOUBLE name_col_2");
 //   for(auto& it: vec){
  //      std::cout<< it << "\n";
  //  }

  /*  Create_Process_Arg_State cc;
    std::queue<std::string> qq;
    qq.push("CUSTOM_INT");
    qq.push("name");
    qq.push(")");
    qq.push("CUSTOM_DOUBLE");
    qq.push("name_2");
    qq.push(")");
    qq.push("CUSTOM_STRING");
    qq.push("name_3");*/
  //  cc.write(vec);

    return 0;
}