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

bool adjacentes(vector<vector<pair<int,int>>> grafo, int u, int v) {
  for(int i = 0; i < grafo[u].size(); i++) {
    if(grafo[u][i].first == v) return true;
  }
  return false;
}

void caminhos(vector<vector<pair<int,int>>> grafo,vector<int> caminho, vector<int> disponiveis) {
  if(disponiveis.empty()) {
    if(!adjacentes(grafo, caminho[0], caminho[caminho.size()-1])) return;
    for(int i = 0; i < caminho.size(); i++) cout << caminho[i] + 1 << " ";
    cout << caminho[0] + 1 << endl;
    return;
  }
  for(int i = 0; i < disponiveis.size(); i++) {
    if(caminho.empty() || adjacentes(grafo, caminho[caminho.size()-1], disponiveis[i])) {
      vector<int> caminhoAux(caminho), dispAux(disponiveis);

      caminhoAux.push_back(disponiveis[i]);
      auto fim = remove(dispAux.begin(), dispAux.end(), disponiveis[i]);
      dispAux.erase(fim, dispAux.end());
      caminhos(grafo,caminhoAux, dispAux);
    }
  }
  return;
}

int main() {
  int vertices, arestas;
  cin >> vertices >> arestas;
  vector<int> array, disponiveis;
  vector<vector<int>> dados(arestas, vector<int>(3));

  for(int i = 0; i < vertices; i++) disponiveis.push_back(i);
  for(int i = 0; i < arestas; i++) {
      int v1, v2, peso;
      cin >> v1 >> v2 >> peso;
      dados[i][0] = v1;
      dados[i][1] = v2;
      dados[i][2] = peso;
  }
  vector<vector<pair<int,int>>> grafo = listaDeAdjacencia(vertices, dados);
  caminhos(grafo, array, disponiveis);
  return 0;
}