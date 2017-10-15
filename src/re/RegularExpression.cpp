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

std::vector<std::string> RegularExpression::find_all(std::string str) {
    if (str.size() == 0) {
        throw ReError("Illegal regular expression pattern", 0);
    }

    std::vector<std::string> results;
    std::stringstream ss;
    Matcher matcher(dfa);
    MatchState::match_state last_state = MatchState::STATE_ACCEPT;
    MatchState::match_state state = MatchState::STATE_REJECT;
    auto it = str.begin();
    while (it < str.end()) {
        state = matcher.test(*it);
        if (state == MatchState::STATE_REJECT) {
            if (last_state == MatchState::STATE_END) {
                results.push_back(ss.str());
                ss.str("");  // 清空
            } else {
                matcher.reset();
                ss.str("");
                ++it;
            }
        } else {
            ss << *it;
            ++it;
        }
        last_state = state;
    }
    if (last_state == MatchState::STATE_END) {
        results.push_back(ss.str());
        ss.str("");  // 清空
    }
    return results;
}


