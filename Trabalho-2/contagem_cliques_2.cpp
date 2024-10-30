#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <omp.h>
#include <ostream>
#include <set>
#include <vector>


typedef struct Graph {

  int *edgelistSize;
  int **edgelist;
  int vertices = 0;
  int *countersPerVertex;

  void initialize(const char *graphDataset) {
    FILE *fp = fopen(graphDataset, "r");
    int src, dst;
    int maxVertexId = -1;

    // Primeira passada para encontrar o número máximo de vértices
    while (fscanf(fp, "%d %d", &src, &dst) != EOF) {
      if (src > maxVertexId)
        maxVertexId = src;
      if (dst > maxVertexId)
        maxVertexId = dst;
    }

    vertices = maxVertexId + 1;
    rewind(fp);

    edgelistSize = (int *)calloc(vertices, sizeof(int));

    // segunda passada para contar as arestas e alocar espaço, incluindo as
    // bidirecionais
    while (fscanf(fp, "%d %d", &src, &dst) != EOF) {
      edgelistSize[src]++;
      edgelistSize[dst]++; // considerando a adição bidirecional
    }

    // alocação de memória para as arestas
    edgelist = (int **)calloc(vertices, sizeof(int *));
    for (int i = 0; i < vertices; i++) {
      edgelist[i] = (int *)calloc(edgelistSize[i], sizeof(int));
    }

    countersPerVertex = (int *)calloc(vertices, sizeof(int));
    rewind(fp);
  }

  int getVertices() { return vertices; }

  int getEdgelistSize(int src) { return edgelistSize[src]; }

  int getEdge(int src, int pos) { return edgelist[src][pos]; }

  void addEdge(int src, int dst) {
    int pos = countersPerVertex[src];
    edgelist[src][pos] = dst;
    countersPerVertex[src]++;
    // printf(" ADDEDGE: src: %d dst: %d %d\n", src, dst, edgelist[src][pos]);
  }

  bool isNeighbour(int src, int dst) {
    for (int i = 0; i < getEdgelistSize(src); i++) {
      if (edgelist[src][i] == dst) {
        return true;
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
  void printEdgelist() {
    for (int i = 0; i < getVertices(); i++) {
      for (int j = 0; j < getEdgelistSize(i); j++) {
        printf("src: %d dst: %d\n", i, getEdge(i, j));
      }
    }
  }

} Graph;
void printVector(const std::vector<int>& vec) {
  for (int v : vec) {
    std::cout << v << " ";
  }
  std::cout << std::endl;
}

int contagem_de_cliques_paralela_openmp(Graph* grafo, int k) {
    int contagem = 0;
    std::vector<std::vector<int>> cliques;
    std::set<std::vector<int>> cliques_save;

    #pragma omp parallel
    {
        std::vector<std::vector<int>> cliques_local;
        #pragma omp for schedule(dynamic)
        for (int i = 0; i < grafo->getVertices(); i++) {
            cliques_local.push_back({i});
            printf("Thread %d adicionou vértice %d à lista de cliques\n", omp_get_thread_num(), i);
        }

        while (!cliques_local.empty()) {
            std::vector<int> clique = cliques_local.back();
            cliques_local.pop_back();

            printf("Thread %d processando clique: ", omp_get_thread_num());
            printVector(clique);

            if (clique.size() == k) {
                printf("Thread %d encontrou clique de tamanho %d: ", omp_get_thread_num(), k);
                printVector(clique);
                #pragma omp critical
                {
                    if (cliques_save.insert(clique).second) {
                        printf("Thread %d adicionou clique à lista de cliques salvos\n", omp_get_thread_num());
                        #pragma omp atomic
                        contagem++;
                    }
                }
                continue;
            }

            int ultimo_vertice = clique.back();
            for (int vertice : clique) {
                for (int j = 0; j < grafo->getEdgelistSize(vertice); j++) {
                    int vizinho = grafo->getEdge(vertice, j);
                    if (vizinho > ultimo_vertice &&
                        std::find(clique.begin(), clique.end(), vizinho) == clique.end() &&
                        std::all_of(clique.begin(), clique.end(), [&](int v) { return grafo->isNeighbour(v, vizinho); })) {
                        std::vector<int> nova_clique = clique;
                        nova_clique.push_back(vizinho);
                        cliques_local.push_back(nova_clique);
                        printf("Thread %d adicionou novo clique à lista: ", omp_get_thread_num());
                        printVector(nova_clique);
                    }
                }
            }
        }
    }

    return contagem;
}
int main(int argc, char *argv[]) {
  int k = atoi(argv[1]);
  //int r = atoi(argv[3]);
  // printf("Valor de k: %d\n", k);
  Graph *graph = new Graph;
  graph->initialize("graph.edgelist");
  FILE *fp = fopen("graph.edgelist", "r");
  int src = 0, dst = 0;

  while (fscanf(fp, "%d %d", &src, &dst) != EOF) {
    graph->addEdge(src, dst);
    printf("src: %d dst: %d \n", src, dst);
    if (!(graph->isNeighbour(dst, src))) {
      graph->addEdge(dst, src);
    }
  }
 printf("Cliques tamanho %d em contagem de cliques escalonando:%d\n", k, contagem_de_cliques_paralela_openmp(graph,  k));
  fclose(fp);
  // graph->printEdgelist();

  graph->release();
  free(graph);
  return 0;
}


