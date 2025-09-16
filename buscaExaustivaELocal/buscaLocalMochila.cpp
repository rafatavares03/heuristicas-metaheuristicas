#include <bits/stdc++.h>
#define endl '\n'

using namespace std;

int quantidadeDeItens, capacidade;
vector<pair<int, int>> itens;

string solucaoAleatoria() {
  srand(time(0));
  string str = "";
  int peso = 0;
  for(int i = 0; i < quantidadeDeItens; i++) {
    int sorteio = rand() % 1000;
    string novoItem = (sorteio > 499) ? "1" : "0";
    if(novoItem == "1" && peso + itens[str.length()].second <= capacidade) {
      peso += itens[str.length()].second;
      str += novoItem;
    } else {
      str += "0";
    }
  }
  return str;
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

string buscaLocal() {
  string solucao = solucaoAleatoria();
  int lucro = calculaLucro(solucao);
  bool earlyStop = false;
  int semMelhoria = 0;
  int it = 0;
  while(it < 1e6 && !earlyStop) {
    string melhorSolucao = solucao;
    int melhorLucro = lucro;
    for(int i = 0; i < solucao.length(); i++) {
      string solucaoAux = melhorSolucao;
      solucaoAux[i] = (solucao[i] == '1') ? '0' : '1';
      if(calculaCapacidade(solucaoAux) <= capacidade) {
        int lucroAux = calculaLucro(solucaoAux);
        if(lucroAux > melhorLucro) {
          melhorSolucao = solucaoAux;
          melhorLucro = lucroAux;
          semMelhoria = 0;
        }
      }
    }
    if(melhorSolucao == solucao) {
      semMelhoria++;
      if(semMelhoria >= 10) earlyStop = true;
    } else {
      solucao = melhorSolucao;
      lucro = melhorLucro;
    }
    it++;
  }
  return solucao;
}

int main() {
  cin >> quantidadeDeItens >> capacidade;
  for(int i = 0; i < quantidadeDeItens; i++) {
    int lucro, peso;
    cin >> lucro >> peso;
    itens.push_back({lucro, peso});
  }
  string solucao = buscaLocal();
  cout << "Solução: " << solucao << endl;
  cout << "Lucro: " << calculaLucro(solucao) << endl;
  cout << "Capacidade: " << calculaCapacidade(solucao) << endl;
  return 0;
}
