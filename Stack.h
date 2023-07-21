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

class Value;

class Stack
{
private:
    std::list<std::shared_ptr<Value>> data;
public:
    Stack();

    void push (const std::shared_ptr<Value>& v);
    std::shared_ptr<Value> pop();

    void print() const;

    size_t length() const;

    template<typename ...T>
    std::tuple<std::shared_ptr<T>...> get()
    {
        if (this->length() < sizeof...(T))
            throw std::runtime_error("Stack must have " + std::to_string(sizeof...(T)) + " elements at least");
        return std::tuple<std::shared_ptr<T>...>{this->get_dynamic_pointer_cast<T>(this->pop())...};
    }
private:
    template <typename T>
    std::shared_ptr<T> get_dynamic_pointer_cast(const std::shared_ptr<Value>& v) {
        auto ptr = std::dynamic_pointer_cast<T>(v);
        if (ptr == nullptr)
            throw std::runtime_error("Can't cast to the right type");
        return ptr;
    }
};

#endif //GENSTACK_STACK_H
