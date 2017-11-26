#include <stdio.h>
#include <stdlib.h>

int main() {
    int N;
    FILE* input = fopen("input.bin", "rb");
    fread(&N, 4, 1, input);
    const int matrix_length = N * N;
    const int unused_bytes = 8;
    const int BLOCK = 400;
    const int block_size = BLOCK * BLOCK;
    int block_i = BLOCK;
    int block_j = BLOCK;
    int block_k = BLOCK;

    unsigned char* a = (unsigned char*)malloc(block_size);
    unsigned char* b = (unsigned char*)malloc(block_size);
    unsigned char* c = (unsigned char*)malloc(block_size);

    FILE* output = fopen("output.bin", "wb");
    fwrite(&N, 4, 1, output);
    fwrite(&N, 4, 1, output);

    for (int i = 0; i < N; i += BLOCK) {
        block_i = BLOCK < (N - i) ? BLOCK : (N - i);
        for (int j = 0; j < N; j += BLOCK) {
            block_j = BLOCK < (N - j) ? BLOCK : (N - j);
            for (int k = 0; k < N; k += BLOCK) {
                block_k = BLOCK < (N - k) ? BLOCK : (N - k);
                for (int l = 0; l < block_i; ++l) {
                    fseek(input, N * i + k + N * l + unused_bytes, SEEK_SET);
                    fread(a + BLOCK * l, 1, block_k, input);
                }

                for (int l = 0; l < block_k; l++) {
                    fseek(input, N * k + j + N * l + unused_bytes * 2 + matrix_length, SEEK_SET);
                    fread(b + BLOCK * l, 1, block_j, input);
                }

                for (int ib = 0; ib < BLOCK && i + ib < N; ++ib) {
                    for (int jb = 0; jb < BLOCK && j + jb < N; ++jb) {
                        if (k == 0) {
                            c[ib * BLOCK + jb] = 0;
                        }
                        for (int kb = 0; kb < BLOCK && k + kb < N; ++kb) {
                            c[ib * BLOCK + jb] += a[ib * BLOCK + kb] * b[kb * BLOCK + jb];
                        }
                    }
                }
            }

            for (int l = 0; l < block_i; ++l) {
                fseek(output, N * i +  j + N * l + unused_bytes, SEEK_SET);
                fwrite(c + BLOCK * l, 1, block_j, output);
            }
        }
    }

    free(a);
    free(b);
    free(c);
    fclose(input);
    fclose(output);

    return 0;
}