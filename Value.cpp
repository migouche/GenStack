//
// Created by migouche on 14/07/23.
//

#include <iostream>
#include "Value.h"
#include "Stack.h"
#include "Variables.h"
#include "FunctionContent.h"

std::shared_ptr<Value> Value::parse(ParserStream &s)
{
    for(const auto& op: Value::parsers)
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



std::tuple<bool, std::shared_ptr<Value>> LambdaValue::try_parse(ParserStream &s)
{
    if(s.peek_token() != "[")
        return std::tuple(false, nullptr);
    s.get_token(); // consume the "["
    auto lambda = std::make_shared<LambdaValue>();
    while (s.peek_token() != "]")
        lambda->add_value(Value::parse(s));
    s.get_token(); // consume the "]"
    return std::tuple(true, lambda);
}

// hacky static initializer for auto-parser functions
// (nameless namespace to make them "private")
namespace
{
    bool _int_value = [](){ Value::parsers.emplace_front(IntValue::try_parse); return true; }();
    bool _string_value = [](){ Value::parsers.emplace_front(StringValue::try_parse); return true; }();
    bool _lambdaCalue = [](){ Value::parsers.emplace_front(LambdaValue::try_parse); return true; }();
    bool _bool_try = [](){ Value::parsers.emplace_front(BoolValue::try_parse); return true; }();
    bool _identifier_try = [](){ Value::parsers.emplace_front(IdentifierValue::try_parse); return true; }();
}

void Value::eval(crp_Stack  s) const {
    s->push(std::const_pointer_cast<Value>(this->shared_from_this()));
}

IntValue::IntValue(int i): data(i) {}

std::string IntValue::print_string() const { return std::to_string(data); }

int IntValue::get() const { return this->data;}



StringValue::StringValue(std::string s): data(std::move(s)) {}

std::string StringValue::print_string() const { return "\"" + this->get() + "\""; }

std::string StringValue::get() const { return this->data; }



OperationValue::OperationValue(std::function<void(crp_Stack )> op): op(std::move(op)) {}

std::string OperationValue::print_string() const { return "operation"; }

void OperationValue::eval(crp_Stack  s) const
{
    //std::cout << "evaluating\n";
    this->op(s);
    //std::cout << "evaluated\n";
}


std::string FunctionValue::print_string() const { return "function"; }

std::shared_ptr<FunctionContent> FunctionValue::get() const { return this->content; }

void FunctionValue::eval(crp_Stack  s) const {
    auto f = *this->get();
    for(const auto& v: f)
    {
        v->eval(s);
    }
}

BoolValue::BoolValue(bool b): data(b) {}

std::string BoolValue::print_string() const { return this->get() ? "true" : "false"; }

bool BoolValue::get() const { return this->data; }

std::tuple<bool, std::shared_ptr<Value>> BoolValue::try_parse(ParserStream &s) {
    if(s.peek_token() == "true")
    {
        s.get_token();
        return {true, std::make_shared<BoolValue>(true)};
    }
    else if(s.peek_token() == "false")
    {
        s.get_token();
        return {true, std::make_shared<BoolValue>(false)};
    }
    return {false, nullptr};
}


IdentifierValue::IdentifierValue(std::string name): name(std::move(name)) {}

std::string IdentifierValue::print_string() const { return "identifier: " + this->name; }

std::string IdentifierValue::get() const { return this->name; }

void IdentifierValue::eval(crp_Stack s) const {
    Variables::get_variable(this->name)->eval(s);
}

std::tuple<bool, std::shared_ptr<Value>> IdentifierValue::try_parse(ParserStream &s) {
    if (Variables::exists(s.peek_token())) {
        auto name = s.get_token();
        return {true, std::make_shared<IdentifierValue>(name)};
    }
    return {false, nullptr};
}

FunctionValue::FunctionValue(const std::shared_ptr<FunctionContent> &c) {
    this->content = c;
}

LambdaValue::LambdaValue()
{
    this->content = std::make_shared<FunctionContent>();
}

std::string LambdaValue::print_string() const { return "lambda"; }

void LambdaValue::add_value(const std::shared_ptr<Value>& v)
{
    this->content->push(v);
}

std::shared_ptr<FunctionContent> LambdaValue::get() const { return this->content; }

void LambdaValue::eval(crp_Stack s) const {
    s->push(std::make_shared<FunctionValue>(this->content));
}
// [ 'func true if ] eval