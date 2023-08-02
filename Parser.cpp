//
// Created by migouche on 30/07/23.
//

#include "Parser.h"

#include "Stack.h"
#include "Value.h"
#include "Operations.h"
#include "Variables.h"

#include <memory>
#include <iostream>
#include <sstream>

Stack Parser::parse(std::istream &input, bool print) {
    bool in_function = false;
    Stack stack = Stack();
    Stack function_stack = Stack();
    std::string s;

    if(print)
        std::cout << "Write 'exit' or 'quit' to exit\n"
                     "Write a number to push it to the stack\n"
                     "Start with \" to push a string\n"
                     "Start with ' to push an operation\n"
                     "Write an operation to execute it\n\n\n";

    // make input not use '-' as a delimiter
    while (input >> s)
    {
        if(s == "exit" || s == "quit")
        {
            if(in_function)
                throw std::runtime_error("Can't exit while in function");
            return stack;
        }

        if(isdigit(s[0]) || (s[0] == '-' && s.length() > 1))
            (in_function ? function_stack: stack).push(std::make_shared<IntValue>(strtol(s.c_str(), nullptr, 10)));
        else if (s[0] == '\'') {
            s.erase(s.begin());
            if(Variables::exists(s))
                Variables::push_variable_no_eval(s, &(in_function ? function_stack: stack));
            else
                if(in_function)
                        function_stack.push(std::make_shared<OperationValue>(
                                [s](Stack *st){
                                    st->push(std::make_shared<OperationValue>(Operations::get_operation(s)));
                                }));
                else
                    stack.push(std::make_shared<OperationValue>(Operations::get_operation(s)));
        }
        else if (s[0] == '\"')
        {
            s.erase(s.begin());
            (in_function ? function_stack: stack).push(std::make_shared<StringValue>(s));
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
            stack.push(std::make_shared<FunctionValue>(function_stack));
            function_stack.clear();
        }
        else
        {
            if(Variables::exists(s))
                Variables::push_variable(s, &(in_function ? function_stack: stack));
            else
            {
                if (in_function)
                    function_stack.push(std::make_shared<OperationValue>(Operations::get_operation(s)));
                else
                    Operations::get_operation(s)(&stack);
            }
        }
        if (print) {
            std::cout << "\n\n\n\n\n\n\n\n\n"; // heh
            stack.print();
        }
    }

    return stack;
}

Stack Parser::parse(const std::string &input)
{
    std::stringstream iss(input);
    return parse(iss, false);
}