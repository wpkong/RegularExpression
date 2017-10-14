//
// Created by kwp on 2017/10/12.
//

#ifndef TASK2_CPP_SYNTAXTREENODE_H
#define TASK2_CPP_SYNTAXTREENODE_H

#include "ReError.h"
#include <vector>
#include "Lexer.h"

class DFA;
class SyntaxTreeNode;
typedef std::shared_ptr<SyntaxTreeNode> re_node_ptr;


class SyntaxTreeNode{
    friend class SyntaxTree;
    friend class DFA;
public:
    SyntaxTreeNode(token_ptr tk);
    SyntaxTreeNode(TokenType::token_type type);
    ~SyntaxTreeNode();
private:
    token_ptr token;
    re_node_ptr ch_node1;
    re_node_ptr ch_node2;

    bool nullable;
    std::vector<SyntaxTreeNode *> first_pos;
    std::vector<SyntaxTreeNode *> last_pos;
    std::vector<SyntaxTreeNode *> follow_pos;

    std::vector<SyntaxTreeNode *> delete_que;  //由本类申请到的node，由本类释放

private:
    TokenType::token_type get_type();
    void adjust();  // 调整结点的四个属性
    void print();
//    void add_new_char_node(char from, char to);

    static void concat_vector(std::vector<SyntaxTreeNode *> &v1, std::vector<SyntaxTreeNode *> &v2);
};


#endif //TASK2_CPP_SYNTAXTREENODE_H
