#include <bits/stdc++.h>
#define endl '\n'
#define INF 0x3f3f3f3f

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

int calculaCusto(vector<int> caminho) {
  if(caminho.empty()) return 0;
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

int random(int lowerbound, int upperbound) {
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> distrib(lowerbound, upperbound);

  int numero = distrib(gen);
  return numero;
}

vector<int> construcaoGulosaAleatoria(double alpha) {
  vector<int> solucao;
  solucao.push_back(0);
  vector<int> candidatos(grafo.size()-1);
  for(int i = 0; i < candidatos.size(); i++) candidatos[i] = i+1;
  while(!candidatos.empty()){
    int maior = -INF, menor = INF;
    vector<pair<int, int>> custos;
    vector<int> aux(solucao);
    for(int i = 0; i < candidatos.size(); i++) {
      aux.push_back(candidatos[i]);

      int custoAux = calculaCusto(aux);
      maior = (custoAux > maior) ? custoAux : maior;
      menor = (custoAux < menor) ? custoAux : menor;

      custos.push_back({candidatos[i], custoAux});
      aux.pop_back();
    }

    int teto = menor + (alpha * (maior - menor));
    vector<int> lrc;
    for(int i = 0; i < candidatos.size(); i++) {
      if(custos[i].second <= teto) {
        lrc.push_back(custos[i].first);
      }
    }
    
    int v = random(0, lrc.size()-1);
    solucao.push_back(lrc[v]);
    auto fim = remove(candidatos.begin(), candidatos.end(), lrc[v]);
    candidatos.erase(fim, candidatos.end());
  }

  return solucao;
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

vector<int> GRASP() {
  vector<int> melhorSolucao;
  int melhorCusto = INF;
  int it = 0;
  while(it < 30) {
    vector<int> solucao = construcaoGulosaAleatoria(0.3);
    solucao = buscaLocal(solucao);
    int custo = calculaCusto(solucao);
    if(custo < melhorCusto) {
      melhorSolucao = solucao;
      melhorCusto = custo;
    }
    it++;
  }
  return melhorSolucao;
}

int main() {
  int v, x, y;
  while(cin >> v >> x >> y) {
    coordenadas.push_back({v,x,y});
  }
  grafo = matriz();
  vector<int> caminho = GRASP();
  cout << "Solução: ";
  for(int i = 0; i < caminho.size(); i++) {
    cout << caminho[i]+1 << " ";
  }
  cout << caminho[0]+1 << endl;
  cout << "Custo: "<< calculaCusto(caminho) << endl;
  return 0;
}