#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include <vector>
typedef struct Graph {

  int *edgelistSize = 0;
  int **edgelist = 0;
  int vertices = 0;
  int *vertice;
  int *countersPerVertex = 0;
  int contador_edge;
  void initialize(const char *graphDataset) {
    FILE *fp = fopen("graph.edgelist", "r");
    int src, dst;
    int maxVertexId = -1;
    while (fscanf(fp, "%d %d", &src, &dst) != EOF) {
      if (src > maxVertexId)
        maxVertexId = src;
      if (dst > maxVertexId)
        maxVertexId = dst;
    }
    vertices = maxVertexId + 1;
    rewind(fp);
    edgelistSize = (int *)malloc(vertices * sizeof(int));
    while (fscanf(fp, "%d %d", &src, &dst) != EOF) {
      edgelistSize[src]++;
    }

    // Memory allocation
    edgelist = (int **)malloc(vertices * sizeof(int *));
    for (int i = 0; i < vertices; i++) {
      edgelist[i] = (int *)malloc(edgelistSize[i] * sizeof(int));
    }

    countersPerVertex = (int *)malloc(vertices * sizeof(int));
    memset(countersPerVertex, 0, vertices * sizeof(int));

    rewind(fp);
  }

  int getVertices() { return vertices; }

  int getEdgelistSize(int src) { return edgelistSize[src]; }

  int getEdge(int src, int pos) { return edgelist[src][pos]; }

  void addEdge(int src, int dst) {
    edgelist[src][countersPerVertex[src]++] = dst;
  }

  bool isNeighbour(int src, int dst) {
    for (int i = 0; i < getVertices(); i++) {
      for (int j = 0; j < getEdgelistSize(i); j++) {
        if (src == i and dst == getEdge(i, j)) {

          return true;
        }
      }
    }

    return false;
  }
  void release() {
    for (int i = 0; i < vertices; i++)
      free(edgelist[i]);
    free(edgelist);
    free(countersPerVertex);
    free(edgelistSize);
  }
} Graph;
/*
int contagem_de_cliques_serial(Graph *grafo, int k) {

  int contagem = 0;
  int clique_size_1 = 0;
  int clique_size_2 = 0;
  int *clique_1 = (int*)malloc(grafo->getVertices() * sizeof(int));

  int ultimo_vertice;
  for (int i = 0; i < grafo->getVertices(); i++) {
    if (grafo->getVertices() == 0) {
      clique_1 = NULL;
    }else{
      clique_1[i] = i;
      clique_size_1++;
    }

  }

  while (clique_size_1 > 0) {
    int *clique_2 = (int*)malloc(clique_size_1 * sizeof(int));
    clique_2 = &clique_1[clique_size_1 - 1];
    clique_size_1--;
    clique_size_2++;
    clique_1 = (int*)realloc(clique_1, clique_size_1 * sizeof(int));

   for (int i = 0; i < clique_size_2; i++) {
     printf("clique_2: %d ", clique_2[i]);
    }
printf("\n");
    if (clique_size_2 == k) {

      contagem++;
      printf("contagem: %d", contagem);
      continue;
    }

    ultimo_vertice = clique_2[clique_size_2 -1];
    printf("ultimo vertice clique 2: %d\n", ultimo_vertice);

    for (int i = 0; i < clique_size_2; i++) {
      for (int j = 0; j < grafo->getEdgelistSize(clique_2[i]); j++) {
         int vizinho =  grafo->getEdge(clique_2[i], j);
          printf("vizinho: %d", vizinho);
          if (vizinho > ultimo_vertice) {
            bool is_clique = true;
           printf("\n");
            for (int k = 0; k < clique_size_2; k++) {
              printf("dentro do for: clique_2: %d ", clique_2[k]);
                if (!grafo->isNeighbour(clique_2[k], vizinho) || clique_2[k] ==
vizinho) { is_clique = false; break;
                }
            }
            printf("\n");
            if (is_clique) {
              int *nova_clique = (int*)malloc((clique_size_2 + 1) *
sizeof(int));

              for (int l = 0; l < clique_size_2; l++) {
                 printf("nova_clique: %d ", clique_2[l]);
                  nova_clique[l] = clique_2[l];
              }
printf("\n");
              nova_clique[clique_size_2] = vizinho;
              printf("vizinho_isclique: %d \n", vizinho);
              clique_1 = (int*)realloc(clique_1, clique_size_1 * sizeof(int));
printf("\n");
              for (int n = 0; n < clique_size_1; n++) {
                  clique_1[n] = nova_clique[n];
                   printf("clique_1: %d \n", clique_2[n]);
              }
              printf("\n");
              free(nova_clique);
            }

          }
      }
    }



  }
  free(clique_1);
  return contagem;
}
*/

