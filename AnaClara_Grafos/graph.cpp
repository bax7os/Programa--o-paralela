#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdio.h>

typedef struct Graph {

  int *edgelistSize = 0;
  int **edgelist= 0;
  int vertices = 0;
  int *countersPerVertex= 0;

  void initialize(const char *graphDataset) {
    FILE *fp = fopen("graph.edgelist", "r");
    int src= 0, dst=0, i = 0, contador_dest = 0;
    

    while (fscanf(fp, "%d %d", &src, &dst) != EOF) {
      contador_dest++;
    }
    printf("contador_destt = %d\n", contador_dest);
    rewind(fp);

    int vertices_dest[contador_dest], cont = 0;
    for (int i = 0; i < contador_dest; i++) {
        vertices_dest[i] = -1;
    }
    while (fscanf(fp, "%d %d", &src, &dst) != EOF) {
      if (i == 0) {
        vertices_dest[i] = dst;
        vertices++;
        i++;
      } else {
        for (int j = 0; j < contador_dest; j++) {
          if (dst == vertices_dest[j]) {
            cont++;
          }
        }
        if (cont == 0) {
            vertices_dest[i] = dst;
            i++;
            vertices++;
        }
      }
      cont = 0;
    }
    printf("vertices = %d\n", vertices);
    for (int j = 0; j < i; j++) {
      printf("v %d \n", vertices_dest[j]);
    }

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
    printf("\n");
    countersPerVertex = (int *)malloc(vertices * sizeof(int));
    memset(countersPerVertex, 0, vertices * sizeof(int));
    rewind(fp);

  }

  int getVertices() { return vertices; }

  int getEdgelistSize(int src) { return edgelistSize[src]; }

  int getEdge(int src, int pos) { return edgelist[src][pos]; }

  void addEdge(int src, int dst) {
    printf("%d %d -- \n", src, dst);
    edgelist[src][countersPerVertex[src]++] = dst;
  }

  // TODO
  bool isNeighbour(int src, int dst) { return true; }
  void release() {
    for (int i = 0; i < vertices; i++)
      free(edgelist[i]);
    free(edgelist);
    free(countersPerVertex);
    free(edgelistSize);
  }
} Graph;

int main() {
  Graph *graph = new Graph;
  graph->initialize("graph.edgelist");
  FILE *fp = fopen("graph.edgelist", "r");
  int src=0, dst=0;

  while (fscanf(fp, "%d %d", &src, &dst) != EOF) {
    graph->addEdge(src, dst);
    printf("src: %d dst: %d \n", src, dst);
  }
  
  printf("%d \n", graph->getVertices());


  fclose(fp);
printf("oi");
  for(int i = 0 ; i < graph->getVertices() ; i++) {
        for(int j = 0 ; j < graph->getEdgelistSize(i) ; j++) {
          printf("%d %d\n", i, graph->getEdge(i, j));
    }
  }
  graph->release();
  free(graph);
  return 0;
}