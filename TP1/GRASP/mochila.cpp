#include <bits/stdc++.h>
#include <chrono>
#define endl '\n'
#define INF 0x3f3f3f3f

using namespace std;
using namespace std::chrono;

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
  cout << tempo.count() << endl;
}

int random(int lowerbound, int upperbound) {
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> distrib(lowerbound, upperbound);

  int numero = distrib(gen);
  return numero;
}

void removeElemento(vector<int> &array, int elemento) {
  auto fim = remove(array.begin(), array.end(), elemento);
  array.erase(fim, array.end());
}

string construcaoGulosaAleatoria(double alpha) {
  string solucao = "";
  vector<int> candidatos;
  for(int i = 0; i < itens.size(); i++) {
    solucao += "0";
    candidatos.push_back(i);
  }
  sort(candidatos.begin(), candidatos.end(), [](int &a, int &b){
    return itens[a].second < itens[b].second;
  });
  int candidatosQtd = max(1, int(alpha * candidatos.size()));
  vector<int> lrc;
  for(int i = 0; i < candidatosQtd; i++) lrc.push_back(candidatos[i]);
  int c = 0;
  while(!lrc.empty()) {
    int item = random(0, lrc.size() - 1);
    c += itens[lrc[item]].second;
    if(c > capacidade) break;
    solucao[lrc[item]] = '1';
    removeElemento(candidatos, lrc[item]);
    removeElemento(lrc, lrc[item]);
  }
  c = calculaCapacidade(solucao);
  for(int i = 0; i < candidatos.size(); i++) {
    if(c + itens[candidatos[i]].second <= capacidade) {
      solucao[candidatos[i]] = '1';
      c += itens[candidatos[i]].second;
    }
  }
  return solucao;
}

string buscaLocal(string solucao) {
  int lucro = calculaLucro(solucao);
  int it = 0, semMelhoria = 0;
  while(it < 1e6 && semMelhoria < 100) {
    string solucaoAtual = solucao;
    int lucroAtual = lucro;
    for(int i = 0; i < solucao.length(); i++) {
      string aux = solucao;
      aux[i] = (solucao[i] == '1') ? '0' : '1';
      if(calculaCapacidade(aux) > capacidade) continue;
      int lucroAux = calculaLucro(aux);
      if(lucroAux > lucroAtual) {
        solucaoAtual = aux;
        lucroAtual = lucroAux;
      }
    }
    if(solucaoAtual == solucao) {
      semMelhoria++;
    } else {
      solucao = solucaoAtual;
      lucro = lucroAtual;
      semMelhoria = 0;
    }
    it++;
  }
  return solucao;
}

string GRASP() {
  string melhorSolucao;
  int it = 0;
  while(it < 100) {
    string solucao = construcaoGulosaAleatoria(0.9);
    solucao = buscaLocal(solucao);
    if(calculaLucro(solucao) > calculaLucro(melhorSolucao)) {
      melhorSolucao = solucao;
    }
    it++;
  }
  return melhorSolucao;
}

int main(){
  lerEntrada();
  auto inicio = high_resolution_clock::now();
  string solucao = GRASP();
  auto fim = high_resolution_clock::now();
  duration<double> tempo = fim - inicio;
  printSolucaoCSV(solucao, tempo);
  return 0;
}