#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, m;
    const int BLOCK = 150000;
    unsigned char *buffer_a = (unsigned char*)malloc(BLOCK);
    FILE *input = fopen("input.bin", "rb");
    fread(&n, sizeof(int), 1, input);

    int block_a = n > BLOCK ? BLOCK : n;
    FILE *reversed_a = fopen("reversed_a.bin", "w+b");
    for (int i = n - block_a; i >= 0; i -= block_a) {
        fseek(input, sizeof(int) + i, SEEK_SET);
        fread(buffer_a, sizeof(char), block_a, input);
        for (int j = block_a - 1; j >= 0; --j) {
            fwrite(buffer_a + j, sizeof(char), 1, reversed_a);
        }
        block_a = i >= BLOCK ? BLOCK : n % BLOCK;
    }

    unsigned char *buffer_b = (unsigned char*)malloc(BLOCK);
    fseek(input, sizeof(int) + n, SEEK_SET);
    fread(&m, sizeof(int), 1, input);

    int block_b = m > BLOCK ? BLOCK : m;
    FILE *reversed_b = fopen("reversed_b.bin", "w+b");
    for (int i = m - block_b; i >= 0; i -= block_b) {
        fseek(input, sizeof(int) + n + sizeof(int) + i, SEEK_SET);
        fread(buffer_b, sizeof(char), block_b, input);
        for (int j = block_b - 1; j >= 0; --j) {
            fwrite(buffer_b + j, sizeof(char), 1, reversed_b);
        }
        block_b = i > BLOCK ? BLOCK : m % BLOCK;
    }

    unsigned char *buffer_c = (unsigned char *) malloc(BLOCK);

    int max = n > m ? n : m;
    int rad_a = 0, rad_b = 0;
    int k = 0, sum, dec = 0;
    int current_a, current_b;
    FILE *reversed_c = fopen("reversed_c.bin", "w+b");
    fseek(reversed_a, 0, SEEK_SET);
    fseek(reversed_b, 0, SEEK_SET);
    for (int i = 0; i < (max % BLOCK == 0 ? max / BLOCK : max / BLOCK + 1); ++i) {
        if (n > rad_a) {
            if (n - rad_a >= BLOCK) {
                fseek(reversed_a, i * BLOCK, SEEK_SET);
                fread(buffer_a, sizeof(char), BLOCK, reversed_a);
                rad_a += BLOCK;
                current_a = BLOCK;
            } else {
                fread(buffer_a, sizeof(char), block_a, reversed_a);
                rad_a += block_a;
                current_a = block_a;
            }
        } else {
            current_a = 0;
        }

        if (m > rad_b) {
            if (m - rad_b >= BLOCK) {
                fread(buffer_b, sizeof(char), BLOCK, reversed_b);
                rad_b += BLOCK;
                current_b = BLOCK;
            } else {
                fread(buffer_b, sizeof(char), block_b, reversed_b);
                rad_b += block_b;
                current_b = block_b;
            }
        } else {
            current_b = 0;
        }
        if (current_a == current_b) {
            for (int j = 0; j < current_a; ++j) {
                sum = buffer_a[j] + buffer_b[j] + dec;
                if (sum > 9) {
                    buffer_c[j] = sum - 10;
                    dec = 1;
                } else {
                    buffer_c[j] = sum;
                    dec = 0;
                }
            }
            fwrite(buffer_c, sizeof(char), current_a, reversed_c);
            k += current_a;
        } else if (current_a > current_b) {
            for (int j = 0; j < current_b; ++j) {
                sum = buffer_a[j] + buffer_b[j] + dec;
                if (sum > 9) {
                    buffer_c[j] = sum - 10;
                    dec = 1;
                } else {
                    buffer_c[j] = sum;
                    dec = 0;
                }
            }
            for (int j = current_b; j < current_a; ++j) {
                sum = buffer_a[j] + dec;
                if (sum > 9) {
                    buffer_c[j] = sum - 10;
                    dec = 1;
                } else {
                    buffer_c[j] = sum;
                    dec = 0;
                }
            }
            fwrite(buffer_c, sizeof(char), current_a, reversed_c);
            k += current_a;
        } else {
            for (int j = 0; j < current_a; ++j) {
                sum = buffer_a[j] + buffer_b[j] + dec;
                if (sum > 9) {
                    buffer_c[j] = sum - 10;
                    dec = 1;
                } else {
                    buffer_c[j] = sum;
                    dec = 0;
                }
            }
            for (int j = current_a; j < current_b; ++j) {
                sum = buffer_b[j] + dec;
                if (sum > 9) {
                    buffer_c[j] = sum - 10;
                    dec = 1;
                } else {
                    buffer_c[j] = sum;
                    dec = 0;
                }
            }
            fwrite(buffer_c, sizeof(char), current_b, reversed_c);
            k += current_b;
        }
    }
    if (dec == 1) {
        fwrite(&dec, sizeof(char), 1, reversed_c);
        k++;
    }

    int block_o = k > BLOCK ? BLOCK : k;
    FILE *output = fopen("output.bin", "wb");
    fwrite(&k, sizeof(int), 1, output);
    for (int i = k - block_o; i >= 0; i -= block_o) {
        fseek(reversed_c, i, SEEK_SET);
        fread(buffer_c, sizeof(char), block_o, reversed_c);
        for (int j = block_o - 1; j >= 0; --j) {
            fwrite(buffer_c + j, sizeof(char), 1, output);
        }
        block_o = i > BLOCK ? BLOCK : k % BLOCK;
    }

    fclose(input);
    fclose(reversed_a);
    fclose(reversed_b);
    fclose(reversed_c);
    fclose(output);

    free(buffer_a);
    free(buffer_b);
    free(buffer_c);

    return 0;
}