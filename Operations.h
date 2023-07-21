//
// Created by migouche on 12/07/23.
//

#ifndef GENSTACK_OPERATIONS_H
#define GENSTACK_OPERATIONS_H

#include <map>
#include <functional>

class Stack;


class Operations {
private:

    static std::map<std::string, std::function<void(Stack*)>> operations;
public:
    static std::function<void(Stack*)> get_operation(const std::string& name);
};

#endif //GENSTACK_OPERATIONS_H
