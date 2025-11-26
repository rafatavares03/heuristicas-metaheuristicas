#include <bits/stdc++.h>
#include <chrono>
#define endl '\n'

using namespace std;
using namespace std::chrono;

int config;
bool outInCSVFormat;
int quantidadeDeItens, capacidade, tamanhoDaPopulacao;
double chanceDeMutacao;
vector<pair<int, int>> itens;

void lerEntrada() {
  cin >> quantidadeDeItens >> capacidade;
  for(int i = 0; i < quantidadeDeItens; i++) {
    int lucro, peso;
    cin >> lucro >> peso;
    itens.push_back({lucro, peso});
  }
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

string solucaoAleatoria() {
  string solucao;
  for(int i = 0; i < quantidadeDeItens; i++) solucao += "0";
  int cap = 0;
  while(cap < capacidade) {
    int bit = (int)random(0, quantidadeDeItens);
    if(solucao[bit] == '1') continue;
    cap += itens[bit].second;
    if(cap < capacidade) solucao[bit] = '1';
  }
  return solucao;
}

int calculaCapacidade(string solucao) {
  int capacidade = 0;
  for(int i = 0; i < solucao.length(); i++) {
    if(solucao[i] == '1') capacidade += itens[i].second;
  }
  return capacidade;
}

int calculaLucro(string solucao) {
  int lucro = 0;
  for(int i = 0; i < solucao.length(); i++) {
    if(solucao[i] == '1') lucro += itens[i].first;
  }
  if(calculaCapacidade(solucao) > capacidade) lucro *= 0.2;
  return lucro;
}

vector<string> populacaoInicial() {
  vector<string> populacao;
  for(int i = 0; i < tamanhoDaPopulacao; i++) {
    populacao.push_back(solucaoAleatoria());
  }
  return populacao;
}

vector<pair<int, double>> fitness(vector<string> populacao) {
  vector<pair<int, double>> fit;
  vector<int> lucros(populacao.size(), 0);
  long long f = 0;

  for(int i = 0; i < populacao.size(); i++) {
    lucros[i] = calculaLucro(populacao[i]);
    f += lucros[i];
  }
  for(int i = 0; i < populacao.size(); i++) {
    double probIndividual = (double) lucros[i] / (double) f; 
    double probAcumulada = (i == 0) ? probIndividual : probIndividual + fit[i-1].second;
    fit.push_back({i, probAcumulada});
  }

  return fit;
}

void removeElemento(vector<pair<int, double>> &array, int indice) {
  if (indice >= 0 && indice < array.size()) {
    array.erase(array.begin() + indice);
  }
}

int roleta(vector<pair<int, double>> &fitness) {
  double x = doubleRandom(0,1);
  int i = 0;
  while(i < fitness.size()) {
    if(x < fitness[i].second) {
      removeElemento(fitness, i);
      return fitness[i].first;
    }
    i++;
  }
  int indice = fitness[0].first;
  removeElemento(fitness, 0);
  return indice;
}

pair<string, string> crossover(string mae, string pai) {
  int point = random(1, mae.length()-1);
  string f1 = mae;
  string f2 = pai;
  for(int i = 0; i < mae.length(); i++) {
    if(doubleRandom(0,1) < 0.5) {
      f1[i] = pai[i];
      f2[i] = mae[i];
    }
  }
  return make_pair(f1, f2);
}

void mutacao(vector<string> &populacao) {
  for(int i = 0; i < populacao.size(); i++) {
    if(doubleRandom(0,1) <= chanceDeMutacao) {
      int gene = random(0, populacao[i].length() - 1);
      populacao[i][gene] = (populacao[i][gene] == '1') ? '0' : '1';
    }
  }
}

vector<string> sobreviventes(vector<string> populacao, int qtd) {
  vector<string> s;
  vector<pair<int, string>> lucros;
  for(int i = 0; i < populacao.size(); i++) {
    lucros.push_back({calculaLucro(populacao[i]), populacao[i]});
  }
  
  sort(lucros.begin(), lucros.end(), [](pair<int, string>&a, pair<int, string>&b) {
    return a.first > b.first;
  });

  for(int i = 0; i < qtd; i++) {
    s.push_back(lucros[i].second);
  }

  return s;
}

vector<string> AG() {
  vector<string> populacao = populacaoInicial();
  int it = 0;
  while(it < 100) {
    vector<pair<int, double>> fit = fitness(populacao);
    vector<string> filhos;

    for(int i = 0; i < tamanhoDaPopulacao / 2; i++) {
      int mae = roleta(fit);
      int pai = roleta(fit);
      pair<string, string> f = crossover(populacao[mae], populacao[pai]);
      filhos.push_back(f.first);
      filhos.push_back(f.second);
    }
    mutacao(filhos);

    int elitismoQtd = (int)round(tamanhoDaPopulacao * 0.3);
    vector<string> elites = sobreviventes(populacao, elitismoQtd);
    vector<string> melhoresFilhos = sobreviventes(filhos, tamanhoDaPopulacao - elites.size());

    populacao = elites;
    populacao.insert(populacao.end(), melhoresFilhos.begin(), melhoresFilhos.end());
    it++;
  }
  return populacao;
}

void printSolucaoCSV(string solucao, duration<double> tempo) {
  cout << solucao << "," << calculaLucro(solucao) << ",";
  cout << fixed << setprecision(3);
  cout << tempo.count() << endl;
}

void printSolucao(string solucao, duration<double> tempo) {
  cout << "Solução: ";
  cout << solucao << endl;
  cout << "Lucro: " << calculaLucro(solucao) << endl;
  cout << "Capacidade: " << calculaCapacidade(solucao) << endl;
  cout << "Tempo de processamento: ";
  cout << fixed << setprecision(3);
  cout << tempo.count() << " segundos." << endl;
}

string solucao(vector<string> populacao) {
  vector<pair<int, string>> lucros;
  for(int i = 0; i < populacao.size(); i++) {
    lucros.push_back({calculaLucro(populacao[i]), populacao[i]});
  }

  sort(lucros.begin(), lucros.end(), [](pair<int, string>&a, pair<int, string>&b) {
    return a.first > b.first;
  });

  for(int i = 0; i < lucros.size(); i++) {
    if(calculaCapacidade(lucros[i].second) <= capacidade) {
      return lucros[i].second;
    }
  }
  return "0";
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

  tamanhoDaPopulacao = (config == 1) ? 50 : 100;
  chanceDeMutacao = (config == 1) ? 0.1 : 0.03;
}

int main(int argc, char* argv[]){
  gerenciarConfiguracoes(argc, argv);
  lerEntrada();

  auto inicio = high_resolution_clock::now();
  vector<string> populacao = AG();
  string melhorSolucao = solucao(populacao);
  auto fim = high_resolution_clock::now();
  duration<double> tempo = fim - inicio;
  if(outInCSVFormat) {
    printSolucaoCSV(melhorSolucao, tempo);
  } else {
    printSolucao(melhorSolucao, tempo);
  }
  return 0;
}