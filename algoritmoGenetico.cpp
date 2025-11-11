#include <bits/stdc++.h>
#include <chrono>
#define endl '\n'

using namespace std;
using namespace std::chrono;

int quantidadeDeItens, capacidade, p;
vector<pair<int, int>> itens;

void lerEntrada() {
  cin >> quantidadeDeItens >> capacidade;
  for(int i = 0; i < quantidadeDeItens; i++) {
    int lucro, peso;
    cin >> lucro >> peso;
    itens.push_back({lucro, peso});
  }
  for(int i = 0; i < quantidadeDeItens; i++) {
    p += itens[i].first;
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
  int solucaoCap = calculaCapacidade(solucao);
  for(int i = 0; i < solucao.length(); i++) {
    if(solucao[i] == '1') lucro += itens[i].first;
  }
  lucro = lucro - (p * max(0, solucaoCap - capacidade));
  return lucro;
}

vector<string> populacaoInicial() {
  vector<string> populacao;
  for(int i = 0; i < 50; i++) {
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

pair<string, string> roleta(vector<pair<int, double>> &fitness) {
  double x = doubleRandom(0,1);
  vector<string> pais(2);
}

vector<string> AG() {
  vector<string> populacao = populacaoInicial();
  int it = 0;
  while(it < 1) {
    vector<pair<int, double>> fit = fitness(populacao);

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


int main(int argc, char* argv[]){
  lerEntrada();

  auto inicio = high_resolution_clock::now();
  vector<string> populacao = AG();
  for(int i = 0; i < populacao.size(); i++) {
    cout << populacao[i] << endl;
  }
  auto fim = high_resolution_clock::now();
  duration<double> tempo = fim - inicio;

  return 0;
}