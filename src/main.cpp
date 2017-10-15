#include <iostream>
#include <fstream>
#include <sstream>
#include "re.h"

int main(int argc, char **argv) {
//    if (argc == 2) {
//        std::fstream fs(argv[1]);
//        std::string line;
//        while (std::getline(fs, line)) {
//            RegularExpression re = line;
//            std::getline(fs, line);
//            int num = atoi(line.c_str());
//            for (int i = 0; i < num; ++i) {
//                std::getline(fs, line);
//                bool m = re.match(line);
//                std::getline(fs, line);
//                if (m == (line == "1")) {
//                    std::cout << "匹配成功" << std::endl;
//                } else {
//                    std::cerr << "匹配失败" << std::endl;
//                }
//            }
//            std::getline(fs, line);
//        }
//    } else if (argc == 3) {
//        RegularExpression re = argv[1];
//        std::fstream fs(argv[2]);
//        std::stringstream ss;
//        ss << fs.rdbuf();
//        std::vector<std::string> vec = re.find_all(ss.str());
//        for (auto i: vec) {
//            std::cout << i << std::endl;
//        }
//    } else {
//        std::cerr << "参数不正确" << std::endl;
//    }
    RegularExpression re = "(http|https)://\\w+\\.qq\\.(com|cn)";
    std::fstream fs("/Users/kwp/Projects/compiler/Task2_Cpp/text.txt");
    std::stringstream ss;
    ss << fs.rdbuf();
    std::vector<std::string>&& vec = re.find_all(ss.str());
    for (auto i: vec) {
        std::cout << i << std::endl;
    }
}