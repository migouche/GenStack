//
// Created by migouche on 30/07/23.
//

#ifndef GENSTACK_PARSER_H
#define GENSTACK_PARSER_H

#include <istream>
#include <memory>
#include "ParserStream.h"

class Stack;
class Value;

class Parser
{
public:
    //static Stack parse(std::istream& input, bool print = true);
    static Stack parse(const std::string& input);

    static void new_parse(ParserStream& input, const std::shared_ptr<Stack>& stack);
    static Stack  interactive_parse(); // will use cin

    static std::shared_ptr<Value> parse_block(ParserStream& input);
};

#endif //GENSTACK_PARSER_H
