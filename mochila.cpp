#include <bits/stdc++.h>
#define endl '\n'

using namespace std;

void binarios(string bin, int pos) {
    if(pos < 0) return;
    string aux = bin;
    cout << bin << endl;
    char inverso = (aux[pos] == '0') ? '1' : '0';
    aux[pos] = inverso;
    binarios(aux, pos-1);
}

int main() {
    int quantidadeDeItens;
    cin >> quantidadeDeItens;
    string zero = "";
    string um = "";
    for(int i = 0; i < quantidadeDeItens; i++) {
        zero += '0';
        um += '1';
    }
    binarios(zero, quantidadeDeItens-1);
    binarios(um, quantidadeDeItens-1);
    return 0;
}
