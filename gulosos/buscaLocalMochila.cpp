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

string solucaoInicialLeves() {
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

string solucaoInicialCustoBeneficio() {
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

string buscaLocal(string solucao) {
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
  string aleatoria = solucaoAleatoria();
  string leves = solucaoInicialLeves();
  string custoBeneficio = solucaoInicialCustoBeneficio();

  string solucao = buscaLocal(aleatoria);
  cout << "Solução inicial aleatória: " << endl << solucao << endl;
  cout << "Lucro: " << calculaLucro(solucao) << endl;
  cout << "Capacidade: " << calculaCapacidade(solucao) << endl << endl;

  solucao = buscaLocal(leves);
  cout << "Solução inicial itens mais leves: " << endl << solucao << endl;
  cout << "Lucro: " << calculaLucro(solucao) << endl;
  cout << "Capacidade: " << calculaCapacidade(solucao) << endl << endl;

  solucao = buscaLocal(custoBeneficio);
  cout << "Solução inicial custo benéficio: " << endl << solucao << endl;
  cout << "Lucro: " << calculaLucro(solucao) << endl;
  cout << "Capacidade: " << calculaCapacidade(solucao) << endl;

  return 0;
}
