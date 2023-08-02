//
// Created by migouche on 4/07/23.
//

#ifndef GENSTACK_VALUE_H
#define GENSTACK_VALUE_H

#include <string>
#include <functional>
#include "Stack.h"
#include "memory"

class Value : public std::enable_shared_from_this<Value>
{
public:
    //virtual void pushToStack(Stack*) = 0;
    virtual std::string to_string() const = 0;
    virtual std::string print_string() const = 0;

    template <class T>
    std::shared_ptr<T> as()
    {
        auto p = std::dynamic_pointer_cast<T>(this->shared_from_this());
        if(p == nullptr)
            throw std::runtime_error("couldn't cast Value to type");
        return p;
    }

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
    explicit FunctionValue(const Stack& stack); // copy constructor
    explicit FunctionValue(std::function<void(Stack*)> op);

    std::string to_string() const override;
    std::string print_string() const override;

    Stack get() const;

    void eval(Stack* s) const; // will just "unpack onto stack and evaluate"
};

template <typename T>
bool is_value_of_type(const std::shared_ptr<Value>& v)
{
    return std::dynamic_pointer_cast<T>(v) != nullptr;
}
#endif //GENSTACK_VALUE_H
