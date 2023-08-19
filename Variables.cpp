//
// Created by migouche on 28/07/23.
//

#include "Variables.h"
#include "Value.h"
#include "Stack.h"
#include <iostream>

std::map<std::string, std::shared_ptr<Value>> Variables::variables = {};

bool Variables::exists(const std::string &name) {
    return variables.find(name) != variables.end();
}

std::shared_ptr<Value> Variables::get_variable(const std::string &name) {
    if (variables.find(name) == variables.end())
        throw std::runtime_error("Variable " + name + " not found");
    return variables.at(name);
}

void Variables::set_variable(const std::string &name, const std::shared_ptr<Value> &value) {
    auto it = variables.find(name);
    if (it != variables.end())
        it->second = value;
    else
        variables.insert({name, value});
}

void Variables::push_variable(const std::string &name, const std::shared_ptr<Stack>& s) {
    if (variables.find(name) == variables.end())
        throw std::runtime_error("Variable " + name + " not found");
    variables.at(name)->eval(s);
}

void Variables::push_variable_no_eval(const std::string &name, const std::shared_ptr<Stack>& s) {
    if (variables.find(name) == variables.end())
        throw std::runtime_error("Variable " + name + " not found");
    auto v = variables.at(name);
    if(!is_value_of_type<FunctionValue>(v) && !is_value_of_type<OperationValue>(v))
        throw std::runtime_error("Variable " + name + " is not a function nor an operation");
    s->push(v);
}