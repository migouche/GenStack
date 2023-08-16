//
// Created by migouche on 30/07/23.
//

#ifndef GENSTACK_PARSER_H
#define GENSTACK_PARSER_H

#include <istream>
#include <memory>

class Stack;
class Value;

class Parser
{
public:
    static Stack parse(std::istream& input, bool print = true);
    static Stack parse(const std::string& input);

    static std::shared_ptr<Value> parse_block(std::istream& input, std::string token);
};

#endif //GENSTACK_PARSER_H
