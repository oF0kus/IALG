#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

struct MBC {
    int ranking;
    int lançamento;
    string nome;
    string diretor;
    float bilheteria;

    void imprime() {
        cout << "Ranking: " << ranking << endl
             << "Ano de Lançamento: " << lançamento << endl
             << "Nome: " << nome << endl
             << "Diretor: " << diretor << endl
             << "Bilheteria: " << fixed << setprecision(0) << bilheteria << endl
             << "------------------------" << endl;
    }
};

int main() {
    ifstream entrada("MaioresBilheteriasCinema.csv");

    if (entrada) {
        string descArq;
        char virgula;
        getline(entrada, descArq); 

        MBC* vetorMBC = new MBC[100];
        MBC algumMBC;

        for (int i = 0; i < 100; i++) {
            
            entrada >> algumMBC.ranking >> virgula;
            entrada >> algumMBC.lançamento >> virgula;
            getline(entrada, algumMBC.nome, ',');
            getline(entrada, algumMBC.diretor, ',');
            entrada >> algumMBC.bilheteria;
            vetorMBC[i] = algumMBC;
            vetorMBC[i].imprime();
        }

        delete[] vetorMBC;
    } else {
        cout << "Erro na leitura do arquivo" << endl;
    }

    return 0;
}
