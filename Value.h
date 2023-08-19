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

class Stack;

class Value : public std::enable_shared_from_this<Value>
{
public:
    //virtual void pushToStack(Stack*) = 0;
    virtual std::string to_string() const = 0;
    virtual std::string print_string() const = 0;
    virtual void eval(const std::shared_ptr<Stack>& s) const;

    static std::shared_ptr<Value> parse(ParserStream& s);

    template <class T>
    std::shared_ptr<T> as()
    {
        auto p = std::dynamic_pointer_cast<T>(this->shared_from_this());
        if(p == nullptr)
            throw std::runtime_error("couldn't cast " +
            this->print_string() +
            " to type " +
            (typeid(T).name() + 1) // FIXME: name() returns different stuff for different compilers, may or may not work
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

    std::string to_string() const override;
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

    std::string to_string() const override;
    std::string print_string() const override;

    std::string get() const;
    static std::tuple<bool, std::shared_ptr<Value>> try_parse(ParserStream& s);
private:
    std::string data;
};

class OperationValue: public Value // this is more like "primitive" functions
{
public:
    explicit OperationValue(std::function<void(const std::shared_ptr<Stack>&)> op);

    std::string to_string() const override;
    std::string print_string() const override;

    void eval(const std::shared_ptr<Stack>& s) const override;

    [[maybe_unused]] [[maybe_unused]] static std::tuple<bool, std::shared_ptr<Value>> try_parse(ParserStream& s);
private:
    //void (*op)(Stack*);
    std::function<void(const std::shared_ptr<Stack>&)> op;
};


class FunctionValue: public Value
{
private:
    std::shared_ptr<Stack> stack;
public:
    explicit FunctionValue(const std::shared_ptr<Stack>&); // copy constructor
    explicit FunctionValue(const std::function<void(const std::shared_ptr<Stack>&)>& op); // constructor from operation (will create a stack with the operation in it)

    std::string to_string() const override;
    std::string print_string() const override;

    std::shared_ptr<Stack> get() const;

    void eval(const std::shared_ptr<Stack>& s) const override;
    static std::tuple<bool, std::shared_ptr<Value>> try_parse(ParserStream& s);

};

template <typename T>
bool is_value_of_type(const std::shared_ptr<Value>& v)
{
    return std::dynamic_pointer_cast<T>(v) != nullptr;
}
#endif //GENSTACK_VALUE_H
