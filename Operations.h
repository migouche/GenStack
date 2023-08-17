//
// Created by migouche on 12/07/23.
//

#ifndef GENSTACK_OPERATIONS_H
#define GENSTACK_OPERATIONS_H

#include <map>
#include <functional>
#include <memory>

class Stack;


class Operations {
private:

    static std::map<std::string, std::function<void(const std::shared_ptr<Stack>&)>> operations;
public:
    static std::function<void(const std::shared_ptr<Stack>&)> get_operation(const std::string& name);
    static bool operation_exists(const std::string& name);
};

#endif //GENSTACK_OPERATIONS_H
