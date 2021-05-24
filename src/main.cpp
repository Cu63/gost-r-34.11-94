#include <iostream>
#include "../header/hash.hpp"

int main() {
    FILE *fRead;
    //fopen("output.txt", "w");

    uint8_t buf[kBufferSize];
    uint8_t hashed[kBlockSize];

    fRead = fopen("input.txt", "r");
    if (fRead == NULL) {
        std::cout << "Can't open file.\n";
        return 1;
    }

    int len;
    int i;
    i = 0;
    while ((len = fread(buf, 1, kBufferSize, fRead)) > 0) {
        hash(buf, len, hashed);
    }

    for (int i = 31; i >= 0; i--)
        std::cout << std::hex << (int)hashed[i];
    std::cout << std::endl;
    std::cout << "Make test1...\n";
//    test1(hashed);
    test2(8, 100);
    return 0;
}
