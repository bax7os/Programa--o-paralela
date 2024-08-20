#include <pthread.h>

#define n 10000000000

void* soma_function(void* args){
    int v1[n], v2[n], v3[n];
    for (int i = 0; i < n; i++) {
            v3[i] = v1[1] + v2[i];
            v3[i] = v1[1] + v2[i];
            v3[i] = v1[1] + v2[i];
        }

}

void* multiplicacao_function(void* args){
        int v1[n], v2[n], v3[n];
    for (int i = 0; i < n; i++) {
            v3[i] = v1[1] * v2[i];
            v3[i] = v1[1] * v2[i];
            v3[i] = v1[1] * v2[i];
        }
}

int main (int argc, char** argv){

    pthread_t soma, multiplicacao;

    pthread_create(&soma, NULL, soma_function, NULL);
    pthread_create(&multiplicacao, NULL, multiplicacao_function, NULL);




    return 0;
}