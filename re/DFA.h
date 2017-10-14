//
// Created by kwp on 2017/10/13.
//

#ifndef TASK2_CPP_DFA_H
#define TASK2_CPP_DFA_H

#include "SyntaxTree.h"
#include <tuple>
#include <list>
#include <set>
#include <map>
#include <functional>

class Matcher;

typedef SyntaxTreeNode *node_ptr;
typedef std::set<node_ptr> node_set;  // 转换表
typedef std::tuple<int, char, int> transition_item;
//typedef std::tuple<int, std::function<bool(char)>, int> transition_item;

class DFA {
    friend class Matcher;
public:
    DFA();

    void create(SyntaxTree &tree);

private:
    // 用vector的话不能在循环体内添加内容
    std::list<node_set> node_set_list;
    std::list<transition_item> transition_table;
    std::vector<int> end_item_vec;  // 具有终结符的序号集合

    bool is_set_in_list(node_set &s, int &pos);  // 集合是否在node_set_list里面
    static bool has_end_token(node_set &s);  // 集合是否有end终结符
};

namespace MatchState{
    enum match_state{
        STATE_ACCEPT,  // 接受状态
        STATE_REJECT,  // 拒绝状态
        STATE_END      // 结束状态
    };
}

class Matcher {
public:
    Matcher(DFA &dfa);
    MatchState::match_state test(char ch);

private:
    bool is_state_end(int state);

private:
    DFA &dfa;
    int current_state;  // 当前dfa状态
};


#endif //TASK2_CPP_DFA_H
