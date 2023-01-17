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
  /* DB_controller DB;
    DB.process("CREATE TABLE table_name (CUSTOM_INT, AWD)");
    DB.process("INSERT INTO table_name VALUES (3)");
    std::string result = DB.process("SELECT FROM * table_name");
*/
    

    DB_data db_data;
    std::string my_name = "awd";
    std::string n_col = "n_cold";
    std::pair<int,std::string> m_p = std::make_pair<int,std::string>(0,"n_col"); 
    db_data.Create_Table(my_name);
    db_data.Add_Column(my_name,m_p);
    std::vector<std::string> vv = {"5"};
    db_data.Insert_Value(my_name,vv);
    db_data.Insert_Value(my_name,vv);
    db_data.Show_Table_Data(my_name);

    return 0;
}
