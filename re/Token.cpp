//
// Created by kwp on 2017/10/10.
//

#include "Token.h"

Token::~Token() {
    token_count--;
}

Token::Token() {
    token_count++;
    type = TokenType::TOKEN_END;
    ch = 0;
}

char Token::get_char() {
    return ch;
}

void Token::set_char(char ch) {
    this->ch = ch;
}

Token::tokenType Token::get_type() {
    return type;
}

void Token::set_type(Token::tokenType type) {
    this->type = type;
}

bool Token::operator==(Token &t) {
    return this->type == t.type && this->ch == t.ch;
}

bool Token::operator!=(Token &t) {
    return this->type != t.type || this->ch != t.ch;
}

void Token::set_pos(int p) {
    pos = p;
}

int Token::get_pos() {
    return pos;
}
