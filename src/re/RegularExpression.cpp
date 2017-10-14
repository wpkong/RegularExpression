//
// Created by kwp on 2017/10/2.
//

#include "RegularExpression.h"

RegularExpression::RegularExpression(std::string pattern) {
    if (pattern.size() == 0) {
        throw ReError("Illegal regular expression pattern", 0);
    }
    Lexer lexer(pattern);
    SyntaxTree tree(lexer);
    tree.adjust_tree();
    dfa.create(tree);
}

RegularExpression::RegularExpression(const char *pattern) : RegularExpression(std::string(pattern)) {
}

bool RegularExpression::match(std::string str) {
    if (str.size() == 0) {
        throw ReError("Illegal regular expression pattern", 0);
    }

    Matcher matcher(dfa);
    MatchState::match_state state = MatchState::STATE_REJECT;
    for (auto ch: str) {
        state = matcher.test(ch);
        if (state == MatchState::STATE_REJECT) {
            return false;
        }
    }
    if (state == MatchState::STATE_ACCEPT)
        return false;
    return state == MatchState::STATE_END;
}


