//
// Created by kwp on 2017/10/2.
//

#ifndef TASK2_CPP_REGULAREXPRESSION_H
#define TASK2_CPP_REGULAREXPRESSION_H

#include <iostream>
#include "Lexer.h"
#include "DFA.h"
#include "SyntaxTree.h"


class RegularExpression {
public:
    RegularExpression(std::string pattern);  // construction

    RegularExpression(const char pattern[1]);

    bool match(std::string str);

    std::vector<std::string> find_all(std::string str);

private:
    DFA dfa;
};


#endif //TASK2_CPP_REGULAREXPRESSION_H
