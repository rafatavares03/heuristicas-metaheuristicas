#include <bits/stdc++.h>
#include <chrono>
#define endl '\n'
#define INF 0x3f3f3f3f

using namespace std;
using namespace std::chrono;

int config;
bool maxItStop, outInCSVFormat;
int quantidadeDeFormigas;

double p;
int Q = 10;

vector<vector<int>> grafo;
vector<vector<int>> coordenadas;
vector<vector<double>> feromonios;

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
  quantidadeDeFormigas = (config == 1) ? g.size() : g.size() / 2;
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

double doubleRandom(double lowerbound, double upperbound) {
  mt19937 eng(std::chrono::high_resolution_clock::now().time_since_epoch().count());
  uniform_real_distribution<double> distrib(lowerbound, upperbound);

  double numero = distrib(eng);
  return numero;
}

void removeElemento(vector<int> &array, int elemento) {
  auto fim = remove(array.begin(), array.end(), elemento);
  array.erase(fim, array.end());
}

int roleta(int u, vector<int> vertices) {
  vector<double> probabilidades;
  double f = 0;

  for(int i = 0; i < vertices.size(); i++){
    f += (feromonios[u][vertices[i]] * ((double)1 / grafo[u][vertices[i]]));
  } 

  double acumulado = 0.0;
  for (int i = 0; i < vertices.size(); i++) {
    double peso = feromonios[u][vertices[i]] * (1.0 / grafo[u][vertices[i]]);
    acumulado += peso / f;
    probabilidades.push_back(acumulado);
  }

  double x = doubleRandom(0,1);
  int v = lower_bound(probabilidades.begin(), probabilidades.end(), x) - probabilidades.begin();
  return v;
}

vector<int> geraCaminho() {
  vector<int> vertices;
  vector<int> trilha;

  for(int i = 0; i < grafo.size(); i++) vertices.push_back(i);
  int inicio = random(0, vertices[vertices.size()-1]);

  removeElemento(vertices, inicio);
  trilha.push_back(inicio);

  while(!vertices.empty()) {
    int u = trilha[trilha.size() - 1];
    int v = roleta(u, vertices);

    trilha.push_back(vertices[v]);
    removeElemento(vertices, vertices[v]);
  }
  return trilha;
}

vector<int> melhorCaminho(vector<vector<int>> trilhas) {
  vector<pair<int, int>> custos;
  for(int i = 0; i < trilhas.size(); i++) {
    custos.push_back({i, calculaCusto(trilhas[i])});
  }

  sort(custos.begin(), custos.end(), [](pair<int, int>&a, pair<int, int>&b) {
    return a.second < b.second;
  });

  return trilhas[custos[0].first];
}

vector<int> coloniaDeFormigas() {
  vector<vector<double>> f(grafo.size(), vector<double>(grafo.size(), 1));
  feromonios = f;
  vector<int> melhorSolucao;
  int melhorCusto = INF;
  int it = 0;
  int itMAX = (config == 1) ? 300 : 500;
  while(it < itMAX) {
    vector<vector<int>> formigas;
    for(int i = 0; i < quantidadeDeFormigas; i++) {
      formigas.push_back(geraCaminho());
    }

    for(int i = 0; i < feromonios.size(); i++) {
      for(int j = i+1; j < feromonios.size(); j++) {
        feromonios[i][j] *= (1 - p);
        feromonios[j][i] *= (1 - p);
      }
    }

    vector<int> melhorTrilha = melhorCaminho(formigas);
    int custo = calculaCusto(melhorTrilha);
    for(int i = 0; i < melhorTrilha.size()-1; i++) {
      int u = melhorTrilha[i];
      int v = melhorTrilha[i+1];
      double reforco = (double)Q / (double)custo;
      feromonios[u][v] += reforco;
      feromonios[v][u] += reforco;
    }
    if(custo < melhorCusto) {
      melhorSolucao = melhorTrilha;
      melhorCusto = custo;
    }

    it++;
  }
  return melhorSolucao;
}

void gerenciarConfiguracoes(int argc, char* argv[]){
  config = 1;
  outInCSVFormat = false;
  if(argc > 1) {
    if(strcmp(argv[1], "2") == 0) {
      config = 2;
    }
  }
  if(argc > 2) {
    if(strcmp(argv[2], "1") == 0) {
      outInCSVFormat = true;
    }
  }
  
  p = (config == 1) ? 0.01 : 0.05;

}

int main(int argc, char* argv[]){
  gerenciarConfiguracoes(argc, argv);
  lerEntrada();

  auto inicio = high_resolution_clock::now();
  vector<int> solucao = coloniaDeFormigas();
  auto fim = high_resolution_clock::now();

  duration<double> tempo = fim - inicio;
  if(outInCSVFormat) {
    printSolucaoCSV(solucao, tempo);
  } else {
    printSolucao(solucao, tempo);
  }
  return 0;
}