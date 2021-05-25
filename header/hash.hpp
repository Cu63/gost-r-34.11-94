#ifndef HASH_HPP
#define HASH_HPP

#include <cstdint>
const int kBlockSize = 32;
const int kBufferSize = 4096;

typedef uint8_t Block[kBlockSize]; // Block - массив из 32х байтов (256 бит)
typedef uint8_t Block32[4]; // Block32 - массив из 4х байтов (32 бита)

// ГОСТ 28147-89
const uint8_t S[8][16] = { // S-блоки, используемые ЦБ РФ
{ 4, 10,  9,  2, 13,  8,  0, 14,  6, 11,  1, 12,  7, 15,  5,  3},
{14, 11,  4, 12,  6, 13, 15, 10,  2,  3,  8,  1,  0,  7,  5,  9},
{ 5,  8,  1, 13, 10,  3,  4,  2, 14, 15, 12,  7,  6,  0,  9, 11},
{ 7, 13, 10,  1,  0,  8,  9, 15, 14,  4,  6, 12, 11,  2,  5,  3},
{ 6, 12,  7,  1,  5, 15, 13,  8,  4, 10,  9, 14,  0,  3, 11,  2},
{ 4, 11, 10,  0,  7,  2,  1, 13,  3,  6,  8,  5,  9, 12, 15, 14},
{13, 11,  4,  1,  3, 15,  5,  9,  0, 10, 14,  7,  6,  8,  2, 12},
{ 1, 15, 13,  0,  5,  7, 10,  4,  9,  2,  3, 14,  6, 11,  8, 12},
};

const Block C2 = {
        0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0x00,
        0xff, 0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0xff, 0x00, 0x00, 0xff,
        0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0xff
    };

void hash(uint8_t arr[], int len, uint8_t res[]);
void f(uint8_t H[], uint8_t M[], uint8_t newH[]);
void P(uint8_t Y[], uint8_t R[]);
int fi(int arg);
void A(uint8_t Y[], uint8_t R[]);
void E(uint8_t D[], uint8_t K[], uint8_t R[]);
void E_f(uint8_t A[], uint8_t K[], uint8_t R[]);
void psi(uint8_t arr[]);
void psi(uint8_t arr[], int p);

void test1(uint8_t hash[],int n, int steps);
void test2(int n, int steps);
int generateString(uint8_t buf[]);
bool compareHash(const uint8_t a[], const uint8_t b[], int n);
#endif
