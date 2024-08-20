#include <stdio.h>
#include <cstring>

typedef struct graph
{
    int *edgelistSize;
    int** edgelist;
    int vertices;
    void initialize(const char* graphDataset);
    void addEdge(int src, int dst);
    bool isNeighbour(int src, int dst);
    void realase(){

    }
}Graph;


int main(int argc, char *argv[])
{
    
    FILE* fp = fopen("graph.edgelist", "r");

    int src, dst;
    int maxVertexId = -1;
    while (fscanf(fp, "%d %d", &src, &dst) != EOF)
    {
        if (src > maxVertexId)
        {
            maxVertexId = src;
        }
        if (dst > maxVertexId)
        {
            maxVertexId = dst;
        }
        
        
        printf("%d %d\n", src, dst);
    }
    this->vertices = maxVertexId+1;
    
    rewind(fp);
    edgelistSize = (int**)malloc(vertices * sizeof(int));
    while (fscanf(fp, "%d %d", &src, &dst) != EOF)
    {
        edgelistSize[src]++;
    }
    
    edgelist = (int**)malloc(vertices * sizeof(int));
    for (int i = 0; i < vertices; i++)
    {
        edgelist[i] = (int*)malloc(edgelistSize[i] *sizeof(int));
    }
    rewind(fp);
    int* countersPerVertex = (int*)malloc(vertices*sizeof(int));
    memset(countersPerVertex, 0, vertices*sizeof(int));

    while (fscanf(fp, "%d %d", &src, &dst) != EOF)
    {
        edgelist[src][countersPerVertex[src]++] = dst; 
    }

    
    for (int i = 0; i < vertices; i++)
    {
        for (int j = 0; j < edgelistSize[i]; j++)
        {
            printf("%d %d\n", i, edgelist[i][j]);
        }
        
    }
    
    for (int i = 0; i < vertices; i++)
    {
        free(edgelist[i]);
    }
    free(edgelist);

   return 0;
}