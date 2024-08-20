typedef struct noh Noh;

struct noh {
    int rotulo;
    int custo;
    Noh *prox;
};

typedef struct grafo *Grafo;

struct grafo {
    Noh **A;
    int n; //numero de vertices
    int m; //numero de arestas

};
typedef struct caminho {
    int vertice;
    struct caminho* proximo;
} Caminho;

Grafo inicializaGrafo(int n);