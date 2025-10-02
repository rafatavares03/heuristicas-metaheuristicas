#include <bits/stdc++.h>
#define endl '\n'

using namespace std;

int quantidadeDeItens, capacidade;
vector<pair<int, int>> itens;

string solucaoInicial() {
  vector<pair<int,double>> custoBeneficio(itens.size());
  for(int i = 0; i < itens.size(); i++) {
    custoBeneficio[i] = {i, (double)itens[i].second / itens[i].first};
  }
  sort(custoBeneficio.begin(), custoBeneficio.end(), [](pair<int, double>& a, pair<int, double>& b) {
    return a.second < b.second;
  });

  int espaco = 0;
  string solucao = "";
  for(int i = 0; i < itens.size(); i++) solucao += "0";
  for(int i = 0; i < itens.size(); i++) {
    if(itens[custoBeneficio[i].first].second + espaco <= capacidade) {
      solucao[custoBeneficio[i].first] = '1';
      espaco += itens[custoBeneficio[i].first].second;
    }
  }

  return solucao;
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

string buscaTabu() {
    string solucao = solucaoInicial();
    string melhorSolucao = solucao;
    int lucro = calculaLucro(solucao);
    int melhorLucro = lucro;
    int it = 0; // condicao de parada: iteracões sem melhora;
    int p = 0;
    for(int i = 0; i < solucao.length(); i++) {
        p += itens[i].second;
    }
    
    vector<int> t(solucao.length(), 0);
    while(it < 100) {
        vector<pair<int, int>> vizinhos(solucao.length());
        for(int i = 0; i < solucao.length(); i++) {
            string s = solucao;
            s[i] = (s[i] == '1') ? '0' : '1';
            int l = calculaLucro(s);
            int c = calculaCapacidade(s);


        }
    }

    return melhorSolucao;
}

int main(){
  cin >> quantidadeDeItens >> capacidade;
  for(int i = 0; i < quantidadeDeItens; i++) {
    int lucro, peso;
    cin >> lucro >> peso;
    itens.push_back({lucro, peso});
  }
  cout << "Solução inicial:" << endl;
  cout << solucaoInicial() << endl;

  return 0;
}