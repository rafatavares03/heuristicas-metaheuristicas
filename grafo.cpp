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

vector<vector<int>> matriz(int vertices, vector<vector<int>> arestas) {
    vector<vector<int>> g(vertices, vector<int>(vertices, -1));
    for(int i = 0; i < vertices; i++) g[i][i] = 0;
    for(int i = 0; i < arestas.size(); i++) {
        int v1 = arestas[i][0] - 1;
        int v2 = arestas[i][1] - 1;
        int peso = arestas[i][2];
        g[v1][v2] = peso;
        g[v2][v1] = peso;
    }
    return g;
}

void printMatriz(vector<vector<int>> g) {
    cout << "MATRIZ" << endl;
    for(int i = 0; i < g.size(); i++) {
        for(int j = 0; j < g.size(); j++) {
            cout << g[i][j] << '\t';
        }
        cout << endl;
    }
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
    vector<vector<pair<int,int>>> grafoLista = listaDeAdjacencia(vertices, dados);
    printListaDeAdjacencia(grafoLista);
    cout << endl;
    vector<vector<int>> grafoMatriz = matriz(vertices, dados);
    printMatriz(grafoMatriz);
}