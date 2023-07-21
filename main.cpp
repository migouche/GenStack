#include <iostream>
#include "Value.h"
#include "Operations.h"
#include "Stack.h"


using namespace std;

int main() {
    std::string s;
    Stack stack;

    std::cout << "Write 'exit' or 'quit' to exit\n"
                 "Write a number to push it to the stack\n"
                 "Start with \" to push a string\n"
                 "Start with ' to push an operation\n"
                 "Write an operation to execute it\n\n\n";

    while (std::cin >> s)
    {
        if(s == "exit" || s == "quit")
            return 0;

        if(isdigit(s[0]) || s[0] == '-')
            stack.push(make_shared<IntValue>(strtol(s.c_str(), nullptr, 10)));
        else if (s[0] == '\'') {
            s.erase(s.begin());
            stack.push(make_shared<OperationValue>(Operations::get_operation(s)));
        }
        else if (s[0] == '\"')
        {
            s.erase(s.begin());
            stack.push(make_shared<StringValue>(s));
        }
        else
            Operations::get_operation(s)(&stack);

        std::cout << "\n\n\n";
        stack.print();
    }

    return 0;
}


/*
5.6
[ 10 * 1 + ]
3
for


3 +

[ 3 + ] eval
 */