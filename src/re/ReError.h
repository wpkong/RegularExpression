//
// Created by kwp on 2017/10/10.
//

#ifndef TASK2_CPP_ERROR_H
#define TASK2_CPP_ERROR_H

#include <exception>
#include <string>
#include <sstream>

class ReError: public std::exception {
public:
    ReError(): ReError("", 0){}
    ReError(std::string m, int p): msg(m), pos(0){}
    std::string what(){
        std::stringstream ss;
        ss << msg << " at " << pos;
        return ss.str();
    }

private:
    std::string msg;
    int pos;
};


#endif //TASK2_CPP_ERROR_H
