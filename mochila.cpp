#include <bits/stdc++.h>
#define endl '\n'

using namespace std;

void binarios(string bin, int tamanho) {
    if(bin.length() >= tamanho) {
        cout << bin << endl;
        return;
    }
    binarios(bin + "0", tamanho);
    binarios(bin + "1", tamanho);
}

int main() {
    int tamanho;
    cin >> tamanho;
    binarios("", tamanho);
    return 0;
}
