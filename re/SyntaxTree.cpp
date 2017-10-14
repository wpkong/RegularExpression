//
// Created by kwp on 2017/10/11.
//

#include "SyntaxTree.h"

#define CHECK_STACK(condition) \
while (condition) { \
    re_node_ptr node; \
    token_ptr token; \
    token = stack_op.top(); \
    stack_op.pop(); \
    node = std::shared_ptr<SyntaxTreeNode>(new SyntaxTreeNode(token)); \
    node->ch_node2 = stack_char.top();\
    stack_char.pop();\
    node->ch_node1 = stack_char.top();\
    stack_char.pop();\
    stack_char.push(node);\
}

SyntaxTree::SyntaxTree(Lexer &lexer) {
    root = std::shared_ptr<SyntaxTreeNode>(new SyntaxTreeNode(TokenType::TOKEN_CONCAT));
    //上一个token的类型
    TokenType::token_type last_type = TokenType::TOKEN_END;
    //是否在范围内
    int is_in_range = 0;
    //迭代输出Token
    for (token_ptr cur = lexer.get_token(); cur != nullptr; cur = lexer.get_token()) {
//        std::cout << cur->get_char() << cur->get_type() << std::endl;
        switch (cur->get_type()) {
            case TokenType::TOKEN_END: {
                CHECK_STACK (stack_op.size() != 0)
                if (stack_char.size() != 0) {
                    root->ch_node1 = stack_char.top();
                    root->ch_node2 = std::shared_ptr<SyntaxTreeNode>(new SyntaxTreeNode(cur));
                }
                break;
            }
            case TokenType::TOKEN_UNION: {
                CHECK_STACK (stack_op.size() != 0 &&
                             (stack_op.top()->get_type() == TokenType::TOKEN_CONCAT ||
                              stack_op.top()->get_type() == TokenType::TOKEN_UNION))
                stack_op.push(cur);
                break;
            }
            case TokenType::TOKEN_LEFT_SQUARE:
            case TokenType::TOKEN_LEFT_BRACKET: {
                if (last_type != TokenType::TOKEN_LEFT_SQUARE && last_type != TokenType::TOKEN_LEFT_BRACKET &&
                    last_type != TokenType::TOKEN_UNION && last_type != TokenType::TOKEN_END) {
                    CHECK_STACK(stack_op.size() != 0 && (stack_op.top()->get_type() == TokenType::TOKEN_CONCAT))
                    token_ptr cat = std::shared_ptr<Token>(new Token());
                    cat->set_type(is_in_range != 0 ? TokenType::TOKEN_UNION : TokenType::TOKEN_CONCAT);
                    stack_op.push(cat);
                }
                stack_op.push(cur);
                if (cur->get_type() == TokenType::TOKEN_LEFT_SQUARE) is_in_range++;
                break;
            }
            case TokenType::TOKEN_PLUS:
            case TokenType::TOKEN_STAR:
            case TokenType::TOKEN_QUESTION: {
                if (stack_char.size() == 0) {
                    throw ReError("Unexpected token", cur->get_pos());
                }
                re_node_ptr node = std::shared_ptr<SyntaxTreeNode>(new SyntaxTreeNode(cur));
                node->ch_node1 = stack_char.top();
                stack_char.pop();
                stack_char.push(node);
                break;
            }
            case TokenType::TOKEN_RANGE_CONNECT: {
                if (stack_char.size() == 0 || last_type != TokenType::TOKEN_NORMAL) {
                    throw ReError("Unexpected token", cur->get_pos());
                }
                stack_op.push(cur);
                break;
            }
            case TokenType::TOKEN_RIGHT_BRACKET: {
                CHECK_STACK(stack_op.size() != 0 && stack_op.top()->get_type() != TokenType::TOKEN_LEFT_BRACKET)
                if (stack_op.top()->get_type() == TokenType::TOKEN_LEFT_BRACKET) stack_op.pop();
                break;
            }
            case TokenType::TOKEN_RIGHT_SQUARE: {
                CHECK_STACK(stack_op.size() != 0 && stack_op.top()->get_type() != TokenType::TOKEN_LEFT_SQUARE)
                if (stack_op.top()->get_type() == TokenType::TOKEN_LEFT_SQUARE) stack_op.pop();
                is_in_range--;
                break;
            }
            case TokenType::TOKEN_SPECIAL:
            case TokenType::TOKEN_ANY:
            case TokenType::TOKEN_NORMAL: {
                re_node_ptr node = std::shared_ptr<SyntaxTreeNode>(new SyntaxTreeNode(cur));
                //检查是否需要将concat类型压入操作符栈
                if (stack_char.size() == 0 || last_type == TokenType::TOKEN_END ||
                    last_type == TokenType::TOKEN_LEFT_BRACKET || last_type == TokenType::TOKEN_LEFT_SQUARE ||
                    last_type == TokenType::TOKEN_RANGE_CONNECT || last_type == TokenType::TOKEN_UNION) {

                    stack_char.push(node);
                } else {
                    token_ptr cat = std::shared_ptr<Token>(new Token());
                    cat->set_type(is_in_range != 0 ? TokenType::TOKEN_UNION : TokenType::TOKEN_CONCAT);
                    CHECK_STACK(stack_op.size() != 0 && (stack_op.top()->get_type() == TokenType::TOKEN_CONCAT ||
                                                         stack_op.top()->get_type() == TokenType::TOKEN_RANGE_CONNECT))
                    stack_op.push(cat);
                    stack_char.push(node);
                }
                break;
            }
            default:
                throw ReError("Unexpected type", cur->get_pos());
        }
        last_type = cur->get_type();
    }
}


void SyntaxTree::adjust_tree() {
    if (root) root->adjust();
}

void SyntaxTree::print() {
    root->print();
}
