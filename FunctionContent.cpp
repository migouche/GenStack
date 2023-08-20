//
// Created by migouche on 20/08/23.
//

#include "FunctionContent.h"

#include "Value.h"

FunctionContent::FunctionContent() {
    this->content = std::list<std::shared_ptr<Value>>();
}

std::_List_iterator<std::shared_ptr<Value>> FunctionContent::begin() {
    return this->content.begin();
}

std::_List_iterator<std::shared_ptr<Value>> FunctionContent::end() {
    return this->content.end();
}

void FunctionContent::push(const std::shared_ptr<Value> &v) {
    this->content.push_back(v);
}

