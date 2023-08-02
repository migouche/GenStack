//
// Created by migouche on 13/07/23.
//

#include <stdexcept>
#include <iostream>
#include "Operations.h"
#include "Stack.h"
#include "Value.h"
#include "Operations.h"
#include "Variables.h"

void int_add(Stack *s) {
    auto [i1, i2] = s->get<IntValue, IntValue>();
    s->push(std::make_shared<IntValue>(i1->get() + i2->get()));
}

void int_sub(Stack *s) {
    auto [i1, i2] = s->get<IntValue, IntValue>();
    s->push(std::make_shared<IntValue>(i2->get() - i1->get()));
}

void int_prod(Stack* s)
{
    auto[i1, i2] = s->get<IntValue, IntValue>();
    s->push(std::make_shared<IntValue>(i1->get() * i2->get()));
}

void eval(Stack *s) {
    auto [op] = s->get<Value>();
    if (is_value_of_type<OperationValue>(op))
        std::dynamic_pointer_cast<OperationValue>(op)->eval(s);
    else if (is_value_of_type<FunctionValue>(op))
        std::dynamic_pointer_cast<FunctionValue>(op)->eval(s);
    else
        throw std::runtime_error("Cannot eval " + op->to_string());
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

void store(Stack *s)
{
    auto [name, val] = s->get<StringValue, Value>();
    Variables::set_variable(name->get(), val);
}

void loop(Stack *s)
{
    auto [n, op] = s->get<IntValue, Value>();
    if(!is_value_of_type<OperationValue>(op) && !is_value_of_type<FunctionValue>(op))
        throw std::runtime_error("Cannot loop on " + op->to_string());



    for(int i = 0; i < n->get(); i++)
    {
        if(is_value_of_type<OperationValue>(op))
            std::dynamic_pointer_cast<OperationValue>(op)->eval(s);
        else if (is_value_of_type<FunctionValue>(op))
            std::dynamic_pointer_cast<FunctionValue>(op)->eval(s);
    }
}

std::map<std::string, std::function<void(Stack*)>> Operations::operations = {
        {"+", int_add},
        {"-", int_sub},
        {"*", int_prod},
        {"eval", eval},
        {"swap", swap},
        {"pop", pop},
        {"cat", cat},
        {"store", store},
        {"for", loop},
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

