//
// Created by migouche on 14/08/23.
//

#ifndef GENSTACK_PARSERSTREAM_H
#define GENSTACK_PARSERSTREAM_H

#include <istream>
#include <memory>

class ParserStream
{
private:
    std::istream& stream;
    std::string token;
    bool eof = false;
public:
    explicit ParserStream(std::istream& stream);

    std::string get_token();
    std::string peek_token() const;

    bool is_end_of_stream() const;
};

#endif //GENSTACK_PARSERSTREAM_H
