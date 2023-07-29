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


FunctionValue::FunctionValue(const std::list<std::shared_ptr<Value>> &values, bool reverse_order)
{
    if (reverse_order)
        for (auto it = values.rbegin(); it != values.rend(); it++) // cant do range in reverse
            stack.push(*it);
    else
        for (const auto & value : values)
            stack.push(value);
}

FunctionValue::FunctionValue(const Stack &stack) {
    this->stack = stack.copy();
}

FunctionValue::FunctionValue(std::function<void(Stack *)> op) {
    this->stack = Stack();
    this->stack.push(std::make_shared<OperationValue>([op](Stack* s){op(s); }));
}

std::string FunctionValue::to_string() const { return "function"; }
std::string FunctionValue::print_string() const { return "function"; }

Stack FunctionValue::get() const { return this->stack; }

void FunctionValue::eval(Stack *s) {
    auto f = this->get();
    while (f.length() > 0) {
        auto v = f.pop();
        if(is_value_of_type<OperationValue>(v))
            std::dynamic_pointer_cast<OperationValue>(v)->eval(s);
        else
            s->push(v);
    }
}
