#ifndef FILEENCRYPTION_H
#define FILEENCRYPTION_H

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#include "des.h"

class FileEncryption
{
public:
    FileEncryption(ui64 key);
    int encrypt(string input, string output);   // 对外提供接口
    int decrypt(string input, string output);
    

private:
    DES des;
    int cipher (string input, string output, bool mode);    // 加密解密操作
};

#endif
