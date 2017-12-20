#include <stdio.h>
#include <stdlib.h>

void make_set(int* parent, int v) {
    parent[v] = v;
}

int find_set(int* parent, int v) {
    if (v == parent[v])
        return v;
    return parent[v] = find_set(parent, parent[v]);
}

void union_sets(int* parent, int a, int b) {
    a = find_set(parent, a);
    b = find_set(parent, b);
    if (a != b) {
        parent[b] = a;
    }
}

int main() {
//    FILE* input = fopen("input.bin", "wb");
//    int in[] = {4, 3, 1, 2, 2, 3, 2, 3};
//    fwrite(&in, sizeof(int), 8, input);
//    fclose(input);

    int n, m;
    FILE* input = fopen("input.bin", "rb");
    fread(&n, sizeof(int), 1, input);
    fread(&m, sizeof(int), 1, input);

    int BLOCK = 2;
    int* edges = (int*)malloc(sizeof(int) * BLOCK);
    int* parent = (int*)malloc(sizeof(int) * n);
    for (int i = 0; i < n; ++i) {
        parent[i] = -1;
    }

    int block = n > BLOCK ? BLOCK : n;
    for (int i = 0; i < ; i+=block) {
        fread(edges, sizeof(int), block, input);
        for (int j = 0; j < block; j+=2) {
            make_set(parent, edges[j]);
            make_set(parent, edges[j+1]);
            union_sets(parent, edges[j], edges[j+1]);
        }
        block = n - i > BLOCK ? BLOCK : n - i;
    }
    return 0;
}