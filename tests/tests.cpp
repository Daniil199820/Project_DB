#include <gtest/gtest.h>
#include "DB_controller.hpp"

TEST(Correct_request, Create_Table_1){
    DB_controller DB;
    std::string result = DB.process("CREATE COLUMN (CUSTOM_INT, AWD)");
    ASSERT_EQ("Error - unknown <CREATE> method.",result);
}

TEST(Correct_request, Create_Table_2){
    DB_controller DB;
    std::string result = DB.process("CREATE TABLE table_name (CUSTOM_INT, AWD)");
    ASSERT_EQ("",result);
}

TEST(Correct_request, Create_Table_3){
    DB_controller DB;
    std::string result = DB.process("CREATE TABLE table_name (CUSTOM_INT, AWD");
    ASSERT_EQ("No end of arguments - <)>.",result);
}

TEST(Functional, delete_column_1){
    DB_controller DB;
    DB.process("CREATE TABLE table_name (CUSTOM_INT, AWD)");
    std::string result = DB.process("DELETE FROM table_nam");
    ASSERT_EQ("Table doesn't exist.",result);
}

TEST(Functional, delete_column_2){
    DB_controller DB;
    DB.process("CREATE TABLE table_name (CUSTOM_INT, AWD)");
    std::string result = DB.process("DELETE FROM table_name");
    ASSERT_EQ("",result);
}

TEST(Functional, Insert_value_1){
    DB_controller DB;
    DB.process("CREATE TABLE table_name (CUSTOM_INT, AWD)");
    DB.process("INSERT VALUES INTO table_name (3)");
    std::string result = DB.process("SELECT FROM * table_name");
    ASSERT_EQ("",result);
}




