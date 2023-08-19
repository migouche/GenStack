//
// Created by migouche on 14/07/23.
//

#include <iostream>
#include "Value.h"
#include "Stack.h"
#include "Variables.h"

std::shared_ptr<Value> Value::parse(ParserStream &s)
{
    for(auto op: Value::parsers)
    {
        auto [b, v] = op(s);
        if(b)
            return v;
    }
    throw std::runtime_error("Couldn't parse, peeked token: " + s.peek_token());
}

std::list<std::function<std::tuple<bool, std::shared_ptr<Value>>(ParserStream&)>> Value::parsers =
        std::list<std::function<std::tuple<bool, std::shared_ptr<Value>>(ParserStream&)>>();
// compiler wil complain if i write auto instead of the full type


std::tuple<bool, std::shared_ptr<Value>> IntValue::try_parse(ParserStream &s) {
    if(!(isdigit(s.peek_token()[0]) || (s.peek_token()[0] == '-' && s.peek_token().length() > 1))) // i'll use morgan later :V
        return std::tuple(false, nullptr);
    return std::tuple(true,
                      std::make_shared<IntValue>(strtol(s.get_token().c_str(), nullptr, 10)));
}

std::tuple<bool, std::shared_ptr<Value>> StringValue::try_parse(ParserStream &s)
{
    if(s.peek_token()[0] != '"')
        return std::tuple(false, nullptr);
    auto str = s.get_token();
    str.erase(str.begin());
    if(str[str.length() - 1] == '"')
        str.erase(str.end() - 1);
    return std::tuple(true, std::make_shared<StringValue>(str));
}



std::tuple<bool, std::shared_ptr<Value>> FunctionValue::try_parse(ParserStream &s)
{
    if(s.peek_token() != "[")
        return std::tuple(false, nullptr);
    s.get_token(); // consume the "["
    auto stack = std::make_shared<Stack>();
    while (s.peek_token() != "]")
        stack->push(Value::parse(s));
    s.get_token(); // consume the "]"
    return std::tuple(true, std::make_shared<FunctionValue>(stack));
}

// hacky static initializer for auto-parser functions
// (nameless namespace to make them "private")
namespace
{
    std::tuple<bool, std::shared_ptr<Value>> try_variable(ParserStream &s)
    {
        auto str = s.peek_token();
        bool eval = false;
        if(str[0] == '\'')
            str.erase(str.begin());
        else
            eval = true;
        if(Variables::exists(str))
        {
            s.get_token(); // consume it
            if(eval)
                return {true, Variables::get_variable(str)};
            else
                return {true, std::make_shared<FunctionValue>([str](const std::shared_ptr<Stack>& s) {
                    s->push(Variables::get_variable(str));
                })};
        }
        return std::tuple(false, nullptr);
    }

    bool _int_value = [](){ Value::parsers.emplace_front(IntValue::try_parse); return true; }();
    bool _string_value = [](){ Value::parsers.emplace_front(StringValue::try_parse); return true; }();
    bool _function_value = [](){ Value::parsers.emplace_front(FunctionValue::try_parse); return true; }();
    //bool _operation_value = [](){ Value::parsers.emplace_front(OperationValue::try_parse); return true; }();
    //bool _operation_try = [](){ Value::parsers.emplace_front(try_operation); return true; }();
    bool _variable_try = [](){ Value::parsers.emplace_front(try_variable); return true; }();

}

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

FunctionValue::FunctionValue(const std::function<void(const std::shared_ptr<Stack>&)>& op) {
    this->stack = std::make_shared<Stack>();
    this->stack->push(std::make_shared<OperationValue>(op));
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
