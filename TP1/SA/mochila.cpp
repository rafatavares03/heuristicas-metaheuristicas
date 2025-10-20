#include <bits/stdc++.h>
#include <chrono>
#define endl '\n'
#define INF 0x3f3f3f3f

using namespace std;
using namespace std::chrono;

int config;
bool outInCSVFormat;

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

void printSolucaoCSV(string solucao, duration<double> tempo) {
  cout << solucao << "," << calculaLucro(solucao) << "," << calculaCapacidade(solucao) << ",";
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
  cout << tempo.count() << " segundos."<< endl;
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
  double t = (config == 1) ? 1e6 : 1e8;
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

    t *= (config == 1) ? 0.9 : 0.7;
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
}

int main(int argc, char* argv[]){
  gerenciarConfiguracoes(argc, argv);
  lerEntrada();
  auto inicio = high_resolution_clock::now();
  string solucao = simulatedAnnealing();
  auto fim = high_resolution_clock::now();
  duration<double> tempo = fim - inicio;
  if(outInCSVFormat) {
    printSolucaoCSV(solucao, tempo);
  } else {
    printSolucao(solucao, tempo);
  }
  return 0;
}
