//
// Created by migouche on 14/07/23.
//

#include "Value.h"
#include "Stack.h"

IntValue::IntValue(int i): data(i) {}

std::string IntValue::to_string() const { return  std::to_string(data); }
std::string IntValue::print_string() const { return to_string(); }

int IntValue::get() const { return this->data;}

StringValue::StringValue(std::string s): data(std::move(s)) {}

std::string StringValue::to_string() const { return this->get(); }
std::string StringValue::print_string() const { return "\"" + this->get() + "\""; }

std::string StringValue::get() const { return this->data; }

OperationValue::OperationValue(std::function<void(Stack *)> op): op(std::move(op)) {}

std::string OperationValue::to_string() const { return "operation"; }
std::string OperationValue::print_string() const { return this->to_string(); }

void OperationValue::eval(Stack *s) const { this->op(s); }