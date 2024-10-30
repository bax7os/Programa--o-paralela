// Ana Clara Bastos Moraes
// Vinicius Santos Silva
// Algoritmo 1, 2 e 3 - Divididos em funções



#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <iostream>
#include <mutex>
#include <semaphore.h>
#include <set>
#include <stdio.h>
#include <thread>
#include <vector>

// estrutura do Grafo
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

 
std::condition_variable cv;
std::atomic<bool> done(false);
std::mutex mtx;
std::set<std::vector<int>> cliques_globais;


// processa cliques com roubo de trabalho entre threads
void processa_cliques_roubo(std::deque<std::vector<int>> &deque, Graph *grafo,
                            int k, int &contador,
                            std::vector<std::deque<std::vector<int>>> &deques,
                            int thread_id, int r) {
  std::vector<int> nova_clique;

  while (!done) {
    std::vector<int> clique;

    {
      std::unique_lock<std::mutex> lock(mtx);
      if (!deque.empty()) {
        clique = deque.back();
        deque.pop_back(); // obtem uma tarefa da fila da thread
      } else {

        // tentar roubar de outra thread

        bool roubado = false;
        for (int i = 0; i < deques.size(); ++i) {
          if (i != thread_id && !deques[i].empty()) {
            int num_to_steal = std::min(r, static_cast<int>(deques[i].size()));
            for (int j = 0; j < num_to_steal; ++j) {
              clique = deques[i].front();
              deques[i].pop_front();
              deque.push_back(clique); // redistribui o trabalho roubado
            }
            roubado = true;
            break;
          }
        }
        if (!roubado) {
          if (std::all_of(deques.begin(), deques.end(),
                          [](auto &d) { return d.empty(); })) {
            done = true; // marca como finalizado se todas as filas estiverem
                         // vazias
            cv.notify_all();
            return;
          } else {
            cv.wait(lock); // espera novos trabalhos
          }
          continue;
        }
      }
    }

    // processamento de clique atual
    if (clique.size() == k) {
      std::lock_guard<std::mutex> lock(mtx);
      if (cliques_globais.insert(clique).second) {
        contador++;
      }
      continue;
    }

    int ultimo_vertice = clique.back();
    for (int vertice : clique) {
      for (int j = 0; j < grafo->getEdgelistSize(vertice); j++) {
        int vizinho = grafo->getEdge(vertice, j);
        if (vizinho > ultimo_vertice &&
            std::find(clique.begin(), clique.end(), vizinho) == clique.end() &&
            std::all_of(clique.begin(), clique.end(), [&](int v) {
              return grafo->isNeighbour(v, vizinho);
            })) {
          nova_clique = clique;
          nova_clique.push_back(vizinho);
          {
            std::lock_guard<std::mutex> lock(mtx);
            deque.push_back(nova_clique);
            cv.notify_all(); // notifica que ha novas tarefas disponiveis
          }
        }
      }
    }
  }
}


// funcao principal para contagem de cliques com threads e roubo de trabalhos
int contagem_de_cliques_paralela_com_roubo(Graph *grafo, int k, int t, int r) {
  int contagem = 0;
  std::vector<std::deque<std::vector<int>>> deques(t);
  std::vector<int> contadores(t, 0);

// inicializa a fila de tarefas
  for (int i = 0; i < grafo->getVertices(); i++) {
    deques[i % t].push_back({i});
  }

  std::vector<std::thread> threads;
  auto start = std::chrono::high_resolution_clock::now();

  // inicializa as threads
  for (int i = 0; i < t; i++) {
    threads.push_back(std::thread(processa_cliques_roubo, std::ref(deques[i]),
                                  grafo, k, std::ref(contadores[i]),
                                  std::ref(deques), i, r));
  }

  // aguarda a finalizacao de todas as threads
  for (auto &th : threads) {
    th.join();
  }

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration = end - start;
  std::cout << "\nTempo de execução para k = " << k << " com " << t
            << " threads e roubo de " << r << " trabalhos: " << duration.count()
            << " segundos" << std::endl;

// agrupando os contadores de cada thread
  for (int c : contadores) {
    contagem += c;
  }

  return contagem;
}



