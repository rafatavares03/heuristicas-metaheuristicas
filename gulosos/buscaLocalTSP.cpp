#include <bits/stdc++.h>
#define endl '\n'

using namespace std;

vector<vector<int>> grafo;
vector<vector<int>> coordenadas;

int distancia(int i, int j) {
  int xd = coordenadas[i][1] - coordenadas[j][1]; 
  int yd = coordenadas[i][2] - coordenadas[j][2]; 
  return (int)(sqrt(xd*xd + yd*yd) + 0.5);
}

vector<vector<int>> matriz() {
  vector<vector<int>> g(coordenadas.size(), vector<int>(coordenadas.size(), -1));
  for(int i = 0; i < coordenadas.size(); i++) g[i][i] = 0;
  for(int i = 0; i < coordenadas.size(); i++) {
    for(int j = i+1; j < coordenadas.size(); j++){
      int peso = distancia(i, j);
      g[i][j] = peso;
      g[j][i] = peso;
    }
  }
  return g;
}

vector<int> solucaoVizinhosDistantes() {
  vector<int> caminho;
  vector<bool> vertices(grafo.size(), false);
  for(int i = 1; i < grafo.size(); i++) vertices.push_back(i);
  caminho.push_back(0);
  vertices[0] = true;
  while(caminho.size() < grafo.size()) {
    int maiorDistancia = -1, v = caminho.size() - 1, u = -1;
    for(int i = 0; i < grafo.size(); i++) {
      if(grafo[caminho[v]][i] > maiorDistancia && !vertices[i]) {
        maiorDistancia = grafo[caminho[v]][i];
        u = i;
      }
    }
    caminho.push_back(u);
    vertices[u] = true;
  }
  return caminho;
}

vector<int> solucaoVizinhosProximos() {
  vector<int> caminho;
  vector<bool> vertices(grafo.size(), false);
  for(int i = 1; i < grafo.size(); i++) vertices.push_back(i);
  caminho.push_back(0);
  vertices[0] = true;
  while(caminho.size() < grafo.size()) {
    int menorDistancia = 0x3f3f3f, v = caminho.size() - 1, u = -1;
    for(int i = 0; i < grafo.size(); i++) {
      if(grafo[caminho[v]][i] < menorDistancia && !vertices[i]) {
        menorDistancia = grafo[caminho[v]][i];
        u = i;
      }
    }
    caminho.push_back(u);
    vertices[u] = true;
  }
  return caminho;
}

int calculaCusto(vector<int> caminho) {
  int custo = 0;
  for(int i = 0; i < caminho.size()-1; i++) {
    custo += grafo[caminho[i]][caminho[i+1]];
  }
  custo += grafo[caminho[0]][caminho[caminho.size()-1]];
  return custo;
}

vector<int> solucaoAleatoria() {
  vector<int> vertices, caminho;
  for(int i = 0; i < grafo.size(); i++) vertices.push_back(i);
  srand(time(0));
  while(!vertices.empty()) {
    int v = rand() % vertices.size();
    caminho.push_back(vertices[v]);
    auto fim = remove(vertices.begin(), vertices.end(), vertices[v]);
    vertices.erase(fim, vertices.end());
  }
  return caminho;
}

vector<int> swap(vector<int> array, int i, int j) {
  vector<int> aux(array);
  int x = aux[i];
  aux[i] = aux[j];
  aux[j] = x;
  return aux;
}

vector<int> buscaLocal(vector<int> solucao) {
  int custo = calculaCusto(solucao);
  bool earlyStop = false;
  int it = 0;
  while(it < 1e6 && !earlyStop) {
    int i = 0;
    int custoInicial = custo;
    while(i < solucao.size()) {
      vector<int> solucaoAux(solucao);
      for(int j = i+1; j < solucao.size(); j++) {
        solucaoAux = swap(solucaoAux, i, j);
        int custoAux = calculaCusto(solucaoAux);
        if(custoAux < custo) {
          solucao = swap(solucao,i,j);
          custo = custoAux;
          i = solucao.size();
          j = solucao.size();
        } else{
          solucaoAux = swap(solucaoAux, i, j);
        }
      }
      i++;
    }
    if(custoInicial == custo) {
      earlyStop = true;
    }
    it++;
  }
  return solucao;
}

int main() {
  int v, x, y;
  while(cin >> v >> x >> y) {
    coordenadas.push_back({v,x,y});
  }
  grafo = matriz();
  vector<int> aleatoria = solucaoAleatoria();
  vector<int> vizinhosDistantes = solucaoVizinhosDistantes();
  vector<int> vizinhosProximos = solucaoVizinhosProximos();
  
  vector<int> solucao = buscaLocal(aleatoria);
  cout << "Solução inicial aleatória: " << endl;
  for(int i = 0; i < solucao.size(); i++) cout << solucao[i]+1 << " ";
  cout << solucao[0]+1 << endl << "Custo: "<< calculaCusto(solucao) << endl << endl;
  

  solucao = buscaLocal(vizinhosDistantes);
  cout << "Solução inicial vizinhos mais distantes: " << endl;
  for(int i = 0; i < solucao.size(); i++) cout << solucao[i]+1 << " ";
  cout << solucao[0]+1 << endl << "Custo: "<< calculaCusto(solucao) << endl << endl;


  solucao = buscaLocal(vizinhosProximos);
  cout << "Solução inicial vizinhos mais próximos: " << endl;
  for(int i = 0; i < solucao.size(); i++) cout << solucao[i]+1 << " ";
  cout << solucao[0]+1 << endl << "Custo: "<< calculaCusto(solucao) << endl;

  return 0;
}