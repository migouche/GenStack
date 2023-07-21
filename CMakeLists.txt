
cmake_minimum_required(VERSION 3.25)
project(GenStack)

set(CMAKE_CXX_STANDARD 17)

include_directories(~/local/include/testudo) # this line may give errors

add_executable(tests stack.ttd Value.h Value.cpp Stack.h Stack.cpp Operations.h Operations.cpp)
add_executable(genstack main.cpp Value.h Operations.h Operations.cpp Stack.h Stack.cpp Value.cpp)