// funcao que processa cliques sem roubo de trabalho
void processa_cliques(std::vector<std::vector<int>> cliques, Graph *grafo,
                      int k, int &contador, int thread_id) {
  std::cout << "Thread " << thread_id << " está processando os cliques..." << std::endl;
    // ... restante do código ...
  std::set<std::vector<int>> cliques_save;
  std::vector<int> nova_clique;

  while (!cliques.empty()) {
    std::vector<int> clique = cliques.back();
    cliques.pop_back();

    if (clique.size() == k) {
      std::lock_guard<std::mutex> lock(mtx);
      if (cliques_save.insert(clique).second) {
        contador++;
      }
      continue;
    }

    int ultimo_vertice = clique.back();
    for (int vertice : clique) {
      for (int j = 0; j < grafo->getEdgelistSize(vertice); j++) {
        int vizinho = grafo->getEdge(vertice, j);
        if (vizinho > ultimo_vertice &&
            std::find(clique.begin(), clique.end(), vizinho) == clique.end() &&
            std::all_of(clique.begin(), clique.end(), [&](int v) {
              return grafo->isNeighbour(v, vizinho);
            })) {
          nova_clique = clique;
          nova_clique.push_back(vizinho);
          cliques.push_back(nova_clique);
        }
      }
    }
  }
}



// funcao principal para contagem de cliques paralela
int contagem_de_cliques_paralela(Graph *grafo, int k, int t) {
  int contagem = 0;
  std::vector<std::vector<int>> cliques;
  for (int i = 0; i < grafo->getVertices(); i++) {
    cliques.push_back({i});
  }

  std::vector<std::thread> threads;
  std::vector<int> contadores(t, 0);
  int tamanho_parte = cliques.size() / t;

  auto start =
      std::chrono::high_resolution_clock::now(); 

  for (int i = 0; i < t; i++) {
    std::vector<std::vector<int>> parte_cliques(
        cliques.begin() + i * tamanho_parte,
        cliques.begin() + (i + 1) * tamanho_parte);
   threads.push_back(std::thread(processa_cliques, parte_cliques, grafo, k, std::ref(contadores[i]), i));
  }

  for (auto &th : threads) {
    th.join();
  }

  auto end =
      std::chrono::high_resolution_clock::now(); // Fim da medição do tempo
  std::chrono::duration<double> duration = end - start;
  std::cout << "\nTempo de execução para k = " << k << " com " << t
            << " threads: " << duration.count() << " segundos" << std::endl;

  for (int c : contadores) {
    contagem += c;
  }

  return contagem;
}



// funcao principal para contagem de cliques serial
int contagem_de_cliques_serial(Graph *grafo, int k) {
  int contagem = 0;
  std::vector<std::vector<int>> cliques;
  std::vector<int> nova_clique;
  std::set<std::vector<int>> cliques_save;

  for (int i = 0; i < grafo->getVertices(); i++) {
    cliques.push_back({i});
  }

  auto start =
      std::chrono::high_resolution_clock::now(); // Início da medição do tempo

  while (!cliques.empty()) {
    std::vector<int> clique = cliques.back();
    cliques.pop_back();

    if (clique.size() == k) {
      if (cliques_save.insert(clique).second) {
        contagem++;
      }
      continue;
    }

    int ultimo_vertice = clique.back();

    for (int vertice : clique) {
      for (int j = 0; j < grafo->getEdgelistSize(vertice); j++) {
        int vizinho = grafo->getEdge(vertice, j);
        if (vizinho > ultimo_vertice &&
            std::find(clique.begin(), clique.end(), vizinho) == clique.end() &&
            std::all_of(clique.begin(), clique.end(), [&](int v) {
              return grafo->isNeighbour(v, vizinho);
            })) {
          nova_clique = clique;
          nova_clique.push_back(vizinho);
          cliques.push_back(nova_clique);
        }
      }
    }
  }

  auto end =
      std::chrono::high_resolution_clock::now(); // Fim da medição do tempo
  std::chrono::duration<double> duration = end - start;
  std::cout << "\nTempo de execução para k = " << k << ": " << duration.count()
            << " segundos" << std::endl;

  return contagem;
}

// função principal
int main(int argc, char *argv[]) {
  int k = atoi(argv[1]);
  int t = atoi(argv[2]);
  int r = atoi(argv[3]);
  //printf("Valor de k: %d\n", k);
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

  fclose(fp);

  printf("Cliques serial: %d", contagem_de_cliques_serial(graph, k));
  printf("Cliques paralela: %d", contagem_de_cliques_paralela(graph, k, t));
  printf("Cliques paralela balanceada: %d", contagem_de_cliques_paralela_com_roubo(graph, k, t, r));
  // graph->printEdgelist();

  graph->release();
  free(graph);
  return 0;
}

