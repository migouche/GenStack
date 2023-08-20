//
// Created by migouche on 4/07/23.
//

#ifndef GENSTACK_VALUE_H
#define GENSTACK_VALUE_H

#include <string>
#include <functional>
#include <stdexcept>
#include <list>
#include "memory"
#include "ParserStream.h"
#include "Stack.dcl"

class FunctionContent;

class Value : public std::enable_shared_from_this<Value>
{
public:
    virtual std::string print_string() const = 0;
    virtual void eval(crp_Stack  s) const;

    static std::shared_ptr<Value> parse(ParserStream& s);

    template <class T>
    std::shared_ptr<T> as()
    {
        auto p = std::dynamic_pointer_cast<T>(this->shared_from_this());
        if(p == nullptr)
            throw std::runtime_error("couldn't cast " +
            this->print_string() +
            " to type " +
            (typeid(T).name() + 1) // NOTE: name() returns different stuff for different compilers, may or may not work
            );
        return p;
    }

    virtual ~Value() = default;
    static std::list<std::function<std::tuple<bool, std::shared_ptr<Value>>(ParserStream&)>> parsers;
};


class IntValue: public Value
{
public:
    explicit IntValue(int i);

    std::string print_string() const override;


    int get() const;

    static std::tuple<bool, std::shared_ptr<Value>> try_parse(ParserStream& s);
private:
    int data;
};


class StringValue: public Value
{
public:
    explicit StringValue(std::string  s);

    std::string print_string() const override;

    std::string get() const;
    static std::tuple<bool, std::shared_ptr<Value>> try_parse(ParserStream& s);
private:
    std::string data;
};


class BoolValue: public Value
{
public:
    explicit BoolValue(bool b);

    std::string print_string() const override;

    bool get() const;
    static std::tuple<bool, std::shared_ptr<Value>> try_parse(ParserStream& s);
private:
    bool data;
};


class OperationValue: public Value // OperationValues point to a c++ function
{
public:
    explicit OperationValue(std::function<void(crp_Stack )> op);

    std::string print_string() const override;

    void eval(crp_Stack  s) const override;
private:
    std::function<void(crp_Stack )> op;
};


class FunctionValue: public Value
{
private:
    std::shared_ptr<FunctionContent> content;
public:
    explicit FunctionValue(const std::shared_ptr<FunctionContent>&);

    std::string print_string() const override;

    std::shared_ptr<FunctionContent> get() const;

    void eval(crp_Stack  s) const override;
    //static std::tuple<bool, std::shared_ptr<Value>> try_parse(ParserStream& s);

};

class IdentifierValue: public Value
{
private:
    std::string name;
public:
    explicit IdentifierValue(std::string name);

    std::string print_string() const override;

    std::string get() const;

    void eval(crp_Stack  s) const override;
    static std::tuple<bool, std::shared_ptr<Value>> try_parse(ParserStream& s);
};


class LambdaValue: public Value
{
private:
    std::shared_ptr<FunctionContent> content;
public:
    LambdaValue();

    std::string print_string() const override;

    std::shared_ptr<FunctionContent> get() const;

    void eval(crp_Stack  s) const override;
    static std::tuple<bool, std::shared_ptr<Value>> try_parse(ParserStream& s);

    void add_value(const std::shared_ptr<Value>& v);
};

template <typename T>
bool is_value_of_type(const std::shared_ptr<Value>& v)
{
    return std::dynamic_pointer_cast<T>(v) != nullptr;
}
#endif //GENSTACK_VALUE_H
