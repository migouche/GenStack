//
// Created by migouche on 30/07/23.
//

#ifndef GENSTACK_PARSER_H
#define GENSTACK_PARSER_H

#include <istream>

class Stack;

class Parser
{
public:
    static Stack parse(std::istream& input, bool print = true);
    static Stack parse(const std::string& input);
};

#endif //GENSTACK_PARSER_H
