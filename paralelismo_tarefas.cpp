#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int n = atoi(argv[1]);

    // Tarefa 1: soma de vetores
    int *v1, *v2, *v3;
    v1 = (int*)malloc(n * sizeof(int));
    v2 = (int*)malloc(n * sizeof(int));
    v3 = (int*)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        v3[i] = v1[1] + v2[i];
        v3[i] = v1[1] + v2[i];
        v3[i] = v1[1] + v2[i];
    }

    free(v1);
    free(v2);
    free(v3);

    // tarefa 2: multiplicação de vetores
    int *v4, *v5, *v6;
    v4 = (int*)malloc(n * sizeof(int));
    v5 = (int*)malloc(n * sizeof(int));
    v6 = (int*)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        v6[i] = v4[1] * v5[i];
        v6[i] = v4[1] * v5[i];
        v6[i] = v4[1] * v5[i];
    }

    free(v4);
    free(v5);
    free(v6);

    return 0;
}
