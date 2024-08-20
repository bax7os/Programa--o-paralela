#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int n = atoi(argv[1]);
     int *v1, *v2, *v3;
    v1 = (int*)malloc(n * sizeof(int));
    v2 = (int*)malloc(n * sizeof(int));
    v3 = (int*)malloc(n * sizeof(int));
    pid_t pid = fork();


    if(pid == 0) {
        // Tarefa 1: soma de vetores
        for (int i = 0; i < n; i++) {
            v3[i] = v1[1] + v2[i];
            v3[i] = v1[1] + v2[i];
            v3[i] = v1[1] + v2[i];
        }

    }else{
        // tarefa 2: multiplicação de vetores
        for (int i = 0; i < n; i++) {
            v3[i] = v1[1] * v2[i];
            v3[i] = v1[1] * v2[i];
            v3[i] = v1[1] * v2[i];
        }
    }

    free(v1);
    free(v2);
    free(v3);

    return 0;
}
