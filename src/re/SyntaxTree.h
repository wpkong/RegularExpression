//
// Created by kwp on 2017/10/11.
//

#ifndef TASK2_CPP_RETREE_H
#define TASK2_CPP_RETREE_H

#include <vector>
#include "Lexer.h"
#include <stack>
#include <tuple>
#include "ReError.h"
#include "SyntaxTreeNode.h"

class SyntaxTree {
    friend class DFA;
public:
    SyntaxTree(Lexer &lexer);
    void adjust_tree();
    void print();

private:
    std::stack<token_ptr> stack_op;  // 符号栈
    std::stack<re_node_ptr> stack_char;  // 结点栈

    re_node_ptr root;
};



#endif //TASK2_CPP_RETREE_H
