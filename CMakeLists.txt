
cmake_minimum_required(VERSION 3.25)
project(GenStack)

set(CMAKE_CXX_STANDARD 17)

include_directories(~/local/include) # this line may give errorsg

add_executable(genstack main.cpp Value.h Stack.h Stack.cpp Value.cpp
        Variables.cpp
        Variables.h
        Parser.cpp
        Parser.h
        ParserStream.cpp
        ParserStream.h
        Stack.dcl
        FunctionContent.cpp
        FunctionContent.h)
