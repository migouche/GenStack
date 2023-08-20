//
// Created by migouche on 20/08/23.
//

#ifndef GENSTACK_FUNCTIONCONTENT_H
#define GENSTACK_FUNCTIONCONTENT_H

#include <list>
#include <memory>

class Value;

class FunctionContent { // this is a queue (FIFO)
    // note begin points to the same end as front
private:
    std::list<std::shared_ptr<Value>> content;
public:

    FunctionContent();

    std::_List_iterator<std::shared_ptr<Value>> begin();
    std::_List_iterator<std::shared_ptr<Value>> end();

    void push(const std::shared_ptr<Value>& v); // will push back to iterate front to back
};


#endif //GENSTACK_FUNCTIONCONTENT_H
