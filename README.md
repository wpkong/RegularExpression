# 正则表达式引擎
> 引擎参照龙书第三章正则表达式的构建
---

## 说明
* 该引擎支持 ?（可选）、*（零次或多次）、+（一次或多次）；
* ?只具备可选匹配功能；
* 支持范围[]，内部可以是选项[abc]，也可以是范围[a-zA-Z]；
* 支持多层括号分组，例如((ab|12)*|(xy|89)*)+ ；
* 支持通配符. ；
* 支持特殊字符: \d、\D、\w、\W、\s、\S；
* 支持转义字符，例如 ab\\(c 。

## 使用方法
```c 
#include <iostream>
#include "re.h"

int main(){
    // 匹配模式
    RegularExpression re = "((ab|12)*|(xy|89)*)+";
    std::cout << re.match("12xy8912ab") << std::endl;

    // 搜索模式
    RegularExpression re = "a";
    std::vector<std::string>&& vec = re.find_all("aaaaaaa");
    for(auto i: vec){
        std::cout << i << std::endl;
    }
}
```

## 测试程序编译方法
```bash
$ chmod +X install.sh
$ ./install.sh
```
可执行文件放在target目录下，命名为re

## 测试集使用方法
使用编译好的re文件
```bash
# 搜索模式使用方法：
$ ./re "(http|https)://\w+\.qq\.(com|cn)" ../text.txt 
# 第一个参数为匹配模式，第二个为文件存放地址，这里是hao123的网页

# 匹配模式使用方法：
￥ ./re ../samples.txt
# 参数为samples存放地址，samples格式如下
```

## samples格式
```
- 正则模式
- 实例数量
- 实例
- 正确结果
...

例：
\d+
3
222
1
moha
0
1202
1
```


## 目前的问题
按照龙书第三章的算法，需要计算正则表达式模式树的first、last、follow集合。而龙书内仅仅对*进行了示范，并未给出范围字符的实例。因此对于通配符、或者范围符号，需要创建大量的冗余节点，这样会导致资源占用较大。