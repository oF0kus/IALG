#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

struct MBC {
    int ranking;
    int lancamento;
    string nome;
    string diretor;
    float bilheteria;

};

void imprime(MBC filme[], int indice) {
    cout << "Ranking: " << filme[indice].ranking << endl
         << "Ano de Lançamento: " << filme[indice].lancamento << endl
         << "Nome: " << filme[indice].nome << endl
         << "Diretor: " << filme[indice].diretor << endl
         << "Bilheteria: " << fixed << setprecision(0) << filme[indice].bilheteria << endl
         << "------------------------" << endl;
    }

void lerArquivo(MBC filme[], int& indice) {
    ifstream entrada("MaioresBilheteriasCinema.csv");

    if (entrada) {
        string descArq;
        char virgula;
        getline(entrada, descArq); 

        MBC* vetorMBC = new MBC[100];
        int i = 0;

          while (entrada >> vetorMBC[i].ranking >> virgula
               >> vetorMBC[i].lancamento >> virgula
               && getline(entrada, vetorMBC[i].nome, ',')
               && getline(entrada, vetorMBC[i].diretor, ',')
               && entrada >> vetorMBC[i].bilheteria) {
            i++;
            if (i >= 100) break;  
        }
        indice = i;
        for (int j = 0; j < indice; j++) {
            filme[j] = vetorMBC[j];
        }
        delete[] vetorMBC;
    } else {
        cout << "Erro na leitura do arquivo" << endl;
    }


}
void buscaR(string& busca, const int indice, MBC filme[]) {
    cout << "Titulo do filme da busca: ";
    cin.ignore();
    getline(cin, busca);

    int i = 0, posicao = -1;
    while (i < indice and busca != filme[i].nome) {
        i++;
    }

    if (i < indice and busca == filme[i].nome) {
        posicao = i;
    }

    if (posicao == -1) {
        cout << "O filme não esta disponivel" << endl;
    } else {
        imprime(filme, posicao);
    }
}
void registro(MBC filme[], int& indice) {
    if (indice >= 100) {
        cout << "Capacidade máxima atingida, não é possível adicionar mais filmes." << endl;
        return;
    }

    cout << "Digite o ranking do filme: ";
    cin >> filme[indice].ranking;
    cin.ignore(); 

    cout << "Digite o ano de lançamento: ";
    cin >> filme[indice].lancamento;
    cin.ignore();

    cout << "Digite o nome do filme: ";
    getline(cin, filme[indice].nome);

    cout << "Digite o nome do diretor: ";
    getline(cin, filme[indice].diretor);

    cout << "Digite a bilheteria (em milhões): ";
    cin >> filme[indice].bilheteria;

    cout << "Filme registrado com sucesso!" << endl;

    indice++;
}

void salvarArquivo(MBC filme[], int indice, int inicioNovos) {
    ofstream saida("MaioresBilheteriasCinema.csv", ios::app); 

    if (saida) {
        for (int i = inicioNovos; i < indice; i++) { 
            saida << filme[i].ranking << ","
                  << filme[i].lancamento << ","
                  << filme[i].nome << ","
                  << filme[i].diretor << ","
                  << fixed << setprecision(0) << filme[i].bilheteria << endl;
        }
        cout << "Novos filmes adicionados ao arquivo com sucesso!" << endl;
    } else {
        cout << "Erro ao abrir o arquivo para salvar." << endl;
    }
}
void menu(MBC filme[], int& indice) {
	int inicioNovos = indice; // Marca a posição inicial dos novos filmes
    int opcao = 999;
    do {
        cout<<"Escolha uma opção:"<<endl;
        cout<<"\t1. Listar Filmes"<<endl;
        cout<<"\t2. Cadastrar novo Filme"<<endl;
        cout<<"\t3. Buscar Filme"<<endl;
        cout<<"\t4. Deletar Filme"<<endl;
        cout<<"\t5. Alterar Dados de um Filme"<<endl;
        cout<<"\t6. Imprimir Filmes por Intervalo"<<endl;
        cout<<"\t7. Salvar alterações"<<endl;
        cout<<"\t0. Sair"<<endl;
        cin >> opcao;

        switch (opcao) {
            case 1: {
                for (int i = 0; i < indice; i++) {
                    imprime(filme, i);
                }
            } break;
            case 2: {
                cout << "Quantos filmes deseja registrar?: ";
                cin >> indice;
				registro(filme, indice);
              
            } break;
            case 3: {
                string busca;
                buscaR(busca, indice, filme);
            } break;
            case 7: {
				salvarArquivo(filme, indice, inicioNovos);
				inicioNovos = indice; // Atualiza o início dos novos registros
			} break;

            case 0: {
                cout << "Saindo..." << endl;
            } break;
            default:
                cout << "Opcao invalida" << endl;
                break;
        }
    } while (opcao != 0);
}


int main(){
    int indice = 0; // Capacidade inicial de filmes
    MBC filme[100]; // Capacidade máxima de filme

    lerArquivo(filme, indice);
    
    menu(filme, indice);

    return 0;
}
