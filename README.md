<!--
 * @Author: your name
 * @Date: 2021-02-23 16:38:35
 * @LastEditTime: 2021-02-24 17:54:18
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /CppImplementDES/README.md
-->
CppImplementDES
======
使用C++编写DES加密方法，
参考<https://www.cnblogs.com/luop/p/4366902.html>   

使用方法
1. 首先确保安装了g++, cmake
2. 执行

```
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make
```
3. 使用方法
```
./CppImplementDES -e/-d key [input-file] [output-file]

例如
./CppImplementDES -d 1234 ../data/2 ../data/3
```