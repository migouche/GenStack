//
// Created by migouche on 28/07/23.
//

#include "Variables.h"
#include "Value.h"
#include "Stack.h"
#include <iostream>

std::map<std::string, std::shared_ptr<Value>> &Variables::variables() { // anti-initialization order fiasco device
    static std::map<std::string, std::shared_ptr<Value>> variables;
    return variables;
}

bool Variables::exists(const std::string &name) {
    return variables().find(name) != variables().end();
}

std::shared_ptr<Value> Variables::get_variable(const std::string &name) {
    if (variables().find(name) == variables().end())
        throw std::runtime_error("Variable " + name + " not found");
    return variables().at(name);
}

void Variables::set_variable(const std::string &name, const std::shared_ptr<Value> &value) {
    if(Variables::exists(name))
        variables().find(name)->second = value;
    else
        variables().insert({name, value});
}

void Variables::push_variable(const std::string &name, const std::shared_ptr<Stack>& s) {
    if (variables().find(name) == variables().end())
        throw std::runtime_error("Variable " + name + " not found");
    variables().at(name)->eval(s);
}

void Variables::push_variable_no_eval(const std::string &name, const std::shared_ptr<Stack>& s) {
    if (variables().find(name) == variables().end())
        throw std::runtime_error("Variable " + name + " not found");
    auto v = variables().at(name);
    if(!is_value_of_type<FunctionValue>(v) && !is_value_of_type<OperationValue>(v))
        throw std::runtime_error("Variable " + name + " is not a function nor an operation");
    s->push(v);
}


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

// static initializer to move functions to variables
namespace
{
    bool _create_variables = [](){
        Variables::set_variable("+", std::make_shared<OperationValue>(int_add));
        Variables::set_variable("-", std::make_shared<OperationValue>(int_sub));
        Variables::set_variable("*", std::make_shared<OperationValue>(int_prod));
        Variables::set_variable("eval", std::make_shared<OperationValue>(eval));
        Variables::set_variable("swap", std::make_shared<OperationValue>(swap));
        Variables::set_variable("pop", std::make_shared<OperationValue>(pop));
        Variables::set_variable("cat", std::make_shared<OperationValue>(cat));
        Variables::set_variable("store", std::make_shared<OperationValue>(store));
        Variables::set_variable("for", std::make_shared<OperationValue>(loop));

        return true;
    }();
}
