#include <pthread.h>
#include <cstdlib>
#include <cstdio>
#include <math.h>

typedef struct
{
    int ini;
    int fim;
    int **A;
    int n;
} Argumentos;

pthread_barrier_t barreira;
pthread_barrier_t barreira_fim;

int **A;

void *corrida_de_numeros(void *argumentos)
{
    Argumentos argumento = *((Argumentos *)argumentos);
    printf("Thread ini: %d, fim: %d\n", argumento.ini, argumento.fim);

    for (int i = 0; i < argumento.n; i++)
    {
        for (int j = argumento.ini; j < argumento.fim; j++)
        {
            if (i == 0)
            {
                argumento.A[i][j] = rand();
            }
            else{
                int coluna_sorteada = rand() % argumento.n;
                argumento.A[i][j] = argumento.A[i-1][coluna_sorteada];
            }
        }
        pthread_barrier_wait(&barreira);

    }
    






        pthread_barrier_wait(&barreira);

    pthread_exit(NULL);
    return NULL;
}

int main(int argc, char **argv)
{
    int n = atoi(argv[1]);
    int t = atoi(argv[2]);


    pthread_barrier_init(&barreira_fim, NULL, t+1);
     pthread_barrier_init(&barreira, NULL, t);
    A = (int **)calloc(n, sizeof(int *));

    for (int i = 0; i < n; i++)
    {
        A[i] = (int *)calloc(n, sizeof(int));
    }
    int ct = ceil((float)n / t);

    pthread_t *threads = (pthread_t *)malloc(t * sizeof(pthread_t));
        Argumentos *argumento = (Argumentos*)malloc(t* sizeof(Argumentos));

    for (int tid = 0; tid < t; tid++)
    {

        argumento[tid].A = A;
        argumento[tid].ini = tid * ct;
        argumento[tid].fim = (tid == t - 1) ? n : (tid + 1) * ct;
        argumento[tid].n = n;
        printf("Thread %d, ini: %d, fim: %d\n", tid, argumento[tid].ini, argumento[tid].fim);
        pthread_create(&threads[tid], NULL, corrida_de_numeros, (void*)&(argumento[tid]));
    }

    pthread_barrier_wait(&barreira_fim);
    free(argumento);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%d", A[i][j]);

        }
    }
    


    pthread_exit(NULL);

    return 0;
}