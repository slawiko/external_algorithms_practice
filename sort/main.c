#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int cmpfunc (const void * a, const void * b) {
    if ( *(long long*)a > *(long long*)b ) {
        return 1;
    } else if ( *(long long*)a < *(long long*)b ) {
        return -1;
    } else {
        return 0;
    }
}

int main() {
    size_t N;
    FILE* input = fopen("input.bin", "rb");
    const int BLOCK = 50000;
    long long* BUFFER = (long long*)malloc(sizeof(long long) * BLOCK);

    fread(&N, sizeof(long long), 1, input);

    size_t FILE_CNT = N / BLOCK;
    if (N % BLOCK > 0) {
        FILE_CNT += 1;
    }
    FILE** outputs = (FILE**)malloc(FILE_CNT * sizeof(FILE*));
    char name[21];
    size_t real_block;

    for (size_t i = 0; i < FILE_CNT; i++) {
        real_block = fread(BUFFER, sizeof(long long), BLOCK, input);
        qsort(BUFFER, real_block, sizeof(long long), cmpfunc);

        sprintf(name, "%zu.bin", i);
        outputs[i] = fopen(name, "w+b");
        fwrite(BUFFER, sizeof(long long), real_block, outputs[i]);
    }

    const size_t BLOCK_SIZE = BLOCK / FILE_CNT;

    size_t* indices = (size_t*)malloc(FILE_CNT * sizeof(size_t));
    size_t tmp;

    for (size_t i = 0; i < FILE_CNT; i++) {
        fseek(outputs[i], 0, SEEK_SET);
        tmp = fread(BUFFER + (i * BLOCK_SIZE), sizeof(long long), BLOCK_SIZE, outputs[i]);
        if (tmp < BLOCK_SIZE) {
            BUFFER[(i * BLOCK_SIZE) + tmp] = -1;
        }
        indices[i] = 0;
    }

    long long min;
    size_t min_i;
    bool found;

    FILE* output = fopen("output.bin", "wb");
    fwrite(&N, 8, 1, output);

    while(true) {
        found = false;
        for (size_t i = 0; i < FILE_CNT; i++) {
            if (indices[i] != -1) {
                min = BUFFER[indices[i] + i * BLOCK_SIZE];
                min_i = i;
                found = true;
                break;
            }
        }
        if (!found) {
            break;
        }

        for (size_t i = 0; i < FILE_CNT; i++) {
            if (indices[i] != -1 && BUFFER[indices[i] + i * BLOCK_SIZE] < min) {
                min = BUFFER[indices[i] + i * BLOCK_SIZE];
                min_i = i;
            }
        }

        fwrite(&min, sizeof(long long), 1, output);
        indices[min_i]++;
        if (indices[min_i] == BLOCK_SIZE || BUFFER[indices[min_i] + min_i * BLOCK_SIZE] == -1) {
            tmp = fread(BUFFER + (min_i * BLOCK_SIZE), sizeof(long long), BLOCK_SIZE, outputs[min_i]);
            if (tmp == 0) {
                indices[min_i] = -1;
            } else if (tmp < BLOCK_SIZE){
                BUFFER[(min_i * BLOCK_SIZE) + tmp] = -1;
                indices[min_i] = 0;
            } else {
                indices[min_i] = 0;
            }
        }
    }

    free(BUFFER);
    for (int i = 0; i < FILE_CNT; i++) {
        fclose(outputs[i]);
    }
    fclose(output);
    fclose(input);
    return 0;
}