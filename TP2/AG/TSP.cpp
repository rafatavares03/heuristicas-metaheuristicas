#include <bits/stdc++.h>
#include <chrono>
#define endl '\n'
#define INF 0x3f3f3f3f

using namespace std;
using namespace std::chrono;

int config;
bool maxItStop, outInCSVFormat;
int individuosQtd = 100;

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

void removeElemento(vector<int> &array, int elemento) {
  auto fim = remove(array.begin(), array.end(), elemento);
  array.erase(fim, array.end());
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

void removeElementoPorIndice(vector<pair<int, double>> &array, int indice) {
  if (indice >= 0 && indice < array.size()) {
    array.erase(array.begin() + indice);
  }
}

vector<int> solucaoAleatoria() {
  vector<int> vertices, caminho;
  for(int i = 0; i < grafo.size(); i++) vertices.push_back(i);
  while(!vertices.empty()) {
    int v = random(0, vertices.size() - 1);
    caminho.push_back(vertices[v]);
    removeElemento(vertices, vertices[v]);
  }
  return caminho;
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


vector<vector<int>> populacaoInicial() {
  vector<vector<int>> populacao;
  for(int i = 0; i < individuosQtd; i++) {
    populacao.push_back(solucaoAleatoria());
  }
  return populacao;
}

void swap(vector<int>& array, int i, int j) {
  int x =array[i];
 array[i] =array[j];
 array[j] = x;
}

vector<pair<int, double>> fitness(vector<vector<int>> populacao) {
  vector<pair<int, double>> probabilidade;
  double acumulado = 0;
  for(int i = 0; i < populacao.size(); i++) {
    double p = (double)1 / (double)calculaCusto(populacao[i]);
    acumulado += p;
    probabilidade.push_back({i, acumulado});
  }

  return probabilidade;
}

int roleta(vector<pair<int, double>> &fitness) {
  double x = doubleRandom(0, 1);

  for(int i = 0; i < fitness.size(); i++) {
    if(x <= fitness[i].second) {
      int escolhido = fitness[i].first;
      removeElementoPorIndice(fitness, i);
      return escolhido;
    }
  }

  int escolhido = fitness.back().first;
  fitness.pop_back();
  return escolhido;
}

vector<int> crossover(vector<int> &mae, vector<int> &pai) {
  vector<int> filho(mae.size(), -1);
  vector<bool> disp(mae.size(), true);
  int inicio = random(0, mae.size()-2);
  int fim = random(inicio+1, mae.size()-1);

  for(int i = inicio; i <= fim; i++) {
    filho[i] = mae[i];
    disp[mae[i]] = false;
  }

  int j = 0;
  for(int i = 0; i < pai.size() && j < filho.size(); i++) {
    if(j == inicio) j = fim + 1;
    if(disp[pai[i]]) {
      filho[j] = pai[i];
      j++;
    }
  }

  return filho;
}

void mutacao(vector<vector<int>> &populacao) {
  for(int i = 0; i < populacao.size(); i++) {
    double x = doubleRandom(0, 1);
    if(x < 0.4) {
        int v = random(0, populacao[i].size()-1);
        int u = random(0, populacao[i].size()-1);
        swap(populacao[i], u, v);
    }
  }
}

void printPopulacao(vector<vector<int>> &populacao) {
  for(int i = 0; i < populacao.size(); i++) {
    for(int j = 0; j < populacao[i].size(); j++) {
      cout << populacao[i][j] << " ";
    }
    cout << endl;
  }
}

vector<vector<int>> sobreviventes(vector<vector<int>> populacao, int qtd) {
  vector<vector<int>> s;
  vector<pair<int, int>> custos;
  for(int i = 0; i < populacao.size(); i++) {
    custos.push_back({i, calculaCusto(populacao[i])});
  }
  sort(custos.begin(), custos.end(), [](pair<int, int>&a, pair<int, int>&b) {
    return a.second < b.second;
  });
  for(int i = 0; i < qtd; i++) {
    s.push_back(populacao[custos[i].first]);
  } return s;
}

vector<vector<int>> AG() {
  vector<vector<int>> populacao = populacaoInicial();
  int it = 0;
  while(it < 1000) {
    vector<pair<int, double>> fit = fitness(populacao);
    vector<vector<int>> filhos;

    for(int i = 0; i < individuosQtd / 2; i++) {
      int mae = roleta(fit);
      int pai = roleta(fit);

      filhos.push_back(crossover(populacao[mae], populacao[pai]));
      filhos.push_back(crossover(populacao[pai], populacao[mae]));
    }
    mutacao(filhos);
    
    int elitismoQtd = (int)round(individuosQtd * 0.3);
    vector<vector<int>> elites = sobreviventes(populacao, elitismoQtd);
    vector<vector<int>> melhoresFilhos = sobreviventes(filhos, individuosQtd - elites.size());
    
    populacao = elites;
    populacao.insert(populacao.end(), melhoresFilhos.begin(), melhoresFilhos.end());

    it++;
  }
  return populacao;
}

vector<int> melhorSolucao(vector<vector<int>> solucoes) {
  sort(solucoes.begin(), solucoes.end(), [](vector<int> a, vector<int> b) {
    return calculaCusto(a) < calculaCusto(b);
  });
  return solucoes[0];
}

int main(int argc, char* argv[]) {
  lerEntrada();

  auto inicio = high_resolution_clock::now();
  vector<vector<int>> solucoes = AG();
  auto fim = high_resolution_clock::now();
  duration<double> tempo = fim - inicio;
  vector<int> solucao = melhorSolucao(solucoes);
  printSolucao(solucao, tempo);

  return 0;
}