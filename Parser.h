//
// Created by migouche on 30/07/23.
//

#ifndef GENSTACK_PARSER_H
#define GENSTACK_PARSER_H

#include <istream>
#include <memory>
#include "ParserStream.h"
#include "Stack.dcl"

class Stack;
class Value;

class Parser
{
public:
    //static Stack parse(std::istream& input, bool print = true);
    static Stack parse(const std::string& input);

    static void new_parse(ParserStream& input, crp_Stack  stack);
    static Stack  interactive_parse(); // will use cin
};

#endif //GENSTACK_PARSER_H
