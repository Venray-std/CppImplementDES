#include "fileencryption.h"

FileEncryption::FileEncryption(uint64_t key) :
    des(key)
{
}

int FileEncryption::encrypt(string input, string output)
{
    return cipher(input, output, false);
}

int FileEncryption::decrypt(string input, string output)
{
    return cipher(input, output, true);
}

int FileEncryption::cipher(string input, string output, bool mode)
{
    ifstream ifile;
    ofstream ofile;
    uint64_t buffer;

    if(input.length()  < 1) input  = "/dev/stdin";
    if(output.length() < 1) output = "/dev/stdout";

    ifile.open(input,  ios::binary | ios::in | ios::ate);   // 打开文件，以二进制，读方式
    ofile.open(output, ios::binary | ios::out);     // 可以写入文件

    uint size = ifile.tellg();  // 返回读写位置
    ifile.seekg(0, ios::beg);

    uint block = size / 8;
    if(mode) block--;

    for(uint64_t i = 0; i < block; i++)
    {
        ifile.read((char*) &buffer, 8);

        if(mode)
            buffer = des.decrypt(buffer);
        else
            buffer = des.encrypt(buffer);

        ofile.write((char*) &buffer, 8);
    }

    if(mode == false)
    // 加密操作
    {
        // Amount of padding needed
        uint8_t padding = 8 - (size % 8);

        // Padding cannot be 0 (pad full block)
        if (padding == 0)
            padding  = 8;

        // Read remaining part of file
        buffer = (uint64_t) 0;
        if(padding != 8)
            ifile.read((char*) &buffer, 8 - padding);

        // Pad block with a 1 followed by 0s
        uint8_t shift = padding * 8;
        buffer <<= shift;
        buffer  |= (uint64_t) 0x0000000000000001 << (shift - 1);

        buffer = des.encrypt(buffer);
        ofile.write((char*) &buffer, 8);
    }
    else
    {
        // Read last line of file
        ifile.read((char*) &buffer, 8);
        buffer = des.decrypt(buffer);

        // Amount of padding on file
        uint8_t padding = 0;

        // Check for and record padding on end
        while(!(buffer & 0x00000000000000ff))
        {
            buffer >>= 8;
            padding++;
        }

        buffer >>= 8;
        padding++;

        if(padding != 8)
            ofile.write((char*) &buffer, 8 - padding);
    }

    ifile.close();
    ofile.close();
    return 0;
}
