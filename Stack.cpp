//
// Created by migouche on 14/07/23.
//

#include "Stack.h"
#include "Value.h"
#include <iostream>


Stack::Stack(): data(std::list<std::shared_ptr<Value>>()){}

void Stack::push (const std::shared_ptr<Value>& v) {this->data.push_front(v);}

std::shared_ptr<Value> Stack::pop() {
    if(data.empty())
        throw std::runtime_error("Stack is empty");
    auto v = this->data.front();
    this->data.pop_front();
    return v;
}

size_t Stack::length() const { return this->data.size(); }

void Stack::print() const {
    for(auto it = data.rbegin(); it != data.rend(); it++)
        std::cout << (*it)->print_string() << std::endl;
}