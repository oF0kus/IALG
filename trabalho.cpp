/*

    COISAS QUE TEM Q TERMINAR


    Salvar alterações ta dando erro e tem que salvar em binario
    funcao do binario

    colocar 
    if (cin.fail())
    transform()
*/


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
    bool deletado;
};

void RedimensionarVetor(MBC *&vetor, int *capacidade) {
    int novaCapacidade = (*capacidade) + 5;
    MBC *novoVetor = new MBC[novaCapacidade];

    for (int i = 0; i < *capacidade; i++) {
        novoVetor[i] = vetor[i];
    }

    delete[] vetor; 
    vetor = novoVetor;
    (*capacidade) = novaCapacidade;  
}

void imprime(MBC filme[], int indice) {
        if (filme[indice].deletado) {
        return;
    }

    cout << "Ranking: " << filme[indice].ranking << endl
         << "Ano de Lançamento: " << filme[indice].lancamento << endl
         << "Nome: " << filme[indice].nome << endl
         << "Diretor: " << filme[indice].diretor << endl
         << "Bilheteria: " << fixed << setprecision(0) << filme[indice].bilheteria << endl
         << "------------------------" << endl;
    }

int BuscaBinariaRecursivaPorRanking(MBC *filmes, int inicio, int fim, int rankingProcurado) {
    if (inicio > fim) {
        return -1;
    }
    int meio = inicio + (fim - inicio) / 2;

    if (filmes[meio].ranking == rankingProcurado && !filmes[meio].deletado) {
        return meio;
    }
    if (filmes[meio].ranking < rankingProcurado) {
        return BuscaBinariaRecursivaPorRanking(filmes, meio + 1, fim, rankingProcurado);
    } else {
        return BuscaBinariaRecursivaPorRanking(filmes, inicio, meio - 1, rankingProcurado);
    }
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
void imprimirPorIntervalo(MBC filme[], int indice) {
    int inicio, fim;

    cout << "Digite o ranking inicial: ";
    cin >> inicio;
    cout << "Digite o ranking final: ";
    cin >> fim;

    if (inicio > fim || inicio < 1 || fim > indice) {
        cout << "Intervalo inválido. Certifique-se de que o ranking inicial é menor ou igual ao final e está dentro do intervalo disponível (1 a " << indice << ")." << endl;
        return;
    }
    cout << "\nFilmes no intervalo de ranking [" << inicio << " - " << fim << "]:" << endl;
    for (int i = 0; i < indice; i++) {
        if (filme[i].ranking >= inicio && filme[i].ranking <= fim) {
            imprime(filme, i);
        }
    }
}

void DeletarFilme(MBC *&filmes, int &indice) {
    int rankingProcurado;
    cout << "Digite o ranking do filme que deseja deletar: ";
    cin >> rankingProcurado;

    // Busca binária para encontrar o índice do filme
    int posicao = BuscaBinariaRecursivaPorRanking(filmes, 0, indice - 1, rankingProcurado);
    if (posicao != -1) {
        filmes[posicao].deletado = true;  // Marca o filme como deletado
        cout << "Filme com ranking " << rankingProcurado << " foi marcado como deletado.\n" << endl;
    } else {
        cout << "Filme com ranking " << rankingProcurado << " não encontrado.\n" << endl;
    }
}

void salvarArquivo(MBC *&filme, int indice) {
    ofstream saida("MaioresBilheteriasCinema.csv", ios::out | ios::trunc); 

    if (saida) {

        saida << "Ranking ,Ano de Lançamento,Nome do filme,Diretor,Bilheteria Total" << endl;

        for (int i = 0; i < indice; i++) { 
            if (!filme[i].deletado) {
                saida << filme[i].ranking << ","
                      << filme[i].lancamento << ","
                      << filme[i].nome << ","
                      << filme[i].diretor << ","
                      << fixed << setprecision(0) << filme[i].bilheteria << endl;
            }
        } 
        cout << "Novos filmes adicionados ao arquivo com sucesso!" << endl;
    } else {
        cout << "Erro ao abrir o arquivo para salvar." << endl;
    }
}

void buscaTitulo(string& busca, const int indice, MBC filme[]) {
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

void ordenarFilmes(MBC filme[], int indice) {
    // Ordena os filmes por bilheteria (ordem decrescente)
    for (int i = 0; i < indice - 1; i++) {
        for (int j = 0; j < indice - i - 1; j++) {
            if (filme[j].bilheteria < filme[j + 1].bilheteria) {
                MBC temp = filme[j];
                filme[j] = filme[j + 1];
                filme[j + 1] = temp;
            }
        }
    }
    // Aqui vai atualizar o rankings após a ordenação
    for (int i = 0; i < indice; i++) {
        filme[i].ranking = i + 1;
    }
}

void registrarNovo(MBC *&filme, int& indice, int *capacidade) {
    if (indice == *capacidade) {
        RedimensionarVetor(filme, capacidade);
    }

    cout << "Digite o ano de lançamento: ";
    cin >> filme[indice].lancamento;
    cin.ignore();

    cout << "Digite o nome do filme: ";
    getline(cin, filme[indice].nome);

    cout << "Digite o nome do diretor: ";
    getline(cin, filme[indice].diretor);

    cout << "Digite a bilheteria : ";
    cin >> filme[indice].bilheteria;
    cin.ignore();

    cout << "Filme registrado com sucesso!" << endl;
    indice++;
    ordenarFilmes(filme, indice);
}

void menu(MBC filme[], int& indice, int *capacidade) {
	int inicioNovos = indice; // Marca a posição inicial dos novos filmes
    int opcao = 999;
    do {
        cout<<"Escolha uma opção:"<<endl;
        cout<<endl;
        cout<<"1. Listar Filmes"<<endl;
        cout<<"2. Cadastrar novo Filme"<<endl;
        cout<<"3. Buscar Filme"<<endl;
        cout<<"4. Deletar Filme"<<endl;
        cout<<"5. Imprimir Filmes por Intervalo"<<endl;
        cout<<"6. Salvar alterações"<<endl;
        cout<<"0. Sair"<<endl;
        cout << endl;
        cout<<"Digite sua opção: ";
        cin >> opcao;

        switch (opcao) {
            case 1: {
                for (int i = 0; i < indice; i++) {
                    imprime(filme, i);
                }
            } break;
            case 2: {
                registrarNovo(filme, indice, capacidade);
            } break;
            case 3: {
                string busca;
                cin.ignore();
                buscaTitulo(busca, indice, filme);
            } break;
            case 4: {
                DeletarFilme(filme, indice);
            } break;
            case 5: {
                imprimirPorIntervalo(filme, indice);
            } break;            
            case 6: {
				salvarArquivo(filme, indice);
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
    int indice = 0, capacidade = 100;
    MBC *filme = new MBC[capacidade];

    lerArquivo(filme, indice);
    menu(filme, indice, &capacidade);

    delete[] filme;
    return 0;
}
