cmake_minimum_required(VERSION 3.2)

project(DATABASE_SQL)

set(INSTALL_GTEST OFF)

find_package(Threads)

add_executable(main main.cpp )

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/GTEST_LINUX"
"googletest")

add_executable(TDD_test tests/tests.cpp)

set_target_properties(main PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)

target_include_directories(TDD_test PRIVATE
        ${GTEST_INCLUDE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(TDD_test PRIVATE
        gtest gtest_main        
)


target_link_libraries(main
        ${CMAKE_THREAD_LIBS_INIT}
        #dl
        )