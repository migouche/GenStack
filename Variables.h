//
// Created by migouche on 28/07/23.
//

#ifndef GENSTACK_VARIABLES_H
#define GENSTACK_VARIABLES_H


#include <string>
#include <map>
#include <memory>

class Value;
class Stack;

class Variables {
private:
    static std::map<std::string, std::shared_ptr<Value>> variables;

public:
    static bool exists(const std::string& name);
    static std::shared_ptr<Value> get_variable(const std::string& name);
    static void set_variable(const std::string& name, const std::shared_ptr<Value>& value);
    static void push_variable(const std::string& name, const std::shared_ptr<Stack>& s);
    static void push_variable_no_eval(const std::string& name, const std::shared_ptr<Stack>& s);
};

#endif //GENSTACK_VARIABLES_H
