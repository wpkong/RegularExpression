//
// Created by kwp on 2017/10/3.
//

#ifndef TASK2_CPP_RELEXER_H
#define TASK2_CPP_RELEXER_H

#include <iostream>
#include <string>
#include "Token.h"
#include "ReError.h"

class Lexer {
public:
    Lexer(std::string str);
    token_ptr get_token();

private:


private:
    std::string pattern;
    int pattern_ptr;
    int is_in_range;
};


#endif //TASK2_CPP_RELEXER_H