int contagem_de_cliques_serial(Graph *grafo, int k) {
  int contagem = 0;
  std::vector<std::vector<int>> cliques;
  std::vector<int> nova_clique;
  std::vector<std::vector<int>> cliques_save;

  

  for (int i = 0; i < grafo->getVertices(); i++) {
    cliques.push_back({i});
  }

  while (!cliques.empty()) {
    std::vector<int> clique = cliques.back();

    cliques.pop_back();

    printf("Clique atual: ");
    for (int vertice : clique) {
      printf("%d ", vertice);
    }
    printf("\n");

    if (clique.size() == k) {

      if (std::find(cliques_save.begin(), cliques_save.end(), clique) == (cliques_save.end())){
        contagem++;
        cliques_save.push_back(clique);
    }
      
      continue;
      printf("\nContagem: %d\n", contagem);
    }



    int ultimo_vertice = clique.back();
    printf("Último vértice: %d\n", ultimo_vertice);
    for (int vertice : clique) {
      printf("Vértice: %d\n", vertice);

        // verifica os vizinhos do vertice
        for (int j = 0; j < grafo->getEdgelistSize(vertice); j++) {
          int vizinho = grafo->getEdge(vertice, j);
          printf("vizinho de %d: %d\n", vertice, vizinho);
          // so adiciona vizinhos que sao maiores que o ultimo vértice da clique atual
          if (vizinho > ultimo_vertice &&
              std::find(clique.begin(), clique.end(), vizinho) ==
                  clique.end() && // vizinho nao esta na clique
              std::all_of(clique.begin(), clique.end(), [&](int v) {
                return grafo->isNeighbour(v, vizinho);
              })) {

              nova_clique = clique;
              nova_clique.push_back(vizinho);
              cliques.push_back(nova_clique);
       
            printf("Nova clique formada: ");
            for (int nv : nova_clique) {
              printf("%d ", nv);
            }
            printf("\n");
          }
        }
      }
      printf("Estado atual das cliques:\n");
      for (const auto &c : cliques) {
        printf("[ ");
        for (int v : c) {
          printf("%d ", v);
        }
        printf("]\n");
      }
    }
  
      printf("cliques_save:\n");
      for (const auto &c : cliques_save) {
        printf("[ ");
        for (int v : c) {
          printf("%d ", v);
        }
        printf("]\n");
      }

  return contagem;
}

int main(int argc, char *argv[]) {
  int k = atoi(argv[1]);
  printf("Valor de k: %d\n", k);
  Graph *graph = new Graph;
  graph->initialize("graph.edgelist");
  FILE *fp = fopen("graph.edgelist", "r");
  int src = 0, dst = 0;

  while (fscanf(fp, "%d %d", &src, &dst) != EOF) {
    graph->addEdge(src, dst);
  }

  printf("Cliques: %d", contagem_de_cliques_serial(graph, k));

  fclose(fp);

  for (int i = 0; i < graph->getVertices(); i++) {
    for (int j = 0; j < graph->getEdgelistSize(i); j++) {
      // printf("%d %d\n", i, graph->getEdge(i, j));
    }
  }
  graph->release();
  free(graph);
  return 0;
}

// Salvar as arestas ida e volta na mão 1 0 e 0 1