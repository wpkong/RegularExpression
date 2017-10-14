//
// Created by kwp on 2017/10/3.
//

#include "Lexer.h"

Lexer::Lexer(std::string str) : pattern(str), pattern_ptr(0), is_in_range(0) {
}

token_ptr Lexer::get_token() {
    token_ptr ptr = std::make_shared<Token>();
    if (pattern_ptr == pattern.length()) {
        pattern_ptr++;
        return ptr;
    } else if (pattern_ptr > pattern.length()){
        return nullptr;
    }
    char ch = pattern[pattern_ptr++];
    ptr->set_char(ch);
    ptr->set_pos(pattern_ptr);
    switch (ch){
        case '*':
            ptr->set_type(TokenType::TOKEN_STAR);
            break;
        case '+':
            ptr->set_type(TokenType::TOKEN_PLUS);
            break;
        case '(':
            ptr->set_type(TokenType::TOKEN_LEFT_BRACKET);
            break;
        case ')':
            ptr->set_type(TokenType::TOKEN_RIGHT_BRACKET);
            break;
        case '[':
            ptr->set_type(TokenType::TOKEN_LEFT_SQUARE);
            is_in_range++;
            break;
        case ']':
            ptr->set_type(TokenType::TOKEN_RIGHT_SQUARE);
            is_in_range--;
            break;
        case '-':
            if(is_in_range == 0){
                ptr->set_type(TokenType::TOKEN_NORMAL);
                ptr->set_char('-');
            } else{
                ptr->set_type(TokenType::TOKEN_RANGE_CONNECT);
            }
            break;
        case '|':
            ptr->set_type(TokenType::TOKEN_UNION);
            break;
        case '.':
            ptr->set_type(TokenType::TOKEN_ANY);
            break;
        case '?':
            ptr->set_type(TokenType::TOKEN_QUESTION);
            break;
        case '\\': {
            if (pattern_ptr >= pattern.length()) {
                throw ReError("Expecting an esc", pattern_ptr);
            }
            char nxt_ch = pattern[pattern_ptr++];
            if (nxt_ch == 'w' || nxt_ch == 'W' || nxt_ch == 'd' || nxt_ch == 'D' || nxt_ch == 's' || nxt_ch == 'S') {
                ptr->set_type(TokenType::TOKEN_SPECIAL);
                ptr->set_char(nxt_ch);
            } else if (nxt_ch == '*' || nxt_ch == '+' || nxt_ch == '(' || nxt_ch == ')' || nxt_ch == '[' ||
                       nxt_ch == ']' || nxt_ch == '-' || nxt_ch == '\\' || nxt_ch == '|' || nxt_ch == '?' || nxt_ch == '.') {
                ptr->set_type(TokenType::TOKEN_NORMAL);
                ptr->set_char(nxt_ch);
                ptr->set_pos(pattern_ptr);
            } else {
                throw ReError("Unexpected token", pattern_ptr);
            }
            break;
        }
        default:
            ptr->set_char(ch);
            ptr->set_type(TokenType::TOKEN_NORMAL);
    }
    return ptr;
}
