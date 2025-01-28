/*

    COISAS QUE TEM Q TERMINAR
    Salvar alterações ta dando erro, que odioooooooooo.... e tem que salvar em binario
    funcao do binario
    parou de encontrar o filme

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
            if (i >= 100) break;  // menos mil do joaquim
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


void salvarArquivo(MBC *&filme, int indice, int inicioNovos) {
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
void registro(MBC *&filme, int& indice, int *capacidade) {
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

    cout << "Digite a bilheteria: ";
    cin >> filme[indice].bilheteria;

    cout << "Filme registrado com sucesso!" << endl;

    indice++;
    ordenarFilmes(filme, indice); // Ordena e atualiza os rankings após adicionar o filme
    
}
void imprimirPorIntervalo(MBC filme[], int indice) {
    int inicio, fim;

    cout << "Digite o ranking inicial: ";
    cin >> inicio;
    cout << "Digite o ranking final: ";
    cin >> fim;

    if (inicio > fim || inicio <= 0 || fim > indice) {
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


void deletarFilme(MBC *&filme, int &indice) {
    int rankingDeletar;

    cout << "Digite o ranking do filme que deseja deletar: ";
    cin >> rankingDeletar;
    // Verifica se o ranking existe
    int posicao = -1;
    for (int i = 0; i < indice; i++) {
        if (filme[i].ranking == rankingDeletar) {
            posicao = i;
            i = indice; 
        }
    }
    if (posicao == -1) {
        cout << "Filme com o ranking " << rankingDeletar << " não encontrado." << endl;
        return;
    }

    for (int i = posicao; i < indice - 1; i++) {
        filme[i] = filme[i + 1];
    }

    indice--; // Reduz o tamanho do vetor
    cout << "Filme deletado com sucesso!" << endl;

    // Atualiza os rankings após a remoção
    for (int i = 0; i < indice; i++) {
        filme[i].ranking = i + 1;
    }
}

void menu(MBC filme[], int& indice, int *capacidade) {
	int inicioNovos = indice; // Marca a posição inicial dos novos filmes
    int opcao = 999;
    do {
        cout<<"Escolha uma opção:"<<endl;
        cout<<"\t1. Listar Filmes"<<endl;
        cout<<"\t2. Cadastrar novo Filme"<<endl;
        cout<<"\t3. Buscar Filme"<<endl;
        cout<<"\t4. Deletar Filme"<<endl;
        cout<<"\t5. Imprimir Filmes por Intervalo"<<endl;
        cout<<"\t6. Salvar alterações"<<endl;
        cout<<"\t0. Sair"<<endl;
        cout<<"Digite sua opção: ";
        cin >> opcao;
		cout<<"------------------------"<<endl;
		
        switch (opcao) {
            case 1: {
                for (int i = 0; i < indice; i++) {
                    imprime(filme, i);
                }
            } break;
            case 2: {
                registro(filme, indice, capacidade);
             } break;
            case 3: {
                string busca;
                cin.ignore();
                buscaR(busca, indice, filme);
            } break;
            case 4: { 
				deletarFilme(filme, indice);
			} break;
            case 5: { 
				imprimirPorIntervalo(filme, indice);
			} break;
            case 6: {
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
    int indice = 0, capacidade = 100;
    MBC *filme = new MBC[capacidade];

    lerArquivo(filme, indice);
    menu(filme, indice, &capacidade);

    delete[] filme;
    return 0;
}
