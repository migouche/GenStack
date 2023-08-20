//
// Created by migouche on 14/08/23.
//

#include <iostream>
#include "ParserStream.h"

ParserStream::ParserStream(std::istream &stream): stream(stream)
{
    this->get_token();
}

std::string ParserStream::get_token() {
    std::string out = this->token;
    this->stream >> this->token;
    this->eof = !this->stream;
    //std::cout << "consuming token: " << out << std::endl;
    return out;
}

std::string ParserStream::peek_token() const {
    return this->token;
}

bool ParserStream::is_end_of_stream() const {
    return this->eof;
}
