//
// Created by migouche on 4/07/23.
//

#ifndef GENSTACK_VALUE_H
#define GENSTACK_VALUE_H

#include <string>
#include <functional>
#include "Stack.h"
#include "memory"

class Value
{
public:
    //virtual void pushToStack(Stack*) = 0;
    virtual std::string to_string() const = 0;
    virtual std::string print_string() const = 0;

    virtual ~Value() = default;
};


class IntValue: public Value
{
public:
    explicit IntValue(int i);

    std::string to_string() const override;
    std::string print_string() const override;

    int get() const;

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

private:
    std::string data;
};

class OperationValue: public Value // this is more like "primitive" functions
{
public:
    explicit OperationValue(std::function<void(Stack *)> op);

    std::string to_string() const override;
    std::string print_string() const override;

    void eval(Stack* s) const;

private:
    //void (*op)(Stack*);
    std::function<void(Stack*)> op;
};

class FunctionValue: public Value
{
private:
    Stack stack;
public:
    FunctionValue(const std::list<std::shared_ptr<Value>>& values, bool reverse_order);
    explicit FunctionValue(const Stack& stack); // copy constructor
    explicit FunctionValue(std::function<void(Stack*)> op); // this is to cast an operation to a function

    std::string to_string() const override;
    std::string print_string() const override;

    Stack get() const;

    void eval(Stack* s); // will just "unpack onto stack and evaluate"
};

template <typename T>
bool is_value_of_type(const std::shared_ptr<Value>& v)
{
    return std::dynamic_pointer_cast<T>(v) != nullptr;
}
#endif //GENSTACK_VALUE_H
