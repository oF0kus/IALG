/*

    COISAS QUE TEM Q TERMINAR

    alterar dados
    funcao do binario

    colocar 
    if (cin.fail())
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
    double bilheteria;
    bool deletado;
};

void RedimensionarVetor(MBC *&vetor, int *capacidade) {
    int novaCapacidade = (*capacidade) + 10;
    MBC *novoVetor = new MBC[novaCapacidade];

    for (int i = 0; i < *capacidade; i++) {
        novoVetor[i] = vetor[i];
    }

    delete[] vetor; 
    vetor = novoVetor;
    (*capacidade) = novaCapacidade;  
}

void imprime(MBC filme[], int indice) {
    if (filme[indice].deletado) return;

    double bilheteria = filme[indice].bilheteria;

    // Separa parte inteira e decimal
    long long parteInteira = static_cast<long long>(bilheteria);
    int centavos = static_cast<int>((bilheteria - parteInteira) * 100 + 0.5); // Arredonda para 2 dígitos

    // Formata a parte inteira com pontos
    string strInteira = to_string(parteInteira);
    int tamanho = strInteira.size();
    for (int i = tamanho - 3; i > 0; i -= 3) {
        strInteira.insert(i, ".");
    }

    // Formata os centavos (sempre 2 dígitos)
    stringstream ss;
    ss << setw(2) << setfill('0') << abs(centavos); // Garante 2 dígitos, mesmo se centavos < 10
    string strDecimal = ss.str();

    cout << "Ranking: " << filme[indice].ranking << endl
         << "Ano de Lançamento: " << filme[indice].lancamento << endl
         << "Nome: " << filme[indice].nome << endl
         << "Diretor: " << filme[indice].diretor << endl
         << "Bilheteria: R$ " << strInteira << "," << strDecimal << endl
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
int BuscaBinariaRecursivaPorTitulo(MBC *filmes, int inicio, int fim, const string &tituloProcurado) {
    if (inicio > fim) {
        return -1;
    }
    int meio = inicio + (fim - inicio) / 2;
    

    if (filmes[meio].nome == tituloProcurado && !filmes[meio].deletado) {
        return meio;
    }
    if (filmes[meio].nome < tituloProcurado) {
        return BuscaBinariaRecursivaPorTitulo(filmes, meio + 1, fim, tituloProcurado);
    } else {
        return BuscaBinariaRecursivaPorTitulo(filmes, inicio, meio - 1, tituloProcurado);
    }
}

void lerArquivo(MBC *&filme, int& indice, int *capacidade) {
    ifstream entrada("MaioresBilheteriasCinema.csv");

    if (entrada) {
        string descArq;
        char virgula;
        getline(entrada, descArq);  

        int i = 0;

        while (entrada >> filme[i].ranking >> virgula
               >> filme[i].lancamento >> virgula
               && getline(entrada, filme[i].nome, ',')
               && getline(entrada, filme[i].diretor, ',')
               && entrada >> filme[i].bilheteria) {
            
            filme[i].deletado = false; 
            i++;

            if (i == *capacidade) {  
                RedimensionarVetor(filme, capacidade);
            }
        }
        indice = i;
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

void ordenarPorTitulo(MBC *filmes, int indice) {
    for (int i = 0; i < indice - 1; i++) {
        for (int j = 0; j < indice - i - 1; j++) {
            if (filmes[j].nome > filmes[j + 1].nome) {
                swap(filmes[j], filmes[j + 1]);
            }
        }
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
void buscaTitulo(int indice, MBC *filmes) {
    cout << "Titulo do filme da busca: ";
    cin.ignore();
    string busca;
    getline(cin, busca);

    ordenarPorTitulo(filmes, indice);
    int posicao = BuscaBinariaRecursivaPorTitulo(filmes, 0, indice - 1, busca);

    if (posicao == -1) {
        cout << "O filme não está disponível" << endl;
    } else {
        imprime(filmes, posicao);
    }
    ordenarFilmes(filmes, indice);
}

void compactarVetor(MBC *&filmes, int &indice) {
    int j = 0; 
    for (int i = 0; i < indice; i++) {
        if (!filmes[i].deletado) {
            filmes[j] = filmes[i]; 
            j++;
        }
    }
    indice = j; 
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
        
        compactarVetor(filmes, indice);
        ordenarFilmes(filmes, indice);
    
    } else {
        cout << "Filme com ranking " << rankingProcurado << " não encontrado.\n" << endl;
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

    filme[indice].deletado = false;
    cout << "Filme registrado com sucesso!" << endl;
    
    indice++;
    ordenarFilmes(filme, indice);
    
}

void listarFilmes(MBC filme[], int indice) {
    int escolha;
    cout << "Escolha a ordem de listagem:\n";
    cout << "1) Por Ranking\n";
    cout << "2) Por Ordem Alfabética\n";
    cout << "Digite sua escolha: ";
    cin >> escolha;

    if (cin.fail() || (escolha != 1 && escolha != 2)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Opção inválida! Listando por ranking (padrão).\n";
        escolha = 1; 
    }

    if (escolha == 1) {
        ordenarFilmes(filme, indice); 
        for (int i = 0; i < indice; i++) {
            imprime(filme, i);
        }
    } else if (escolha == 2) {
        ordenarPorTitulo(filme, indice); 
        for (int i = 0; i < indice; i++) {
            imprime(filme, i);
        }
        ordenarFilmes(filme, indice); 
    }
}

//função que altera dados dos filmes, basta adicionar o ranking
void AlterarFilme(MBC *filmes, int indice) {
    int rankingProcurado;
    cout << "Digite o ranking do filme que deseja alterar: ";
    cin >> rankingProcurado;

    int posicao = BuscaBinariaRecursivaPorRanking(filmes, 0, indice - 1, rankingProcurado);

    if (posicao != -1) {
        cout << "Filme encontrado: " << filmes[posicao].nome << endl;

        cout << "\nDigite os novos dados do filme:" << endl;

     
        cout << "Nome: ";
        cin.ignore();  // Limpa o buffer
        getline(cin, filmes[posicao].nome);
        
      
        cout << "Diretor: ";
        getline(cin, filmes[posicao].diretor);

    
        cout << "Ano de Lançamento: ";
        cin >> filmes[posicao].lancamento;

        
        cout << "Bilheteira: ";
        cin >> filmes[posicao].bilheteria;

        cout << "Dados alterados com sucesso!" << endl;
    } else {
        cout << "Filme com ranking " << rankingProcurado << " não encontrado." << endl;
    }
}

void salvarEmBinario(const string& nomeArquivo, const MBC& filme) {
    ofstream arquivo(nomeArquivo, ios::binary | ios::app);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo para escrita!" << endl;
        return;
    }

    // Escreve os dados primitivos normalmente
    arquivo.write(reinterpret_cast<const char*>(&filme.ranking), sizeof(filme.ranking));
    arquivo.write(reinterpret_cast<const char*>(&filme.lancamento), sizeof(filme.lancamento));
    arquivo.write(reinterpret_cast<const char*>(&filme.bilheteria), sizeof(filme.bilheteria));
    arquivo.write(reinterpret_cast<const char*>(&filme.deletado), sizeof(filme.deletado));

    // Escreve o tamanho das strings antes dos dados
    size_t tamanhoNome = filme.nome.size();
    size_t tamanhoDiretor = filme.diretor.size();
    
    arquivo.write(reinterpret_cast<const char*>(&tamanhoNome), sizeof(size_t));
    arquivo.write(filme.nome.c_str(), tamanhoNome);
    
    arquivo.write(reinterpret_cast<const char*>(&tamanhoDiretor), sizeof(size_t));
    arquivo.write(filme.diretor.c_str(), tamanhoDiretor);

    arquivo.close();
}

void menu(MBC filme[], int& indice, int *capacidade) {
    int opcao = 999;
    do {
        cout << "╔════════════════════════════════════════════╗\n";
        cout << "║       MAIORES BILHETERIAS DO CINEMA        ║\n";
        cout << "╚════════════════════════════════════════════╝\n";
        cout<<"Escolha uma opção:"<<endl;
        cout<<endl;
        cout<<"1) Listar Filmes"<<endl;
        cout<<"2) Cadastrar novo Filme"<<endl;
        cout<<"3) Buscar Filme"<<endl;
        cout<<"4) Deletar Filme"<<endl;
        cout<<"5) Imprimir Filmes por Intervalo"<<endl;
        cout<<"6) Alterar Dados de um Filme"<<endl;
        cout<<"7) Salvar alterações"<<endl;
        cout<<"0) Sair"<<endl;
        cout << endl;
        cout<<"Digite sua opção: ";
        cin >> opcao;

        switch (opcao) {
            case 1: {
                listarFilmes(filme, indice);
            } break;
            case 2: {
                registrarNovo(filme, indice, capacidade);
            } break;
            case 3: {
                buscaTitulo(indice, filme);
            } break;
            case 4: {
                DeletarFilme(filme, indice);
            } break;
            case 5: {
                imprimirPorIntervalo(filme, indice);
            } break; 
            case 6: {
                AlterarFilme(filme, indice);
            } break;                      
            case 7: {
                salvarArquivo(filme, indice);
                 for (int i = 0; i < indice; i++) {
                    if (!filme[i].deletado) {
                        salvarEmBinario("MaioresBilheteriasCinema.bin", filme[i]);
                    }
                }
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
    int indice = 0, capacidade = 40;
    MBC *filme = new MBC[capacidade];

    lerArquivo(filme, indice, &capacidade);
    menu(filme, indice, &capacidade);

    delete[] filme;
    return 0;
}
