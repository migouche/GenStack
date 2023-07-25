//
// Created by migouche on 13/07/23.
//

#include <stdexcept>
#include "Operations.h"
#include "Stack.h"
#include "Value.h"


void int_add(Stack *s) {
    auto [i1, i2] = s->get<IntValue, IntValue>();
    s->push(std::make_shared<IntValue>(i1->get() + i2->get()));
}

void int_sub(Stack *s) {
    auto [i1, i2] = s->get<IntValue, IntValue>();
    s->push(std::make_shared<IntValue>(i2->get() - i1->get()));
}

void eval(Stack *s) {
    auto [op] = s->get<OperationValue>();
    op->eval(s);
}

void swap(Stack *s) {
    auto [v1, v2] = s->get<Value, Value>();
    s->push(v1);
    s->push(v2);
}

void pop(Stack *s) {
    if (s->length() < 1)
        throw std::runtime_error("Stack must have 1 element at least");
    s->pop();
}

void cat(Stack *s) {
    auto [s1, s2] = s->get<StringValue, StringValue>();

    s->push(std::make_shared<StringValue>(s2->get() + s1->get()));
}

std::map<std::string, std::function<void(Stack*)>> Operations::operations = {
        {"+", int_add},
        {"-", int_sub},
        {"eval", eval},
        {"swap", swap},
        {"pop", pop},
        {"cat", cat}
};

std::function<void(Stack*)> Operations::get_operation(const std::string& name)
{
    if(operations.find(name) == operations.end())
        throw std::runtime_error("Operation " + name + " does not exist!");
    return operations.at(name);
}

/*

 3
 [
 $1
 $2
 $3
 ]
 '+
 '+
 */

