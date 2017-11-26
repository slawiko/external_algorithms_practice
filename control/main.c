#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main() {
    int N, M;
    FILE* inputA = fopen("input.bin", "rb");
    FILE* inputb = fopen("input.bin", "rb");
    fread(&N, 4, 1, inputA);
    fread(&M, 4, 1, inputA);

    const int matrix_length = N * M;
    const int unused_bytes = sizeof(int) * 2;
    const int BLOCK = 450;
    int block_N = BLOCK, block_M = BLOCK;
    int block_i = BLOCK;
    int block_j = BLOCK;
    bool is_col = false;

    if (M < BLOCK) {
        block_N = BLOCK * BLOCK / M;
        block_M = M;
        is_col = true;
    }

    const int block_size = block_N * block_M;

    unsigned char* A = (unsigned char*)malloc(block_size);
    unsigned char* b = (unsigned char*)malloc(block_M);
    unsigned char* f = (unsigned char*)malloc(block_N);

    FILE* output = fopen("output.bin", "wb");
    fseek(inputb, unused_bytes + matrix_length, SEEK_SET);

    for (int i = 0; i < N; i += block_N) {
        block_i = block_N < (N - i) ? block_N : (N - i);

        for (int j = 0; j < M; j += block_M) {
            block_j = block_M < (M - j) ? block_M : (M - j);

            if (!is_col) {
                fseek(inputb, unused_bytes + matrix_length + j, SEEK_SET);
            }
            fread(b, 1, block_j, inputb);

            for (int l = 0; l < block_i; ++l) {
                if (!is_col) {
                    fseek(inputA, M * i + j + M * l + unused_bytes, SEEK_SET);
                }
                fread(A + block_M * l, 1, block_j, inputA);
            }

            for (int ib = 0; ib < block_N && i + ib < N; ++ib) {
                if (j == 0) {
                    f[ib] = 0;
                }
                for (int jb = 0; jb < block_M && j + jb < M; ++jb) {
                    f[ib] += A[ib * block_M + jb] * b[jb];
                }
            }
        }

        fwrite(f, 1, block_i, output);
    }

    free(A);
    free(b);
    free(f);
    fclose(inputb);
    fclose(inputA);
    fclose(output);

    return 0;
}