#include <bits/stdc++.h>
#include <chrono>
#define endl '\n'
#define INF 0x3f3f3f3f

using namespace std;
using namespace std::chrono;

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

void lerEntrada() {
  int v, x, y;
  while(cin >> v >> x >> y) {
    coordenadas.push_back({v,x,y});
  }
  grafo = matriz();
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

void printSolucaoCSV(vector<int> solucao, duration<double> tempo) {
  for(int i = 0; i < solucao.size(); i++) {
    cout << solucao[i]+1 << " ";
  }
  cout << solucao[0]+1 << "," << calculaCusto(solucao) << ",";
  cout << fixed << setprecision(3);
  cout << tempo.count() << endl;
}

void printSolucao(vector<int> solucao, duration<double> tempo) {
  cout << "Solução: ";
  for(int i = 0; i < solucao.size(); i++) cout << solucao[i] << " ";
  cout << solucao[0] << endl;
  cout << "Custo: " << calculaCusto(solucao) << endl;
  cout << "Tempo de processamento: ";
  cout << fixed << setprecision(3);
  cout << tempo.count() << " segundos." << endl;
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

vector<int> solucaoInicial() {
  vector<int> vertices, caminho;
  for(int i = 0; i < grafo.size(); i++) vertices.push_back(i);
  while(!vertices.empty()) {
    int v = random(0, vertices.size()-1);
    caminho.push_back(vertices[v]);
    auto fim = remove(vertices.begin(), vertices.end(), vertices[v]);
    vertices.erase(fim, vertices.end());
  }
  return caminho;
}

vector<vector<int>> geraVizinhancaOrdenada(vector<int> solucao) {
  vector<vector<int>> vizinhos;
  for(int i = 0; i < grafo.size(); i++) {
    for(int j = i+1; j < grafo.size(); j++) {
      int c = calculaCusto(swap(solucao, i, j));
      vizinhos.push_back({i,j,c});
    }
  }
  sort(vizinhos.begin(), vizinhos.end(), [](vector<int> &a, vector<int> &b) {
    return a[2] < b[2];
  });

  return vizinhos;
}

vector<int> buscaTabu(vector<int> solucao) {
  vector<int> melhorSolucao = solucao;
  int melhorCusto = calculaCusto(solucao);

  vector<vector<int>> t(grafo.size(), vector<int>(grafo.size(), 0));
  double dt = 0.3;
  int duracaoTabu = max(1, (int)(dt * grafo.size()));
  int it = 0, stop = 50;
  while(it < stop) {
    vector<vector<int>> vizinhos = geraVizinhancaOrdenada(solucao);
    vector<int> melhorTroca;
    int custo = INF;
    int k = 0;
    int u, v;
    while(melhorTroca.empty() && k < vizinhos.size()) {
      u = vizinhos[k][0];
      v = vizinhos[k][1];
      int novoCusto = vizinhos[k][2];
      if(t[min(u,v)][max(u,v)] > 0) {
        if(novoCusto < melhorCusto) {
          melhorTroca = swap(solucao, u, v);
          custo = novoCusto;
        }
      } else if(novoCusto < custo) {
          melhorTroca = swap(solucao, u, v);
          custo = novoCusto;
        }
      k++;
    }
    if(melhorTroca.empty()) {
      int menor = INF;
      for(int i = 0; i < t.size(); i++) {
        for(int j = i+1; j < t[i].size(); j++) {
          if(t[i][j] < menor) {
            u = i;
            v = j;
            menor = t[i][j];
          }
        }
      }
      melhorTroca = swap(solucao, u, v);
    }
    for(int i = 0; i < t.size(); i++) {
      for(int j = i+1; j < t.size(); j++) {
        if(t[i][j] > 0) t[i][j]--;
      }
    }
    t[min(u,v)][max(u,v)] = duracaoTabu;
    solucao = melhorTroca;
    if(custo < melhorCusto) {
      melhorSolucao = solucao;
      melhorCusto = custo;
    }
    it++;
  }

  return melhorSolucao;
}

vector<int> perturbacao(vector<int> solucao, int k) {
  vector<int> s(solucao);
  for(int i = 0; i < k; i++) {
    int u = random(0, s.size()-1);
    int v = random(0, s.size()-1);
    swap(s, u, v);
  }
  return s;
}

vector<int> VNS() {
  vector<int> solucao = solucaoInicial();
  int it = 0;
  while(it < 10) {
    it++;
    int k = 1;
    while(k <= 5) {
      vector<int> s = perturbacao(solucao, k);
      vector<int> aux = buscaTabu(s);
      if(calculaCusto(aux) < calculaCusto(solucao)) {
        solucao = aux;
        k = 1;
        it = 0;
      } else {
        k++;
      }
    }
  }
  return solucao;
}

int main(int argc, char* argv[]){
  lerEntrada();
  auto inicio = high_resolution_clock::now();
  vector<int> solucao = VNS();
  auto fim = high_resolution_clock::now();
  duration<double> tempo = fim - inicio;
  printSolucao(solucao, tempo);
  return 0;
}