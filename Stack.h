//
// Created by migouche on 14/07/23.
//

#ifndef GENSTACK_STACK_H
#define GENSTACK_STACK_H

#include <list>
#include <memory>
#include <tuple>
#include <string>
#include <stdexcept>
#include "Value.h"

class Stack
{
private:
    std::list<std::shared_ptr<Value>> data;

    void push_back(const std::shared_ptr<Value>& v);
    std::shared_ptr<Value> pop_back();
public:
    Stack();

    void clear();

    void push (const std::shared_ptr<Value>& v);
    std::shared_ptr<Value> pop();
    std::shared_ptr<Value> peek(); // for testing purposes

    std::shared_ptr<Stack> copy() const;

    void print() const;

    size_t length() const;

    template<typename ...T>
    std::tuple<std::shared_ptr<T>...> get()
    {
        if (this->length() < sizeof...(T))
            throw std::runtime_error("Stack must have " + std::to_string(sizeof...(T)) + " elements at least");
        return std::tuple<std::shared_ptr<T>...>{this->pop()->as<T>()...};
    }
private:
    friend class FunctionValue;
};

#endif //GENSTACK_STACK_H
