#include <iostream>
#include "Value.h"
#include "Stack.h"
#include "Parser.h"

using namespace std;

int main() {
    Stack s = Parser::interactive_parse();
    std::cout << "Last element is: " << s.peek()->print_string() << std::endl;

    return 0;
}


/*
5.6
[ 10 * 1 + ]
3
for


3 +

[ 3 + ] eval
 */