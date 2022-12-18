cmake_minimum_required(VERSION 3.2)

project(27.DB)

find_package(Threads)

add_executable(main main.cpp )

set_target_properties(main PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        )

target_link_libraries(main
        ${CMAKE_THREAD_LIBS_INIT}
        #dl
        )