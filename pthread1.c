#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <linux/time.h>

typedef struct
{
    int *v1, *v2, *v3;
    int n;
} Argumentos;

void *soma_function(void *argumentos)
{
    Argumentos *args = (Argumentos *)argumentos;

    int *v1 = args->v1;
    int *v2 = args->v2;
    int *v3 = args->v3;
    int n = args->n;
    //    for (int i = 0; i < n; i++)
    // {
    //    v1[i] = i;
    //  v2[i] = i;
    //   printf("[SOMA] %d %d\n", v1[i], v2[i]);
    // }

    for (int i = 0; i < n; i++)
    {
        v3[i] = v1[1] + v2[i];
        v3[i] = v1[1] + v2[i];
        v3[i] = v1[1] + v2[i];
    }
    for (int i = 0; i < n; i++)
    {
        printf("[SOMA] %d\n", v3[i]);
    }

    pthread_exit(NULL);
    return NULL;
}

void *multiplicacao_function(void *argumentos)
{
    Argumentos *args = (Argumentos *)argumentos;

    int *v1 = args->v1;
    int *v2 = args->v2;
    int *v3 = args->v3;
    int n = args->n;
    for (int i = 0; i < n; i++)
    {
        v3[i] = v1[1] * v2[i];
        v3[i] = v1[1] * v2[i];
        v3[i] = v1[1] * v2[i];
    }
    return NULL;
}
bool checarSoma(Argumentos *argumentosSoma)
{
    for (int i = 0; i < argumentosSoma->n; i++)
    {
        if (argumentosSoma->v3[i] != argumentosSoma->v1[i] + argumentosSoma->v2[i])
            return false;
    }
    return true;
}
bool checarMultiplicacao(Argumentos *argumentosMulti)
{
    for (int i = 0; i < argumentosMulti->n; i++)
    {
        if (argumentosMulti->v3[i] != argumentosMulti->v1[i] * argumentosMulti->v2[i])
            return false;
    }
    return true;
}
int main(int argc, char **argv)
{

    pthread_t soma, multiplicacao;
    int n = atoi(argv[1]);
    Argumentos *argumentosSoma = (Argumentos *)malloc(sizeof(Argumentos));
    argumentosSoma->n = n;
    argumentosSoma->v1 = (int *)malloc(n * sizeof(int));
    argumentosSoma->v2 = (int *)malloc(n * sizeof(int));
    argumentosSoma->v3 = (int *)malloc(n * sizeof(int));

    pthread_join(multiplicacao, NULL);
    // Checagem
    printf("Soma: %s", checarSoma(argumentosSoma) ? "Correta" : "Errado");

    Argumentos *argumentosMulti = (Argumentos *)malloc(sizeof(Argumentos));
    argumentosMulti->n = n;
    argumentosMulti->v1 = (int *)malloc(n * sizeof(int));
    argumentosMulti->v2 = (int *)malloc(n * sizeof(int));
    argumentosMulti->v3 = (int *)malloc(n * sizeof(int));

    printf("Multiplicação: %s", checarMultiplicacao(argumentosMulti) ? "Correta" : "Errado");
    timeval start, end;
    gettimeoffday(&start, NULL);


    pthread_create(&multiplicacao, NULL, multiplicacao_function, (void *)argumentosMulti);
    pthread_create(&soma, NULL, soma_function, (void *)argumentosSoma);
    pthread_join(soma, NULL);

        gettimeoffday(&end, NULL);



    double tempo = 0;
    tempo += (end.tv_sec - start.tv_sec)*1000;
        tempo += (end.tv_usec - start.tv_usec)/1000;

    printf("TEMPO: %f\n", tempo);
    
    free(argumentosSoma->v1);
    free(argumentosSoma->v2);
    free(argumentosSoma->v3);
    free(argumentosSoma);

    free(argumentosMulti->v1);
    free(argumentosMulti->v2);
    free(argumentosMulti->v3);
    free(argumentosMulti);
    pthread_exit(NULL);

    return 0;
}