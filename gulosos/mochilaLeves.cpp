#include <bits/stdc++.h>
#define endl '\n'

using namespace std;

int quantidadeDeItens, capacidade;
vector<pair<int, int>> itens;

string solucaoInicial() {
  vector<pair<int, int>> custoBeneficio(itens.size());
  for(int i = 0; i < itens.size(); i++) {
    custoBeneficio[i] = {i, itens[i].second};
  }
  sort(custoBeneficio.begin(), custoBeneficio.end(), [](pair<int, int>& a, pair<int, int>& b) {
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

int main(){
  cin >> quantidadeDeItens >> capacidade;
  for(int i = 0; i < quantidadeDeItens; i++) {
    int lucro, peso;
    cin >> lucro >> peso;
    itens.push_back({lucro, peso});
  }
  cout << "Solucão inicial:" << endl;
  cout << solucaoInicial() << endl;

  return 0;
}