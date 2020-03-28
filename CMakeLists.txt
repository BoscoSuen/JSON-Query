cmake_minimum_required(VERSION 3.15)
project(wi20_assignment4_Yukinichi)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(wi20_assignment4_Yukinichi
        CommandProcessor.cpp
        CommandProcessor.hpp
        diagram.png
        domquery2.png
        domquery3.png
        domquery4.png
        JSONParser.cpp
        JSONParser.hpp
        main.cpp
        Model.cpp
        Model.hpp
        README.md
        test.json
        test.txt
        Testing.hpp)
