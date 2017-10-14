//
// Created by kwp on 2017/10/12.
//

#include "SyntaxTreeNode.h"
#include <iomanip>

#define ADD_NEW_CHAR_NODE(from, to) \
for(char ch = from; ch <= to; ++ch){ \
    SyntaxTreeNode* node = new SyntaxTreeNode(TokenType::TOKEN_NORMAL); \
    node->token->set_char(ch); \
    first_pos.push_back(node); \
    last_pos.push_back(node); \
    delete_que.push_back(node); \
}


SyntaxTreeNode::SyntaxTreeNode(token_ptr tk) : token(tk) {}


TokenType::token_type SyntaxTreeNode::get_type() {
    return token->get_type();
}

SyntaxTreeNode::SyntaxTreeNode(TokenType::token_type type) {
    token = std::shared_ptr<Token>(new Token());
    token->set_type(type);
}


void SyntaxTreeNode::adjust() {
    if (ch_node1) ch_node1->adjust();
    if (ch_node2) ch_node2->adjust();

    switch (get_type()) {
        case TokenType::TOKEN_END:
        case TokenType::TOKEN_NORMAL: {
            nullable = false;
            first_pos.push_back(this);
            last_pos.push_back(this);
            break;
        }
        case TokenType::TOKEN_ANY: {
            nullable = false;
            ADD_NEW_CHAR_NODE(32, 126)
            break;
        }
        case TokenType::TOKEN_SPECIAL: {
            nullable = false;
            switch (token->get_char()) {
                case 'd': { // 数字
                    ADD_NEW_CHAR_NODE('0', '9')
                    break;
                }
                case 'D': { // 非数字
                    ADD_NEW_CHAR_NODE(32, 47)
                    ADD_NEW_CHAR_NODE(58, 126)
                    break;
                }
                case 's': { // 空白符
                    ADD_NEW_CHAR_NODE('\t', '\t')
                    ADD_NEW_CHAR_NODE('\n', '\n')
                    ADD_NEW_CHAR_NODE('\r', '\r')
                    ADD_NEW_CHAR_NODE('\f', '\f')
                    ADD_NEW_CHAR_NODE('\v', '\v')
                    break;
                }
                case 'S': { // 非空白符
                    ADD_NEW_CHAR_NODE(32, 126)
                    break;
                }
                case 'w': { // a-zA-Z_0-9
                    ADD_NEW_CHAR_NODE('a', 'z')
                    ADD_NEW_CHAR_NODE('A', 'Z')
                    ADD_NEW_CHAR_NODE('0', '9')
                    ADD_NEW_CHAR_NODE('_', '_')
                    break;
                }
                case 'W': {
                    ADD_NEW_CHAR_NODE(32, 47)
                    ADD_NEW_CHAR_NODE(58, 96)
                    ADD_NEW_CHAR_NODE(123, 126)
                }
                default:
                    throw ReError("Illegal special token", token->get_pos());
            }
            break;
        }
        case TokenType::TOKEN_STAR: {
            nullable = true;
            concat_vector(first_pos, ch_node1->first_pos);
            concat_vector(last_pos, ch_node1->last_pos);
            // follow_pos
            for (auto item: last_pos) {
                concat_vector(item->follow_pos, first_pos);
            }
            break;
        }
        case TokenType::TOKEN_PLUS: {
            nullable = ch_node1->nullable;
            concat_vector(first_pos, ch_node1->first_pos);
            concat_vector(last_pos, ch_node1->last_pos);

            // follow_pos
            for (auto item: last_pos) {
                concat_vector(item->follow_pos, first_pos);
            }
            break;
        }
        case TokenType::TOKEN_QUESTION: {
            nullable = true;
            concat_vector(first_pos, ch_node1->first_pos);
            concat_vector(last_pos, ch_node1->last_pos);
            break;
        }
        case TokenType::TOKEN_RANGE_CONNECT: {
            nullable = false;
            char pre_ch = ch_node1->token->get_char();
            char last_ch = ch_node2->token->get_char();
            if (pre_ch > last_ch) {
                throw ReError("Illegal range token", ch_node1->token->get_pos() + 1);
            }
            if (pre_ch >= '0' && pre_ch <= '9' && last_ch >= '0' && last_ch <= '9' ||
                pre_ch >= 'a' && pre_ch <= 'z' && last_ch >= 'a' && last_ch <= 'z' ||
                pre_ch >= 'A' && pre_ch <= 'Z' && last_ch >= 'A' && last_ch <= 'Z') {

                ADD_NEW_CHAR_NODE(pre_ch, last_ch);

            } else {
                throw ReError("Unexpected token type before '-'", ch_node1->token->get_pos() + 1);
            }
            break;
        }
        case TokenType::TOKEN_UNION: {
            nullable = ch_node1->nullable || ch_node2->nullable;
            concat_vector(first_pos, ch_node1->first_pos);
            concat_vector(last_pos, ch_node1->last_pos);

            concat_vector(first_pos, ch_node2->first_pos);
            concat_vector(last_pos, ch_node2->last_pos);
            break;
        }
        case TokenType::TOKEN_CONCAT: {
            nullable = ch_node1->nullable && ch_node2->nullable;
            // 计算first
            concat_vector(first_pos, ch_node1->first_pos);

            if (ch_node1->nullable) {
                concat_vector(first_pos, ch_node2->first_pos);
            }
            // 计算last
            concat_vector(last_pos, ch_node2->last_pos);
            if (ch_node2->nullable) {
                concat_vector(last_pos, ch_node1->last_pos);
            }
            // 计算follow
            for (auto item: ch_node1->last_pos) {
                concat_vector(item->follow_pos, ch_node2->first_pos);
            }
            break;
        }

    }
}

void SyntaxTreeNode::print() {
    if (ch_node1) ch_node1->print();
    if (ch_node2) ch_node2->print();

    std::cout << token->get_char() << token->get_type() << "\tnullable: " << nullable << " \t first: ";
    for (auto i: first_pos) {
        std::cout << i->token->get_char() << i->get_type();
    }
    std::cout << std::setw(20) << "last: ";
    for (auto i: last_pos) {
        std::cout << i->token->get_char() << i->get_type();
    }
    std::cout << std::setw(20) << "follow（" << follow_pos.size() << "）: ";
    for (auto i: follow_pos) {
        std::cout << i->token->get_char() << i->get_type();
    }
    std::cout << " \n";
}

void SyntaxTreeNode::concat_vector(std::vector<SyntaxTreeNode *> &v1, std::vector<SyntaxTreeNode *> &v2) {
    for (SyntaxTreeNode *node: v2) {
        v1.push_back(node);
    }
}

SyntaxTreeNode::~SyntaxTreeNode() {
    for (SyntaxTreeNode *node: delete_que) {
        delete node;
    }
}

