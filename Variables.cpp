//
// Created by migouche on 28/07/23.
//

#include "Variables.h"
#include "Value.h"
#include "Stack.h"
#include <iostream>

std::map<std::string, std::shared_ptr<Value>> &Variables::variables() { //Anti-Initialization-Order-Fiasco-Device(AIOFD)
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

void Variables::push_variable(const std::string &name, crp_Stack  s) {
    if (variables().find(name) == variables().end())
        throw std::runtime_error("Variable " + name + " not found");
    variables().at(name)->eval(s);
}

void Variables::push_variable_no_eval(const std::string &name, crp_Stack  s) {
    if (variables().find(name) == variables().end())
        throw std::runtime_error("Variable " + name + " not found");
    auto v = variables().at(name);
    if(!is_value_of_type<FunctionValue>(v) && !is_value_of_type<OperationValue>(v))
        throw std::runtime_error("Variable " + name + " is not a function nor an operation");
    s->push(v);
}


void int_add(crp_Stack s) {
    auto [i1, i2] = s->get<IntValue, IntValue>();
    s->push(std::make_shared<IntValue>(i1->get() + i2->get()));
}

void int_sub(crp_Stack s) {
    auto [i1, i2] = s->get<IntValue, IntValue>();
    s->push(std::make_shared<IntValue>(i2->get() - i1->get()));
}

void int_prod(crp_Stack  s)
{
    auto[i1, i2] = s->get<IntValue, IntValue>();
    s->push(std::make_shared<IntValue>(i1->get() * i2->get()));
}

void eval(crp_Stack s) { //NOTE: will eval numbers too, be careful
    auto [op] = s->get<Value>();
    op->eval(s);
}

void swap(crp_Stack s) {
    auto [v1, v2] = s->get<Value, Value>();
    s->push(v1);
    s->push(v2);
}

void pop(crp_Stack s) {
    if (s->length() < 1)
        throw std::runtime_error("Stack must have 1 element at least");
    s->pop();
}

void cat(crp_Stack s) {
    auto [s1, s2] = s->get<StringValue, StringValue>();

    s->push(std::make_shared<StringValue>(s2->get() + s1->get()));
}

void store(crp_Stack s)
{
    auto [name, val] = s->get<StringValue, Value>();
    Variables::set_variable(name->get(), val);
}

void loop(crp_Stack s)
{
    auto [n, op] = s->get<IntValue, Value>();
    if(!is_value_of_type<OperationValue>(op) && !is_value_of_type<FunctionValue>(op))
        throw std::runtime_error("Cannot loop on " + op->print_string());

    for(int i = 0; i < n->get(); i++)
        op->eval(s);
}

void if_no_else(crp_Stack  s)
{
    auto [f,b] = s->get<FunctionValue, BoolValue>();
    // order is reversed because it pops normally from the stack
    if(b->get())
        f->eval(s);
}

void dup(crp_Stack s)
{
    s->push(s->peek());
}

void eq(crp_Stack s)
{
    auto [v1, v2] = s->get<IntValue, IntValue>();
    s->push(std::make_shared<BoolValue>(v1->get() == v2->get()));
}

void neq(crp_Stack s)
{
    auto [v1, v2] = s->get<IntValue, IntValue>();
    s->push(std::make_shared<BoolValue>(v1->get() != v2->get()));
}

void gt(crp_Stack s)
{
    auto [v1, v2] = s->get<IntValue, IntValue>();
    s->push(std::make_shared<BoolValue>(v2->get() > v1->get()));
}

void lt(crp_Stack s)
{
    auto [v1, v2] = s->get<IntValue, IntValue>();
    s->push(std::make_shared<BoolValue>(v2->get() < v1->get()));
}

void gte(crp_Stack s)
{
    auto [v1, v2] = s->get<IntValue, IntValue>();
    s->push(std::make_shared<BoolValue>(v2->get() >= v1->get()));
}

void lte(crp_Stack s)
{
    auto [v1, v2] = s->get<IntValue, IntValue>();
    s->push(std::make_shared<BoolValue>(v2->get() <= v1->get()));
}

void while_loop(crp_Stack s)
{
    auto [func, cond] = s->get<FunctionValue, FunctionValue>();
    cond->eval(s);
    while(s->pop()->as<BoolValue>()->get())
    {
        func->eval(s);
        cond->eval(s);
    }
}

void if_else(crp_Stack s)
{
    auto [f2 /* else */, f1 /* if */, b] =
            s->get<FunctionValue, FunctionValue, BoolValue>();
    // we first pop the "else func", then the "if func" and then the bool
    if(b->get())
        f1->eval(s);
    else
        f2->eval(s);
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
        Variables::set_variable("if", std::make_shared<OperationValue>(if_no_else));
        Variables::set_variable("if_else", std::make_shared<OperationValue>(if_else));
        Variables::set_variable("dup", std::make_shared<OperationValue>(dup));
        Variables::set_variable("==", std::make_shared<OperationValue>(eq));
        Variables::set_variable("!=", std::make_shared<OperationValue>(neq));
        Variables::set_variable(">", std::make_shared<OperationValue>(gt));
        Variables::set_variable("<", std::make_shared<OperationValue>(lt));
        Variables::set_variable(">=", std::make_shared<OperationValue>(gte));
        Variables::set_variable("<=", std::make_shared<OperationValue>(lte));
        Variables::set_variable("while", std::make_shared<OperationValue>(while_loop));

        return true;
    }();
}
