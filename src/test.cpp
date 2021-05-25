#include <iostream>
#include <ctime>
#include <cstdlib>
#include "../header/hash.hpp"

void test1(uint8_t h[],int n, int steps) {
    uint8_t *buf;
    uint8_t *newHash;
    int len;
    int avgStep;
    int i;
    
    srand(time(0));
    avgStep = 0;
    buf = new uint8_t[kBufferSize];
    newHash = new uint8_t[kBlockSize];

    for (i = 0; i < steps; i++) {
        unsigned int step = 0;
        bool key = true;
        while(key) {
            step++;
            len = generateString(buf);
            memset(newHash, 0, kBlockSize);

            hash(buf, len, newHash);
            if (compareHash(newHash, h, n))
                key = false;
        }
        std::cout << std::dec << avgStep / (i + 1) << std::endl;
        avgStep += step;
    }
    std::cout << std::dec << "Average step: " << avgStep / i << std::endl;
    delete[] buf;
    delete[] newHash;
}

void test2(int n, int steps) {
    uint8_t *buf;
    uint8_t *a;
    uint8_t *b;
    int len;
    int avgStep;
    int i;
    
    srand(time(0));
    avgStep = 0;
    buf = new uint8_t[kBufferSize];
    a = new uint8_t[kBlockSize];
    b = new uint8_t[kBlockSize];

    for (i = 0; i < steps; i++) {
        unsigned int step = 0;
        bool key = true;
        while(key) {
            step++;
            len = generateString(buf);
            memset(a, 0, kBlockSize);
            hash(buf, len, a);

            len = generateString(buf);
            memset(b, 0, kBlockSize);
            hash(buf, len, b);
            if (compareHash(a, b, n))
                key = false;
        }
    //    std::cout << std::dec << avgStep / (i + 1) << std::endl;
        avgStep += step;
    }
    std::cout << std::dec << "Average step: " << avgStep / i << std::endl;
    delete[] buf;
    delete[] a;
    delete[] b;
}

bool compareHash(const uint8_t a[], const uint8_t b[], int n) {
    int i;

    for (i = 0; i < n; i++) {
        if (((a[i / 8]) & (1 << (i % 8))) != ((b[i / 8]) & (1 << (i % 8))))
            break;
    }
    if (i == n)
        return true;
    else
        return false;
}

int generateString(uint8_t *s) {
    int len;

    len = rand() % 31 + 1;;
    for (int i = 0; i < len; ++i)
        s[i] = rand() % 256;
    return len;
}

