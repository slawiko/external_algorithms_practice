#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Pair {
    int first;
    int second;
};

struct Triple {
    int first;
    int second;
    int third;
};

int first_cmp (const void * a, const void * b) {
    return ( ((struct Pair *)a)->first - ((struct Pair *)b)->first );
}

int second_cmp (const void * a, const void * b) {
    return ( ((struct Pair *)a)->second - ((struct Pair *)b)->second );
}

int first_triple_cmp (const void * a, const void * b) {
    return ( ((struct Triple *)a)->first - ((struct Triple *)b)->first );
}

int main() {
    size_t N;
    FILE* input = fopen("input.bin", "rb");
    const int BLOCK = 51000;
    struct Pair* BUFFER = (struct Pair*)malloc(sizeof(struct Pair) * BLOCK);

    fread(&N, sizeof(int), 1, input);

    size_t FILE_CNT = N / BLOCK;
    if (N % BLOCK > 0) {
        FILE_CNT += 1;
    }
    FILE** outputs = (FILE**)malloc(FILE_CNT * 3 / 2 * sizeof(FILE*));
    char name[21];
    size_t real_block;

    // FIRST

    for (size_t i = 0; i < FILE_CNT; i++) {
        real_block = fread(BUFFER, sizeof(struct Pair), BLOCK, input);
        qsort(BUFFER, real_block, sizeof(struct Pair), first_cmp);

        sprintf(name, "%zuf.bin", i);
        outputs[i] = fopen(name, "w+b");
        setvbuf(outputs[i], NULL, _IONBF, 0);
        fwrite(BUFFER, sizeof(struct Pair), real_block, outputs[i]);
    }

    const size_t BLOCK_SIZE = BLOCK / FILE_CNT;

    size_t* indices = (size_t*)malloc(FILE_CNT * 3 / 2 * sizeof(size_t));
    size_t tmp;

    for (size_t i = 0; i < FILE_CNT; i++) {
        fseek(outputs[i], 0, SEEK_SET);
        tmp = fread(BUFFER + (i * BLOCK_SIZE), sizeof(struct Pair), BLOCK_SIZE, outputs[i]);
        if (tmp < BLOCK_SIZE) {
            BUFFER[i * BLOCK_SIZE + tmp].first = -1;
        }
        indices[i] = 0;
    }

    struct Pair min;
    size_t min_i;
    bool found;

    FILE* output_f = fopen("output_f.bin", "w+b");

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
            if (indices[i] != -1 && BUFFER[indices[i] + i * BLOCK_SIZE].first < min.first) {
                min = BUFFER[indices[i] + i * BLOCK_SIZE];
                min_i = i;
            }
        }

        fwrite(&min, sizeof(struct Pair), 1, output_f);
        indices[min_i]++;
        if (indices[min_i] == BLOCK_SIZE || BUFFER[indices[min_i] + min_i * BLOCK_SIZE].first == -1) {
            tmp = fread(BUFFER + (min_i * BLOCK_SIZE), sizeof(struct Pair), BLOCK_SIZE, outputs[min_i]);
            if (tmp == 0) {
                indices[min_i] = -1;
            } else if (tmp < BLOCK_SIZE){
                BUFFER[(min_i * BLOCK_SIZE) + tmp].first = -1;
                indices[min_i] = 0;
            } else {
                indices[min_i] = 0;
            }
        }
    }

    // SECOND

    fseek(input, sizeof(int), SEEK_SET);
    for (size_t i = 0; i < FILE_CNT; i++) {
        real_block = fread(BUFFER, sizeof(struct Pair), BLOCK, input);
        qsort(BUFFER, real_block, sizeof(struct Pair), second_cmp);

        sprintf(name, "%zus.bin", i);
        outputs[i] = fopen(name, "w+b");
        setvbuf(outputs[i], NULL, _IONBF, 0);
        fwrite(BUFFER, sizeof(struct Pair), real_block, outputs[i]);
    }

    for (size_t i = 0; i < FILE_CNT; i++) {
        fseek(outputs[i], 0, SEEK_SET);
        tmp = fread(BUFFER + (i * BLOCK_SIZE), sizeof(struct Pair), BLOCK_SIZE, outputs[i]);
        if (tmp < BLOCK_SIZE) {
            BUFFER[i * BLOCK_SIZE + tmp].second = -1;
        }
        indices[i] = 0;
    }

    FILE* output_s = fopen("output_s.bin", "w+b");

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
            if (indices[i] != -1 && BUFFER[indices[i] + i * BLOCK_SIZE].second < min.second) {
                min = BUFFER[indices[i] + i * BLOCK_SIZE];
                min_i = i;
            }
        }

        fwrite(&min, sizeof(struct Pair), 1, output_s);
        indices[min_i]++;
        if (indices[min_i] == BLOCK_SIZE || BUFFER[indices[min_i] + min_i * BLOCK_SIZE].second == -1) {
            tmp = fread(BUFFER + (min_i * BLOCK_SIZE), sizeof(struct Pair), BLOCK_SIZE, outputs[min_i]);
            if (tmp == 0) {
                indices[min_i] = -1;
            } else if (tmp < BLOCK_SIZE){
                BUFFER[(min_i * BLOCK_SIZE) + tmp].second = -1;
                indices[min_i] = 0;
            } else {
                indices[min_i] = 0;
            }
        }
    }

    // ------------

    fclose(input);

    FILE* output_t = fopen("output_t.bin", "w+b");

    const size_t NEW_BLOCK_SIZE =  BLOCK / 2;

    fseek(output_f, 0, SEEK_SET);
    fread(BUFFER, sizeof(struct Pair), NEW_BLOCK_SIZE, output_f);

    fseek(output_s, 0, SEEK_SET);
    tmp = fread(BUFFER + NEW_BLOCK_SIZE, sizeof(struct Pair), NEW_BLOCK_SIZE, output_s);

    struct Triple t;
    while(tmp > 0) {
        for(size_t i = 0; i < tmp; i++) {
            t.first = BUFFER[NEW_BLOCK_SIZE + i].first;
            t.second = BUFFER[i].first;
            t.third = BUFFER[i].second;
            fwrite(&t, sizeof(int) * 3, 1, output_t);
        }
        fread(BUFFER, sizeof(int) * 2, NEW_BLOCK_SIZE, output_f);
        tmp = fread(BUFFER + NEW_BLOCK_SIZE, sizeof(int) * 2, NEW_BLOCK_SIZE, output_s);
    }

    fclose(output_f);
    fclose(output_s);

    // THIRD

    const int NEW_BLOCK = BLOCK / 3 * 2;
    size_t NEW_FILE_CNT = N / NEW_BLOCK;
    if (N % NEW_BLOCK > 0) {
        NEW_FILE_CNT += 1;
    }
    const size_t NEW_BLOCK_SIZE_2 = NEW_BLOCK / NEW_FILE_CNT;

    struct Triple * T_BUFFER = (struct Triple*) BUFFER;
    struct Triple min_t;

    fseek(output_t, 0, SEEK_SET);
    for (size_t i = 0; i < NEW_FILE_CNT; i++) {
        real_block = fread(T_BUFFER, sizeof(struct Triple), NEW_BLOCK, output_t);
        qsort(T_BUFFER, real_block, sizeof(struct Triple), first_triple_cmp);

        sprintf(name, "%zut.bin", i);
        outputs[i] = fopen(name, "w+b");
        setvbuf(outputs[i], NULL, _IONBF, 0);
        fwrite(T_BUFFER, sizeof(struct Triple), real_block, outputs[i]);
    }

    for (size_t i = 0; i < NEW_FILE_CNT; i++) {
        fseek(outputs[i], 0, SEEK_SET);
        tmp = fread(T_BUFFER + (i * NEW_BLOCK_SIZE_2), sizeof(struct Triple), NEW_BLOCK_SIZE_2, outputs[i]);
        if (tmp < NEW_BLOCK_SIZE_2) {
            T_BUFFER[i * NEW_BLOCK_SIZE_2 + tmp].first = -1;
        }
        indices[i] = 0;
    }

    FILE* output = fopen("output.bin", "w+b");

    while(true) {
        found = false;
        for (size_t i = 0; i < NEW_FILE_CNT; i++) {
            if (indices[i] != -1) {
                min_t = T_BUFFER[indices[i] + i * NEW_BLOCK_SIZE_2];
                min_i = i;
                found = true;
                break;
            }
        }
        if (!found) {
            break;
        }

        for (size_t i = 0; i < NEW_FILE_CNT; i++) {
            if (indices[i] != -1 && T_BUFFER[indices[i] + i * NEW_BLOCK_SIZE_2].first < min_t.first) {
                min_t = T_BUFFER[indices[i] + i * NEW_BLOCK_SIZE_2];
                min_i = i;
            }
        }

        fwrite(&min_t, sizeof(struct Triple), 1, output);
        indices[min_i]++;
        if (indices[min_i] == NEW_BLOCK_SIZE_2 || T_BUFFER[indices[min_i] + min_i * NEW_BLOCK_SIZE_2].first == -1) {
            tmp = fread(T_BUFFER + (min_i * NEW_BLOCK_SIZE_2), sizeof(struct Triple), NEW_BLOCK_SIZE_2, outputs[min_i]);
            if (tmp == 0) {
                indices[min_i] = -1;
            } else if (tmp < NEW_BLOCK_SIZE_2){
                T_BUFFER[(min_i * NEW_BLOCK_SIZE_2) + tmp].first = -1;
                indices[min_i] = 0;
            } else {
                indices[min_i] = 0;
            }
        }
    }

    // -------
    free(BUFFER);
    free(indices);
    for (int i = 0; i < FILE_CNT; i++) {
        fclose(outputs[i]);
    }
    free(outputs);
    fclose(output_t);
    fclose(output);
    return 0;
}