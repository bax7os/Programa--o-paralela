#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define n 1000000

void* soma_function(void* args){
    int *v1 = (int*)malloc(n * sizeof(int)),
    *v2 = (int*)malloc(n * sizeof(int)),
    *v3 = (int*)malloc(n * sizeof(int));
    
    

//    for (int i = 0; i < n; i++)
  // {
    //    v1[i] = i;
      //  v2[i] = i;
    //   printf("[SOMA] %d %d\n", v1[i], v2[i]);
   // }
    
   for (int i = 0; i < n; i++) {
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

    pthread_exit(NULL);


    return 0;
}