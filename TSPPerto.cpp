#include <bits/stdc++.h>
#define endl '\n'

using namespace std;

vector<vector<pair<int,int>>> listaDeAdjacencia(int vertices, vector<vector<int>> arestas) {
    vector<vector<pair<int,int>>> g(vertices);
    for(int i  = 0; i < arestas.size(); i++) {
        int v1 = arestas[i][0] - 1;
        int v2 = arestas[i][1] - 1;
        int peso = arestas[i][2];
        g[v1].push_back(make_pair(v2, peso));
        g[v2].push_back(make_pair(v1, peso));
    }
    return g;
}

void printListaDeAdjacencia(vector<vector<pair<int,int>>> g) {
    cout << "LISTA DE ADJACÊNCIA" << endl;
    for(int i = 0; i < g.size(); i++) {
        cout << "Vertice " << i+1 << ": ";
        for(int j = 0; j < g[i].size(); j++) {
            cout << "(" << g[i][j].first +1 << ", " << g[i][j].second  << ") ";
        }
        cout << endl;
    }
}

vector<int> solucaoInicial(vector<vector<pair<int,int>>> grafo) {
    vector<int> caminho;
    bool adicionados[grafo.size()] = {false};
    caminho.push_back(0);
    adicionados[0] = true;
    while(caminho.size() < grafo.size()) {
        int u = caminho[caminho.size() - 1];
        cout << u << endl;
        int vizinhoMaisProximo = -1;
        int distanciaVizinho = 0x3f3f3f;
        for(int v = 0; v < grafo[u].size(); v++) {
            if(!adicionados[grafo[u][v].first]) {
                if(grafo[u][v].second < distanciaVizinho) {
                    cout << "entrou " << grafo[u][v].second; 
                    vizinhoMaisProximo = v;
                    //distanciaVizinho = grafo[u][v].second;
                }
            }
        }
        if(vizinhoMaisProximo != -1) {
            caminho.push_back(grafo[u][vizinhoMaisProximo].first);
            adicionados[grafo[u][vizinhoMaisProximo].first] = true;
        }
    }
    return caminho;
}

int main() {
    int vertices, arestas;
    cin >> vertices >> arestas;
    vector<vector<int>> dados(arestas, vector<int>(3));
    for(int i = 0; i < arestas; i++) {
        int v1, v2, peso;
        cin >> v1 >> v2 >> peso;
        dados[i][0] = v1;
        dados[i][1] = v2;
        dados[i][2] = peso;
    }
    vector<vector<pair<int,int>>> grafo = listaDeAdjacencia(vertices, dados);
    printListaDeAdjacencia(grafo);
    vector<int> caminho = solucaoInicial(grafo);
    for(int i = 0; i < caminho.size(); i++) {
        cout << caminho[i]+1 << " ";
    }
    cout << caminho[0]+1 << endl;
    return 0;
}