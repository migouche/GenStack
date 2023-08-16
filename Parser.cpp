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
        else if (s[0] == '"')
        {
            s.erase(s.begin());
            if(s[s.length() - 1] == '"')
                s.erase(s.end() - 1);
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
    ParserStream stream(iss);
    Stack stack;
    new_parse(stream, &stack);
    return stack;
}


Stack Parser::interactive_parse()
{ //REPL
    Stack stack;
    while(true) {
        std::string line;
        std::getline(std::cin, line);
        // std::cout << "line: " << line << std::endl;
        std::istringstream iss(line);
        ParserStream stream(iss);
        new_parse(stream, &stack);
        std::cout << "\n\n\n\n\n\n\n\n\n";
        stack.print();
    }

}


void Parser::new_parse(ParserStream &input, Stack* stack) {
    while(!input.is_end_of_stream() && input.peek_token() != "exit")
    {
        if(Operations::operation_exists(input.peek_token()))
            Operations::get_operation(input.get_token())(stack);
        else if (Variables::exists(input.peek_token()))
            Variables::push_variable(input.get_token(), stack);
        else
            stack->push(parse_block(input));
    }
}

std::shared_ptr<Value> Parser::parse_block(ParserStream &input) {
    std::string token = input.get_token();
    //std::cout << "Parsing token: " << token << std::endl;

    if(isdigit(token[0]) || (token[0] == '-' && token.length() > 1))
        return std::make_shared<IntValue>(strtol(token.c_str(), nullptr, 10));
    else if (token[0] == '"')
    {
        token.erase(token.begin());
        if(token[token.length() - 1] == '\"')
            token.erase(token.end() - 1);
        // std::cout << "going back" << std::endl;
        return std::make_shared<StringValue>(token);
    }
    else if(token[0] == '\'') // can be cleaner if I manage to implement input.get_char() and input.peek_char()
    {
        token.erase(token.begin());
        if(Operations::operation_exists(token))
            return std::make_shared<OperationValue>([token](Stack* s){
                s->push(std::make_shared<FunctionValue>(Operations::get_operation(token)));
            });
        else if(Variables::exists(token))
            return Variables::get_variable(token); //NOTE: evaluating and not evaluating an Int will do the same
    }
    else if (Variables::exists(token))
        return Variables::get_variable(token);
    else if(Operations::operation_exists(token))
        return std::make_shared<OperationValue>(Operations::get_operation(token));
    else if (token == "[")
    {
        Stack s;
        while(input.peek_token() != "]")
            s.push(parse_block(input));
        input.get_token();
        return std::make_shared<FunctionValue>(s);
    }
    else if (token == "]")
        throw std::runtime_error("No function to end");
    throw std::runtime_error("Couldn't parse this :(");
}