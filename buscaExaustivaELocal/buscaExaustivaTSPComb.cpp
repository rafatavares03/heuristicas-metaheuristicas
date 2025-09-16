#include <bits/stdc++.h>
#define endl '\n'

using namespace std;

void caminhos(vector<int> caminho, vector<int> disponiveis) {
  if(disponiveis.empty()) {
    for(int i = 0; i < caminho.size(); i++) cout << caminho[i] << " ";
    cout << caminho[0] << endl;
    return;
  }
  for(int i = 0; i < disponiveis.size(); i++) {
    vector<int> caminhoAux(caminho), dispAux(disponiveis);
    caminhoAux.push_back(disponiveis[i]);
    auto fim = remove(dispAux.begin(), dispAux.end(), disponiveis[i]);
    dispAux.erase(fim, dispAux.end());
    caminhos(caminhoAux, dispAux);
  }
}

int main() {
  int n;
  cin >> n;
  vector<int> array, disponiveis;
  for(int i = 1; i <= n; i++) disponiveis.push_back(i);
  clock_t inicio = clock();
  caminhos(array, disponiveis);
  clock_t fim = clock();
  double duracao = static_cast<double>(fim - inicio) / CLOCKS_PER_SEC;
  cout << "Tempo de execução: " << duracao << " segundos." << endl;
  return 0;
}