#include "des.h"
#include "des_key.h"
#include "des_data.h"


DES::DES(uint64_t key)
{
    keygen(key);
}

ui64 DES::encrypt(uint64_t block)
{
    return des(block, false);
}

ui64 DES::decrypt(uint64_t block)
{
    return des(block, true);
}

ui64 DES::encrypt(uint64_t block, uint64_t key)
{
    DES des(key);
    return des.des(block, false);
}

ui64 DES::decrypt(uint64_t block, uint64_t key)
{
    DES des(key);
    return des.des(block, true);
}

void DES::keygen(uint64_t key)
{
    // initial key schedule calculation
    uint64_t permuted_choice_1 = 0; // 56 bits
    for (uint8_t i = 0; i < 56; i++)
    {
        permuted_choice_1 <<= 1;
        permuted_choice_1 |= (key >> (64-PC1[i])) & LB64_MASK;
    }

    // 左右28 bits
    ui32 C = (uint32_t) ((permuted_choice_1 >> 28) & 0x000000000fffffff);
    ui32 D = (uint32_t)  (permuted_choice_1 & 0x000000000fffffff);

    // Calculation of the 16 keys, 左右C,D移位一次，置换一次，就得到一轮密钥
    for (uint8_t i = 0; i < 16; i++)
    {
        // key schedule, shifting Ci and Di C_i D_i按照移位表进行移位
        for (uint8_t j = 0; j < ITERATION_SHIFT[i]; j++)
        {
            C = (0x0fffffff & (C << 1)) | (0x00000001 & (C >> 27));
            D = (0x0fffffff & (D << 1)) | (0x00000001 & (D >> 27));
        }
        // 左右合并为56bit
        ui64 permuted_choice_2 = (((uint64_t) C) << 28) | (uint64_t) D;

        sub_key[i] = 0; // 48 bits (2*24)
        // 进行第二次置换，得到48bit
        for (uint8_t j = 0; j < 48; j++)
        {
            sub_key[i] <<= 1;
            sub_key[i] |= (permuted_choice_2 >> (56-PC2[j])) & LB64_MASK;
        }
    } 
}

ui64 DES::des(uint64_t block, bool mode)
{
    // applying initial permutation 初始置换
    block = ip(block);

    // dividing 64bit into two 32-bit parts 64位切分成两个32bit
    ui32 L = (uint32_t) (block >> 32) & L64_MASK;
    ui32 R = (uint32_t) (block & L64_MASK);

    // 16 rounds, f()是每轮操作
    for (ui8 i = 0; i < 16; i++)
    {
        ui32 F = mode ? f(R, sub_key[15-i]) : f(R, sub_key[i]); // 每轮的操作
        feistel(L, R, F);   // 每轮之后的异或
    }

    // swapping the two parts 交换左右两部分
    block = (((ui64) R) << 32) | (ui64) L;
    // applying final permutation 最后的逆置换
    return fp(block);
}

ui64 DES::ip(uint64_t block)
{
    // initial permutation 初始置换， 使用初始置换表IP
    ui64 result = 0;
    for (ui8 i = 0; i < 64; i++)
    {
        result <<= 1;
        result |= (block >> (64-IP[i])) & LB64_MASK;
    }
    return result;
}

ui64 DES::fp(uint64_t block)
{
    // inverse initial permutation 逆初始置换，使用表FP
    ui64 result = 0;
    for (ui8 i = 0; i < 64; i++)
    {
        result <<= 1;
        result |= (block >> (64-FP[i])) & LB64_MASK;
    }
    return result;
}

void DES::feistel(uint32_t &L, uint32_t &R, uint32_t F)
{
    // 异或操作
    ui32 temp = R;
    R = L ^ F;
    L = temp;
}

ui32 DES::f(uint32_t R, uint64_t k) // f(R,k) function
{
    // expansion permutation and returning 48-bit data 扩展置换，32bit变为48bit，根据扩展置换表
    uint64_t s_input = 0;
    for (uint8_t i = 0; i < 48; i++)
    {
        s_input <<= 1;
        s_input |= (uint64_t) ((R >> (32-EXPANSION[i])) & LB32_MASK);
    }

    // XORing expanded Ri with Ki, the round key
    s_input = s_input ^ k;

    // applying S-Boxes function and returning 32-bit data S-box置换， 48bit分成8组对应一个s-box，
    uint32_t s_output = 0;
    for (uint8_t i = 0; i < 8; i++)
    {
        // Outer bits 行索引
        char row = (char) ((s_input & (0x0000840000000000 >> 6*i)) >> (42-6*i));
        row = (row >> 4) | (row & 0x01);

        // Middle 4 bits of input 列索引
        char column = (char) ((s_input & (0x0000780000000000 >> 6*i)) >> (43-6*i));

        s_output <<= 4;
        s_output |= (uint32_t) (SBOX[i][16*row + column] & 0x0f);
    }

    // round permutation 对32bit进行置换
    uint32_t f_result = 0;
    for (ui8 i = 0; i < 32; i++)
    {
        f_result <<= 1;
        f_result |= (s_output >> (32 - PBOX[i])) & LB32_MASK;
    }

    return f_result;
}

//
