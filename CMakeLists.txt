cmake_minimum_required(VERSION 3.7)
project(Task2_Cpp)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        re/RegularExpression.h re/RegularExpression.cpp
        re/Lexer.h re/Lexer.cpp
        re/Token.h re/Token.cpp
        re/SyntaxTree.cpp re/SyntaxTree.h
        re/SyntaxTreeNode.cpp re/SyntaxTreeNode.h
        re/ReError.h re/DFA.h re/DFA.cpp
        re.h main.cpp)

add_executable(Task2_Cpp ${SOURCE_FILES})
