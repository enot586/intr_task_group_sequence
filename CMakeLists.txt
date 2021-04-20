cmake_minimum_required(VERSION 2.8)

project(uniteller)

add_executable(${PROJECT_NAME} "main.cpp"
                               "group.hpp"
                               "sequence.hpp")

set_target_properties(${PROJECT_NAME} PROPERTIES
   CXX_STANDARD 17
   CXX_STANDARD_REQUIRED YES
   CXX_EXTENSIONS NO
)
