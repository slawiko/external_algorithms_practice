#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, m;
    const int BLOCK = 1;
    unsigned char* buffer_a = (unsigned char*)malloc(BLOCK);
    FILE* input = fopen("input.bin", "rb");
    fread(&n, sizeof(int), 1, input);

    int block_a = n > BLOCK ? BLOCK : n;
    FILE* reversed_a = fopen("reversed_a.bin", "wb");
    for (int i = n - block_a; i >= 0; i-=block_a) {
        fseek(input, sizeof(int) + i, SEEK_SET);
        fread(buffer_a, sizeof(char), block_a, input);
        fwrite(&buffer_a, sizeof(char), block_a, reversed_a);
        block_a = i > BLOCK ? BLOCK : i;
    }

    unsigned char* buffer_b = (unsigned char*)malloc(BLOCK);
    fseek(input, sizeof(int) + n, SEEK_SET);
    fread(&m, sizeof(int), 1, input);

    int block_b = m > BLOCK ? BLOCK : m;
    FILE* reversed_b = fopen("reversed_b.bin", "wb");
    for (int i = m - block_b; i >= 0; i-=block_b) {
        fseek(input, sizeof(int) + n + sizeof(int) + i, SEEK_SET);
        fread(buffer_b, sizeof(char), block_b, input);
        fwrite(&buffer_b, sizeof(char), block_b, reversed_b);
        block_b = i > BLOCK ? BLOCK : i;
    }

    unsigned char* buffer_c = (unsigned char*)malloc(BLOCK);
    FILE* reversed_c = fopen("reversed_c.bin", "wb");
    int max = n > m ? n : m;
    for (int i = 0; i < max % BLOCK == 0 ? max / BLOCK : max / BLOCK + 1; ++i) {
        fread(buffer_a, sizeof(char), 1, reversed_a);
        fread(buffer_b, sizeof(char), 1, reversed_b);

    }


    return 0;
}