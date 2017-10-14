//
// Created by kwp on 2017/10/10.
//

#ifndef TASK2_CPP_TOKEN_H
#define TASK2_CPP_TOKEN_H

#include <memory>

namespace TokenType {
    enum token_type {
        TOKEN_NORMAL,  // 一般字符
        TOKEN_ANY, // .
        TOKEN_SPECIAL, // 预定字符
        TOKEN_UNION,  //  |
        TOKEN_CONCAT,  //  连接
        TOKEN_STAR,  // *
        TOKEN_PLUS,  // +
        TOKEN_QUESTION, //?
        TOKEN_LEFT_BRACKET, // (
        TOKEN_RIGHT_BRACKET,  // )
        TOKEN_LEFT_SQUARE, // [
        TOKEN_RIGHT_SQUARE,  // ]
        TOKEN_RANGE_CONNECT,  // -
        TOKEN_END,
    };
}

static int token_count = 0;
class Token {
    typedef TokenType::token_type tokenType;
public:
    Token();

    ~Token();

    char get_char();

    void set_char(char ch);

    tokenType get_type();

    void set_type(tokenType type);

    void set_pos(int p);

    int get_pos();

    bool operator==(Token &t);

    bool operator!=(Token &t);

private:
    char ch;
    int pos;
    tokenType type;
};

typedef std::shared_ptr<Token> token_ptr;

#endif //TASK2_CPP_TOKEN_H
