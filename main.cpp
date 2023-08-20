#include "Stack.h"
#include "Parser.h"

using namespace std;

int main() {
    Stack s = Parser::interactive_parse();
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