#include <bits/stdc++.h>
#define endl '\n'

using namespace std;

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

void printSolucao(string solucao) {
  cout << "Solução: ";
  cout << solucao << endl;
  cout << "Lucro: " << calculaLucro(solucao) << endl;
}

vector<pair<int, int>> geraVizinhancaOrdenada(string solucao) {
  vector<pair<int, int>> vizinhos;
  for(int i = 0; i < solucao.length(); i++) {
    string s = solucao;
    s[i] = (s[i] == '1') ? '0' : '1';
    int l = calculaLucro(s);
    vizinhos.push_back({i, l});
  }

 sort(vizinhos.begin(), vizinhos.end(), [](pair<int, int> &a, pair<int, int> &b){
    return a.second > b.second;
  });

  return vizinhos;
}

int aspiracaoPorDefault(vector<pair<int, int>> vizinhos,vector<int> t, int duracaoTabu) {
  int k = 0;
  int menorTabu = duracaoTabu;
  int vizinho = -1;
  while(vizinhos[k].second > 0 && k < vizinhos.size()) {
    int bit = vizinhos[k].first;
    int lucroBit = vizinhos[k].second;
    if(t[bit] < menorTabu) {
      menorTabu = t[bit];
      vizinho = k;
    }
    k++;
  }
  return vizinho;
}

string buscaTabu() {
  string solucao = solucaoInicial();
  string melhorSolucao = solucao;
  int lucro = calculaLucro(solucao);
  int melhorLucro = lucro;
  int it = 0; // condicao de parada: iteracões sem melhora;
  int duracaoTabu = max(1, (int)(0.1 * solucao.length()));
  vector<int> t(solucao.length(), 0);

  while(it < 100) {
    vector<pair<int, int>> vizinhos = geraVizinhancaOrdenada(solucao);
    int melhorVizinho = -1;
    int vizinhoLucro = -1;
    int j = 0;

    while(vizinhoLucro < 0 && j < vizinhos.size()) {
      int bit = vizinhos[j].first;
      int lucroBit = vizinhos[j].second;
      if(t[bit] > 0) {
        if(lucroBit > melhorLucro) { // aspiração regional
          melhorVizinho = bit;
          vizinhoLucro = lucroBit;
        }
      } else if(lucroBit > vizinhoLucro) {
        melhorVizinho = bit;
        vizinhoLucro = lucroBit;
      }
      j++;
    }

    if(melhorVizinho < 0) {
      int indice = aspiracaoPorDefault(vizinhos, t, duracaoTabu);
      melhorVizinho = vizinhos[indice].first;
      vizinhoLucro = vizinhos[indice].second;
    }

    for(int i = 0; i < t.size(); i++) {
      if(t[i] > 0) t[i]--;
    }
    t[melhorVizinho] = duracaoTabu;

    solucao[melhorVizinho] = (solucao[melhorVizinho] == '1') ? '0' : '1';
    lucro = vizinhoLucro;
    if(lucro > melhorLucro) {
      melhorSolucao = solucao;
      melhorLucro = lucro;
      it = 0;
    } else {
      it++;
    }
  }

  return melhorSolucao;
}

int main(){
  lerEntrada();
  string solucao = buscaTabu();
  printSolucao(solucao);
  return 0;
}
