#include <stdio.h>
#include <stdlib.h>

void make_set(int* parent, int v) {
    if (parent[v-1] == -1) {
        parent[v-1] = v-1;
    }
}

int find_set(int* parent, int v) {
    if (v-1 == parent[v-1])
        return v;
    return (parent[v-1] = find_set(parent, parent[v-1] + 1) - 1) + 1;
}

void union_sets(int* parent, int a, int b) {
    a = find_set(parent, a);
    b = find_set(parent, b);
    if (a != b) {
        parent[b-1] = a-1;
    }
}

int main() {
    int n, m;
    FILE* input = fopen("input.bin", "rb");
    fread(&n, sizeof(int), 1, input);
    fread(&m, sizeof(int), 1, input);

    int BLOCK = 50000;
    int* edges = (int*)malloc(sizeof(int) * BLOCK);
    int* parent = (int*)malloc(sizeof(int) * n);
    for (int i = 0; i < n; ++i) {
        parent[i] = -1;
    }

    int block = m * 2 > BLOCK ? BLOCK : m * 2;
    int read = 0;
    while(block > 0) {
        fread(edges, sizeof(int), block, input);
        read += block;
        for (int j = 0; j < block; j+=2) {
            make_set(parent, edges[j]);
            make_set(parent, edges[j+1]);
            union_sets(parent, edges[j], edges[j+1]);
        }
        block = m * 2 - read > BLOCK ? BLOCK : m * 2 - read;
    }
    fclose(input);

    int result = 0;
    for (int i = 0; i < n; ++i) {
        if (parent[i] == i || parent[i] == -1) {
            result++;
        }
    }

    free(edges);
    free(parent);

    FILE* output = fopen("output.bin", "wb");
    fwrite(&result, sizeof(int), 1, output);
    fclose(output);

    return 0;
}