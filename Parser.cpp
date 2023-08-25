//
// Created by migouche on 30/07/23.
//

#include "Parser.h"

#include "Stack.h"
#include "Value.h"
#include "Variables.h"

#include <memory>
#include <iostream>
#include <sstream>



Stack Parser::parse(const std::string &input)
{
    std::stringstream iss(input);
    ParserStream stream(iss);
    auto stack = std::make_shared<Stack>();
    new_parse(stream, stack);
    return *stack->copy();
}


Stack  Parser::interactive_parse()
{ //REPL
    auto stack = std::make_shared<Stack>();
    bool running = true;


    Variables::set_variable("exit", std::make_shared<OperationValue>([&running](crp_Stack ){
        running = false;
    }));

    while(running) {
        std::string line;
        std::getline(std::cin, line);
        // std::cout << "line: " << line << std::endl;
        std::istringstream iss(line);
        ParserStream stream(iss);

        new_parse(stream, stack);
        std::cout << "\n\n\n\n\n\n\n\n\n";
        stack->print();

    }
    return *stack->copy();
}


void Parser::new_parse(ParserStream &input, crp_Stack  stack) {
    while(!input.is_end_of_stream())
    {

        if (Variables::exists(input.peek_token()))
            Variables::push_variable(input.get_token(), stack);
            //std::cout << "variable pushed\n";
        else if(input.peek_token()[0] == '\'')
        {
            auto token = input.get_token();
            token.erase(token.begin());
            if (Variables::exists(token))
                Variables::push_variable_no_eval(token, stack);
            else
                throw std::runtime_error("Unknown operation or variable: " + token);
        }
        else
            Value::parse(input)->eval(stack);
    }
}
