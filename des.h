#ifndef DES_H
#define DES_H

#include <cstdint>
using namespace std;

#define ui64 uint64_t
#define ui32 uint32_t
#define ui8  uint8_t

class DES
{
public:
    DES(uint64_t key);
    uint64_t des(uint64_t block, bool mode);    // mode mean encryption or descryption

    uint64_t encrypt(uint64_t block);
    uint64_t decrypt(uint64_t block);

    static uint64_t encrypt(uint64_t block, ui64 key);
    static uint64_t decrypt(uint64_t block, ui64 key);

protected:
    void keygen(uint64_t key);

    uint64_t ip(uint64_t block);
    uint64_t fp(uint64_t block);

    void feistel(uint32_t &L, uint32_t &R, uint32_t F);
    uint32_t f(uint32_t R, uint64_t k);

private:
    uint64_t sub_key[16]; // 48 bits each, 代表每一轮的密钥
};

#endif
