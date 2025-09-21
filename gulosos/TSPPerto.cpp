#include <bits/stdc++.h>
#define endl '\n'

using namespace std;

vector<vector<pair<int,int>>> grafo;
vector<bool> visitados;

vector<vector<pair<int,int>>> listaDeAdjacencia() {
    int vertices, arestas;
    cin >> vertices >> arestas;
    vector<vector<pair<int,int>>> g(vertices);

    for(int i  = 0; i < arestas; i++) {
        int v1, v2, peso;
        cin >> v1 >> v2 >> peso;
        v1--; v2--;
        g[v1].push_back(make_pair(v2, peso));
        g[v2].push_back(make_pair(v1, peso));
    }

    return g;
}

bool adjacentes(int u, int v) {
    for(int i = 0; i < grafo[u].size(); i++) {
        if(grafo[u][i].first == v) return true;
    }
    return false;
}

bool backtracking(vector<int>& caminho) {
    if(caminho.size() == grafo.size()) {
        return adjacentes(caminho[0], caminho[caminho.size() - 1]);
    }

    int v = caminho[caminho.size() - 1];
    vector<pair<int,int>> vizinhos(grafo[v]);
    sort(vizinhos.begin(), vizinhos.end(), [](pair<int,int> &a, pair<int,int> &b){
        return a.second < b.second;
    });

    for(int i = 0; i < vizinhos.size(); i++) {
        if(visitados[vizinhos[i].first]) continue;

        caminho.push_back(vizinhos[i].first);
        visitados[vizinhos[i].first] = true;

        bool sucesso = backtracking(caminho);
        if(sucesso) {
            return true;
        } else {
            caminho.pop_back();
            visitados[vizinhos[i].first] = false;
        }
    }
    return false;
}

int custo(int u, int v) {
  for(int i = 0; i < grafo[u].size(); i++) {
    if(grafo[u][i].first == v) return grafo[u][i].second;
  }
  return 0;
}

int calculaCusto(vector<int> array) {
    int total = 0;
    for(int i = 0; i < array.size() - 1; i++) {
      total += custo(array[i], array[i+1]);
    }
    total += custo(array[0], array[array.size() - 1]);
    return total;
}

void printSolucao(vector<int> solucao) {
    if(solucao.empty()) {
        cout << "Não foi possível encontrar uma solução." << endl;
        return;
    }

    cout << "Solução: ";
    for(int i = 0; i < solucao.size(); i++) cout << solucao[i]+1 << " - ";
    cout << solucao[0]+1 << endl;
    cout << "Custo: " << calculaCusto(solucao) << endl;;
}

vector<int> solucaoInicial() {
    vector<int> caminho;
    caminho.push_back(0);
    visitados[0] = true;
    bool solucao = backtracking(caminho);
    if(!solucao) caminho.pop_back();
    return caminho;
}

int main() {
    grafo = listaDeAdjacencia();
    for(int i = 0; i < grafo.size(); i++) visitados.push_back(false);
    vector<int> solucao = solucaoInicial();
    printSolucao(solucao);
    return 0;
}