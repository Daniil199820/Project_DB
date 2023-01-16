#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <variant>
#include <queue>
#include <utility>
#include "DB_parcer.hpp"
#include "DB_request.hpp"
#include "DB_data.hpp"
#include <utility>
#include "DB_controller.hpp"
#include "DB_client.hpp" 
int main(){
    DB_controller DB;
    DB.process("CREATE TABLE table_name (CUSTOM_INT, AWD)");
    DB.process("INSERT INTO table_name VALUES (3)");
    std::string result = DB.process("SELECT FROM * table_name");
    return 0;
}
