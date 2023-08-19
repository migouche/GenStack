//
// Created by migouche on 13/07/23.
//

#include <stdexcept>
#include <iostream>
#include "Operations.h"
#include "Stack.h"
#include "Value.h"
#include "Variables.h"

void int_add(const std::shared_ptr<Stack>&s) {
    auto [i1, i2] = s->get<IntValue, IntValue>();
    s->push(std::make_shared<IntValue>(i1->get() + i2->get()));
}

void int_sub(const std::shared_ptr<Stack>&s) {
    auto [i1, i2] = s->get<IntValue, IntValue>();
    s->push(std::make_shared<IntValue>(i2->get() - i1->get()));
}

void int_prod(const std::shared_ptr<Stack>& s)
{
    auto[i1, i2] = s->get<IntValue, IntValue>();
    s->push(std::make_shared<IntValue>(i1->get() * i2->get()));
}

void eval(const std::shared_ptr<Stack>&s) { //NOTE: will eval numbers too, be careful
    auto [op] = s->get<Value>();
    op->eval(s);
}

void swap(const std::shared_ptr<Stack>&s) {
    auto [v1, v2] = s->get<Value, Value>();
    s->push(v1);
    s->push(v2);
}

void pop(const std::shared_ptr<Stack>&s) {
    if (s->length() < 1)
        throw std::runtime_error("Stack must have 1 element at least");
    s->pop();
}

void cat(const std::shared_ptr<Stack>&s) {
    auto [s1, s2] = s->get<StringValue, StringValue>();

    s->push(std::make_shared<StringValue>(s2->get() + s1->get()));
}

void store(const std::shared_ptr<Stack>&s)
{
    auto [name, val] = s->get<StringValue, Value>();
    Variables::set_variable(name->get(), val);
}

void loop(const std::shared_ptr<Stack>&s)
{
    auto [n, op] = s->get<IntValue, Value>();
    if(!is_value_of_type<OperationValue>(op) && !is_value_of_type<FunctionValue>(op))
        throw std::runtime_error("Cannot loop on " + op->to_string());



    for(int i = 0; i < n->get(); i++)
    {
        //std::cout << "loop\n";
        op->eval(s);
        //s->pop()->eval(s);
    }
}

std::map<std::string, std::function<void(const std::shared_ptr<Stack>&)>> Operations::operations = {
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

std::function<void(const std::shared_ptr<Stack>&)> Operations::get_operation(const std::string& name)
{
    if(!operation_exists(name))
        throw std::runtime_error("Operation " + name + " does not exist!");
    return operations.at(name);
}

bool Operations::operation_exists(const std::string &name) {
    return operations.find(name) != operations.end();
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

