//
// Created by migouche on 28/07/23.
//

#include "Variables.h"
#include "Value.h"
#include "Stack.h"

std::map<std::string, std::shared_ptr<Value>> Variables::variables = {};

bool Variables::exists(const std::string &name) {
    return variables.find(name) != variables.end();
}


void Variables::set_variable(const std::string &name, const std::shared_ptr<Value> &value) {
    if (variables.find(name) != variables.end())
        throw std::runtime_error("Variable " + name + " already exists"); // for now they constants
    variables.insert({name, value});
}

void Variables::push_variable(const std::string &name, Stack *s) {
    if (variables.find(name) == variables.end())
        throw std::runtime_error("Variable " + name + " not found");
    auto var = variables.at(name);
    if (is_value_of_type<FunctionValue>(var))
    {
        auto func = std::dynamic_pointer_cast<FunctionValue>(var);
        func->eval(s);
    }
    else
        s->push(var);
}