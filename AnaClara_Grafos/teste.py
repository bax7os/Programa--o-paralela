class Grafo:
    def __init__(self, vertices):
        self.vertices = vertices  # Lista de vértices
        self.adjacencias = {v: set() for v in vertices}  # Mapa de adjacências para cada vértice

    def adicionar_aresta(self, v1, v2):
        self.adjacencias[v1].add(v2)
        self.adjacencias[v2].add(v1)

    def vizinhos(self, vertice):
        return self.adjacencias[vertice]

def contagem_de_cliques_serial(g: Grafo, k: int):
    cliques = [[v] for v in g.vertices]  # Inicia com cada vértice como um clique de tamanho 1
    contador = 0
    
    while cliques:
        clique = cliques.pop()

        # Se a clique atual já tem tamanho k
        if len(clique) == k:
            contador += 1
            continue

        ultimo_vertice = clique[-1]
        
        for vertice in clique:
            for vizinho in g.vizinhos(vertice):
                if (vizinho not in clique) and all(v in g.vizinhos(vizinho) for v in clique) and vizinho > ultimo_vertice:
                    nova_clique = clique.copy()
                    nova_clique.append(vizinho)
                    cliques.append(nova_clique)
    
    return contador

# Exemplo de uso:
g = Grafo([0, 1, 2, 3, 4, 5, 6, 7])
g.adicionar_aresta(0, 1)
g.adicionar_aresta(0, 5)
g.adicionar_aresta(0, 2)
g.adicionar_aresta(1, 2)
g.adicionar_aresta(2, 3)
g.adicionar_aresta(3, 7)
g.adicionar_aresta(3, 4)
g.adicionar_aresta(3, 5)
g.adicionar_aresta(4, 5)
g.adicionar_aresta(4, 7)
g.adicionar_aresta(5, 7)
k = 3
print(f"Número de cliques de tamanho {k}: {contagem_de_cliques_serial(g, k)}")
