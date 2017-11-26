#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main() {
    int N;
    int M;
    FILE* input = fopen("input.bin", "rb");
    fread(&N, 4, 1, input);
    fread(&M, 4, 1, input);
    const int unused_bytes = 8;
    const int BLOCK = 750;
    int block_N, block_M;
    bool is_transposed_col = false, is_original_col = false;

    if (N < BLOCK && M < BLOCK) {
        block_N = N;
        block_M = M;
        is_transposed_col = is_original_col = true;
    } else if (N < BLOCK) {
        block_N = N;
        block_M = BLOCK * BLOCK / N;
        is_transposed_col = true;
        if (block_M > M) {
            is_original_col = true;
        }
    } else if (M < BLOCK) {
        block_N = BLOCK * BLOCK / M;
        block_M = M;
        is_original_col = true;
        if (block_N > N) {
            is_transposed_col = true;
        }
    } else {
        block_N = BLOCK;
        block_M = BLOCK;
    }

    const int block_size = block_N * block_M;
    unsigned char* a = (unsigned char*)malloc(block_size);

    FILE* output = fopen("output.bin", "wb");
    fwrite(&M, 4, 1, output);
    fwrite(&N, 4, 1, output);

    for (int i = 0; i < N; i += block_N) {
        block_N = block_N < (N - i) ? block_N : (N - i);
        for (int j = 0; j < M; j += block_M) {
            block_M = block_M < (M - j) ? block_M : (M - j);
            if (!is_original_col) {
                fseek(input, M * i + j + unused_bytes, SEEK_SET);
            }
            for (int l = 0; l < block_N; ++l) {
                fread(a + block_M * l, 1, block_M, input);
                if (!is_original_col) {
                    fseek(input, M - block_M, SEEK_CUR);
                }
            }

            if (!is_transposed_col) {
                fseek(output, N * j + i + unused_bytes, SEEK_SET);
            }
            for (int jb = 0; jb < block_M; jb++) {
                for (int ib = 0; ib < block_N; ib++) {
                    fwrite(a + ib * block_M + jb, 1, 1, output);
                }
                if (!is_transposed_col) {
                    fseek(output, N - block_N, SEEK_CUR);
                }
            }
        }
    }

    free(a);
    fclose(input);
    fclose(output);

    return 0;
}