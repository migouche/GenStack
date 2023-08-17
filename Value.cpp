//
// Created by migouche on 14/07/23.
//

#include <iostream>
#include "Value.h"
#include "Stack.h"

void Value::eval(const std::shared_ptr<Stack>& s) const {
    s->push(std::const_pointer_cast<Value>(this->shared_from_this()));
}

IntValue::IntValue(int i): data(i) {}

std::string IntValue::to_string() const { return  std::to_string(data); }
std::string IntValue::print_string() const { return to_string(); }

int IntValue::get() const { return this->data;}



StringValue::StringValue(std::string s): data(std::move(s)) {}

std::string StringValue::to_string() const { return this->get(); }
std::string StringValue::print_string() const { return "\"" + this->get() + "\""; }

std::string StringValue::get() const { return this->data; }



OperationValue::OperationValue(std::function<void(const std::shared_ptr<Stack>&)> op): op(std::move(op)) {}

std::string OperationValue::to_string() const { return "operation"; }
std::string OperationValue::print_string() const { return this->to_string(); }

void OperationValue::eval(const std::shared_ptr<Stack>& s) const
{
    //std::cout << "evaluating\n";
    this->op(s);
    //std::cout << "evaluated\n";
}


FunctionValue::FunctionValue(const std::shared_ptr<Stack>& stack) {
    this->stack = stack->copy();
}


std::string FunctionValue::to_string() const { return "function"; }
std::string FunctionValue::print_string() const { return "function"; }

std::shared_ptr<Stack> FunctionValue::get() const { return this->stack; }

void FunctionValue::eval(const std::shared_ptr<Stack>& s) const {
    auto f = this->get()->copy();
    while (f->length() > 0)
    {
        auto v = f->pop_back();
        v->eval(s);
    }
}
