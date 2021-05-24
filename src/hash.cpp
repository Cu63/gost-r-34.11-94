#include <iostream>
#include <memory.h>
#include "../header/hash.hpp"


void hash(uint8_t buf[], int len, uint8_t result[]) {
    uint8_t block[kBlockSize];
    uint8_t Sum[kBlockSize];
    uint8_t L[kBlockSize];
    uint8_t H[kBlockSize];
    uint8_t newH[kBlockSize];
    int pos;

    pos = 0;
    memset(Sum, 0, kBlockSize);
    memset(H, 0, kBlockSize);
    memset(L, 0,  kBlockSize);

    while (pos != len) {
        if (len >= kBlockSize) {
            memcpy(block, &buf[pos], kBlockSize);
            pos += kBlockSize;
        } else {
            memset(block, 0, kBlockSize);
            memcpy(block, &buf[pos], len - pos);
            pos += len - pos;
        }
        int c = 0;
        for (int i = 0; i < kBlockSize; i++) {
            c += block[i] + Sum[i];
            Sum[i] = c & 0xFF;
            c >>= 8;
        }

        f(H, block, newH); 
        memcpy(H, newH, kBlockSize);
    }

    int c = len << 3;
    for (int i = 0; i < kBlockSize; i++) {
        L[i] = c & 0xFF;
        c >>= 8;
    }

    f(H, L, newH);
    memcpy(H, newH,  kBlockSize);
    f(H, Sum, newH);
    memcpy(result, newH, kBlockSize);
}

void f(uint8_t H[], uint8_t M[], uint8_t newH[]) { // Функция f
    Block C[4];
    Block U;
    Block V;
    Block W;
    Block K[4];
    Block tmp;
    memset(C, 0, sizeof C);
    //Согласно госту нужно заполнить константными значениями.
    memcpy(&C[2], C2, kBlockSize);

    memcpy(U, H, kBlockSize);
    memcpy(V, M, kBlockSize);

    //Генерация ключей
    for (int i = 0; i < kBlockSize; i++)
        W[i] = U[i] ^ V[i];
    P(W, K[0]);
    for (int step = 1; step < 4; step++) {
        A(U, tmp);
        for (int i = 0; i < kBlockSize; i++) 
            U[i] = tmp[i] ^ C[step][i];
        A(V, tmp); 
        A(tmp, V);
        for (int i = 0; i < kBlockSize; i++)
            W[i] = U[i] ^ V[i];
        P(W, K[step]);
    }

    //Шифрование
    Block S;
    for (int i = 0; i < kBlockSize; i += 8)
        E(H + i, K[i >> 3], S + i);

    psi(S, 12);
    for (int i = 0; i < kBlockSize; i++)
        S[i] ^= M[i];
    psi(S, 1);
    for (int i = 0; i < kBlockSize; i++)
        S[i] ^= H[i];
    psi(S, 61);
    memcpy(newH, S, kBlockSize);
}

void P(uint8_t Y[], uint8_t R[]) {
    for (int i = 0; i < kBlockSize; i++)
        R[i] = Y[fi(i)];
}

int fi(int arg) { 
    //запоминаем последние 3 бита
    int i = arg & 0x03;
    //делим число на 4
    int k = arg >> 2; 
    //умножаем i на 8 и складываем с k
    return (i << 3) + k;
}

// GOST R 34.11-94
void A(uint8_t Y[], uint8_t R[]) {
    for (int i = 0; i < 24; i++)
        R[i] = Y[i + 8];
    for (int i = 0; i < 8; i++) 
        R[i + 24] = Y[i] ^ Y[i + 8];
}

//Функция шифрования
void E(uint8_t  D[], uint8_t K[], uint8_t R[]) { // ГОСТ 28147-89
    Block32 A;
    Block32 B;
    for (int i = 0; i < 4; i++)
        A[i] = D[i];
    for (int i = 0; i < 4; i++)
        B[i] = D[i + 4];
// K1..K24 идут в прямом порядке - три цикла K1..K8
    for (int step = 0; step < 3; step++)
        for (int i = 0; i < kBlockSize; i += 4) {
// (K + i) - массив K с i-го элемента
            Block32 tmp;
            E_f(A, K + i, tmp);              
            for (int i = 0; i < 4; i++)
                tmp[i] ^= B[i];
            memcpy(B, A, sizeof A);
            memcpy(A, tmp, sizeof tmp);
    }
    for (int i = 28; i >= 0; i -= 4) { // А K25..K32 идут в обратном порядке
        Block32 tmp; E_f(A, K + i, tmp);
        for (int i = 0; i < 4; i++)
            tmp[i] ^= B[i];
        memcpy(B, A, sizeof A);
        memcpy(A, tmp, sizeof tmp);
    }
    for (int i = 0; i < 4; i++)
        R[i] = B[i];
    for (int i = 0; i < 4; i++)
        R[i + 4] = A[i];
}

// Функция f в ГОСТ 28147-89
void E_f(uint8_t A[], uint8_t K[], uint8_t R[]) {
    int c = 0; //Складываем по модулю 2^32. c - перенос  в следующий разряд
    for (int i = 0; i < 4; i++) {
        c += A[i] + K[i];
        R[i] = c & 0xFF;
        c >>= 8;
      }
//Заменяем 4х-битные кусочки согласно S-блокам
    for (int i = 0; i < 8; i++) {
// x - 4х-битный кусочек
        int x = R[i >> 1] & ((i & 1) ? 0xF0 : 0x0F);
        // Обнуляем соответствующие биты
        R[i >> 1] ^= x;                                
// сдвигаем x либо на 0, либо на 4 бита влево
        x >>= (i & 1) ? 4 : 0;                         
// Заменяем согласно S-блоку 
        x = S[i][x];
        R[i >> 1] |= x << ((i & 1) ? 4 : 0);
  }

    int tmp = R[3]; // Сдвигаем на 8 бит (1 байт) влево
    R[3] = R[2];
    R[2] = R[1];
    R[1] = R[0];
    R[0] = tmp;

    tmp = R[0] >> 5; // Сдвигаем еще на 3 бита влево
    for (int i = 1; i < 4; i++) {
        int nTmp = R[i] >> 5;
        R[i] = (R[i] << 3) | tmp;
        tmp = nTmp;
    }
    R[0] = (R[0] << 3) | tmp;
}

void psi(uint8_t arr[]) {
    uint8_t y16[] = {0, 0};
    y16[0] ^= arr[ 0]; 
    y16[1] ^= arr[ 1];
    y16[0] ^= arr[ 2]; 
    y16[1] ^= arr[ 3];
    y16[0] ^= arr[ 4]; 
    y16[1] ^= arr[ 5];
    y16[0] ^= arr[ 6]; 
    y16[1] ^= arr[ 7];
    y16[0] ^= arr[24]; 
    y16[1] ^= arr[25];
    y16[0] ^= arr[30]; 
    y16[1] ^= arr[31];
    for (int i = 0; i < 30; i++)
        arr[i] = arr[i + 2];
    arr[30] = y16[0]; arr[31] = y16[1];
}

void psi(uint8_t arr[], int p) {
    while (p--)
        psi(arr);
}
