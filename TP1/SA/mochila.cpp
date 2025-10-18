#include <bits/stdc++.h>
#define endl '\n'
#define INF 0x3f3f3f3f

using namespace std;

int quantidadeDeItens, capacidade;
vector<pair<int, int>> itens;

void lerEntrada() {
  cin >> quantidadeDeItens >> capacidade;
  for(int i = 0; i < quantidadeDeItens; i++) {
    int lucro, peso;
    cin >> lucro >> peso;
    itens.push_back({lucro, peso});
  }
}

int calculaLucro(string solucao) {
  int lucro = 0;
  for(int i = 0; i < solucao.length(); i++) {
    if(solucao[i] == '1') lucro += itens[i].first;
  }
  return lucro;
}

int calculaCapacidade(string solucao) {
  int capacidade = 0;
  for(int i = 0; i < solucao.length(); i++) {
    if(solucao[i] == '1') capacidade += itens[i].second;
  }
  return capacidade;
}

void printSolucao(string solucao) {
  cout << "Solução: " << solucao << endl;
  cout << "Lucro: " << calculaLucro(solucao) << endl;
  cout << "Capacidade: " << calculaCapacidade(solucao) << endl;
}

double random(double lowerbound, double upperbound) {
  mt19937 eng(std::chrono::high_resolution_clock::now().time_since_epoch().count());
  uniform_real_distribution<double> distrib(lowerbound, upperbound);

  double numero = distrib(eng);
  return numero;
}

string solucaoInicial() {
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

string simulatedAnnealing() {
  string solucao = solucaoInicial();
  string melhorSolucao = solucao;
  int lucro = calculaLucro(solucao);
  int melhorLucro = lucro;
  double t = 1e6;
  while(t > 0.01) {
    int i = 0;
    while(i < 30) {
      vector<int> vizinhosViaveis;
      for(int i = 0; i < solucao.size(); i++) {
        string s = solucao;
        s[i] = (s[i] == '1') ? '0' : '1';
        if(calculaCapacidade(s) <= capacidade) {
          vizinhosViaveis.push_back(i);
        }
      }
      string aux = solucao;
      int bit = (int)random(0, vizinhosViaveis.size());
      aux[vizinhosViaveis[bit]] = (aux[vizinhosViaveis[bit]] == '1') ? '0' : '1';
      int auxLucro = calculaLucro(aux);
      if(auxLucro > lucro) {
        solucao = aux;
        lucro = auxLucro;
        if(lucro > melhorLucro) {
          melhorSolucao = solucao;
          melhorLucro = lucro;
        }
      } else if(random(0, 1) < exp((auxLucro - lucro)/t)){
        solucao = aux;
        lucro = auxLucro;
      } 
      i++;
    }

    t *= 0.8;
  }
  return melhorSolucao;
}

int main() {
  lerEntrada();
  string solucao = simulatedAnnealing();
  printSolucao(solucao);
  return 0;
}
