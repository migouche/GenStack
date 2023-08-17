//
// Created by migouche on 14/07/23.
//

#include "Stack.h"
#include <iostream>
#include "Value.h"


Stack::Stack(): data(std::list<std::shared_ptr<Value>>()){}

void Stack::push (const std::shared_ptr<Value>& v) {this->data.push_front(v);}

void Stack::clear() {
    this->data.clear();
}

std::shared_ptr<Value> Stack::pop() {
    if(data.empty())
        throw std::runtime_error("Stack is empty");
    auto v = this->data.front();
    this->data.pop_front();
    return v;
}

std::shared_ptr<Value> Stack::peek() {
    if(data.empty())
        throw std::runtime_error("Stack is empty");
    return this->data.front();
}

void Stack::push_back(const std::shared_ptr<Value> &v) {
    this->data.push_back(v);
}

std::shared_ptr<Value> Stack::pop_back() {
    if(data.empty())
        throw std::runtime_error("Stack is empty");
    auto v = this->data.back();
    this->data.pop_back();
    return v;
}

std::shared_ptr<Stack> Stack::copy() const {
    auto s = std::make_shared<Stack>();
    for (const auto & value : this->data)
        s->push_back(value);
    return s;
}

size_t Stack::length() const { return this->data.size(); }

void Stack::print() const {
    for(auto it = data.rbegin(); it != data.rend(); it++)
        std::cout << (*it)->print_string() << std::endl;
}