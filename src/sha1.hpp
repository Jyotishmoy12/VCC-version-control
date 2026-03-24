#ifndef SHA1_HPP
#define SHA1_HPP

#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <cstdint>

class SHA1
{
public:
    SHA1() { reset(); }

    void update(const std::string &s)
    {
        std::istringstream is(s);
        update(is);
    }

    void update(std::istream &is)
    {
        char s;
        while (is.get(s))
        {
            uint32_t char_val = static_cast<uint8_t>(s);
            buffer[buffer_ptr++] = char_val;
            if (buffer_ptr == 64)
            {
                process_block();
                buffer_ptr = 0;
            }
            count += 8;
        }
    }

    std::string final()
    {
        uint64_t total_count = count;
        update_byte(0x80);
        while (buffer_ptr != 56)
            update_byte(0x00);

        for (int i = 7; i >= 0; --i)
        {
            update_byte((total_count >> (i * 8)) & 0xFF);
        }

        std::stringstream ss;
        for (int i = 0; i < 5; ++i)
        {
            ss << std::hex << std::setw(8) << std::setfill('0') << state[i];
        }
        reset();
        return ss.str();
    }

private:
    uint32_t state[5];
    uint8_t buffer[64];
    uint32_t buffer_ptr;
    uint64_t count;

    void reset()
    {
        state[0] = 0x67452301;
        state[1] = 0xEFCDAB89;
        state[2] = 0x98BADCFE;
        state[3] = 0x10325476;
        state[4] = 0xC3D2E1F0;
        buffer_ptr = 0;
        count = 0;
    }

    void update_byte(uint8_t b)
    {
        buffer[buffer_ptr++] = b;
        if (buffer_ptr == 64)
        {
            process_block();
            buffer_ptr = 0;
        }
    }

    void process_block()
    {
        uint32_t w[80];
        for (int i = 0; i < 16; ++i)
        {
            w[i] = (buffer[i * 4] << 24) | (buffer[i * 4 + 1] << 16) | (buffer[i * 4 + 2] << 8) | buffer[i * 4 + 3];
        }
        for (int i = 16; i < 80; ++i)
        {
            w[i] = left_rotate(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
        }
        uint32_t a = state[0], b = state[1], c = state[2], d = state[3], e = state[4];
        for (int i = 0; i < 80; ++i)
        {
            uint32_t f, k;
            if (i < 20)
            {
                f = (b & c) | (~b & d);
                k = 0x5A827999;
            }
            else if (i < 40)
            {
                f = b ^ c ^ d;
                k = 0x6ED9EBA1;
            }
            else if (i < 60)
            {
                f = (b & c) | (b & d) | (c & d);
                k = 0x8F1BBCDC;
            }
            else
            {
                f = b ^ c ^ d;
                k = 0xCA62C1D6;
            }
            uint32_t temp = left_rotate(a, 5) + f + e + k + w[i];
            e = d;
            d = c;
            c = left_rotate(b, 30);
            b = a;
            a = temp;
        }
        state[0] += a;
        state[1] += b;
        state[2] += c;
        state[3] += d;
        state[4] += e;
    }

    uint32_t left_rotate(uint32_t x, uint32_t n) { return (x << n) | (x >> (32 - n)); }
};

#endif