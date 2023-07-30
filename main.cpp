#include <iostream>
#include "Value.h"
#include "Operations.h"
#include "Stack.h"
#include "Variables.h"


using namespace std;

int main() {
    std::string s;
    Stack stack;
    Stack function_stack;
    bool in_function = false;

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
            (in_function ? function_stack: stack).push(make_shared<IntValue>(strtol(s.c_str(), nullptr, 10)));
        else if (s[0] == '\'') {
            s.erase(s.begin());
            if(Variables::exists(s))
                Variables::push_variable_no_eval(s, &(in_function ? function_stack: stack));
            else
                (in_function ? function_stack: stack).push(make_shared<OperationValue>(Operations::get_operation(s)));
        }
        else if (s[0] == '\"')
        {
            s.erase(s.begin());
            (in_function ? function_stack: stack).push(make_shared<StringValue>(s));
        }
        else if (s == "[")
        {
            if (in_function)
                throw std::runtime_error("Nested functions are not allowed");
            in_function = true;
        }
        else if (s == "]")
        {
            if(!in_function)
                throw std::runtime_error("No function to end");
            in_function = false;
            stack.push(make_shared<FunctionValue>(function_stack));
            function_stack.clear();
        }
        else
        {
            if(Variables::exists(s))
                Variables::push_variable(s, &(in_function ? function_stack: stack));
            else
            {
                if (in_function)
                    function_stack.push(make_shared<OperationValue>(Operations::get_operation(s)));
                else
                     Operations::get_operation(s)(&stack);
            }
        }
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