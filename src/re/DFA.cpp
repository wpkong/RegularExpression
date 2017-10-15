//
// Created by kwp on 2017/10/13.
//

#include "DFA.h"

DFA::DFA():node_set_list() {

}

bool DFA::is_set_in_list(node_set &s, int &pos) {
    for(auto &it: node_set_list){
        if(s == it){
            return true;
        }
        pos++;
    }
    return false;
}

bool DFA::has_end_token(node_set &s) {
    for(auto item: s){
        if(item->get_type() == TokenType::TOKEN_END){
            return true;
        }
    }
    return false;
}

void DFA::create(SyntaxTree &tree) {
    /**
        * node_set_list -> [ <a1, a2, b1, c3>, <a1, a2> ...]
        * s -> [a1, a2, b1, c3]
        */
    node_set set_node;
    int cur = 1;
    // 将root的first集合添加到队列
    for(auto it: tree.root->first_pos){
        set_node.insert(it);
    }
    node_set_list.push_back(set_node);
    if(has_end_token(set_node)){
        end_item_vec.push_back(cur);
    }

    for(auto it = node_set_list.begin(); it != node_set_list.end();++it, ++cur){
        std::map<char, node_set> temp;  // 将可接受相同字符的集合放到一起
        for(node_ptr node: *it){
            temp[node->token->get_char()].insert(node);
        }

        // 取出相同可接受字符的项目
        for(auto item: temp){
            char ch = item.first;
            node_set &nodes = item.second;
            node_set new_node;

            // 计算所有可接受的FOLLOW集合的并集
            for(auto node: nodes){
                auto &follow_vec = node->follow_pos;
                for(auto follow_item: follow_vec){
                    new_node.insert(follow_item);
                }
            }

            if(new_node.empty()) continue;

            // 判断集合是否在list里面
            int pos = 1;
            if(!is_set_in_list(new_node, pos)){  // 不在
                node_set_list.push_back(new_node);
                if(has_end_token(new_node)){
                    end_item_vec.push_back(pos);
                }
            }
            // 添加到转换表里面
            transition_item transition = std::make_tuple(cur, ch, pos);
            transition_table.push_back(transition);
        }
    }

#ifdef DEBUG
    for(auto &set: node_set_list){
        for(auto &it: set){
            std::cout << it->token->get_char() << it->token->get_type() << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "\ntable: \n";
    for(auto &t: transition_table){
        std::cout << std::get<0>(t) << " "  << std::get<1>(t) << " "  << std::get<2>(t) << std::endl;
    }

    std::cout << "\nend: ";
    for(auto i: end_item_vec){
        std::cout << i << std::endl;
    }
#endif
}

Matcher::Matcher(DFA &dfa): dfa(dfa), current_state(1) {
}

MatchState::match_state Matcher::test(char ch) {
    for(transition_item &transition: dfa.transition_table){
        int from = std::get<0>(transition);
        char process = std::get<1>(transition);
        int to = std::get<2>(transition);
        if(from == current_state && process == ch){
            current_state = to;
            // 目标是否在终结符集合里面
            if(is_state_end(to)){
                return MatchState::STATE_END;
            } else{
                return MatchState::STATE_ACCEPT;
            }
        }
    }
    return MatchState::STATE_REJECT;
}

bool Matcher::is_state_end(int state) {
    for(int i :dfa.end_item_vec){
        if(state == i){
            return true;
        }
    }
    return false;
}

void Matcher::reset() {
    current_state = 1;
}

