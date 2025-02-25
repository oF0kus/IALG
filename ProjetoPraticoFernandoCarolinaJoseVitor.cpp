/*
Grupo 25 - Turma 14B

Fernando Chaves Scarabeli, 
José Vítor Machado de Oliveira, 
Carolina Costa Ude 

Tema: Maiores Bilheterias do Cinema
Campos: Ranking (inteiro), Ano de lançamento (inteiro),Nome do filme (string com espaços), Diretor (string com espaços), Bilheteria total (float)

*/

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <limits>

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

    //Separa parte inteira e decimal
    long long parteInteira = static_cast<long long>(bilheteria);
    int centavos = static_cast<int>((bilheteria - parteInteira) * 100 + 0.5); // Arredonda para 2 dígitos

    //Formata a parte inteira com pontos
    string strInteira = to_string(parteInteira);
    int tamanho = strInteira.size();
    for (int i = tamanho - 3; i > 0; i -= 3) {
        strInteira.insert(i, ".");
    }

    //Formata os centavos (sempre 2 dígitos)
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

void lerArquivo(MBC *&filme, int &indice, int *capacidade) {
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

void lerArquivoBinario(MBC *&filme, int &indice, int *capacidade) {
    ifstream entrada("MaioresBilheteriasCinema.bin", ios::binary);

    if (!entrada) {
        cout << "Arquivo binário não encontrado. Lendo dados do arquivo CSV." << endl;
        return;
    }

    MBC temp;
    // Utiliza a leitura do campo ranking para controlar o loop.
    while (entrada.read(reinterpret_cast<char*>(&temp.ranking), sizeof(temp.ranking))) {
        entrada.read(reinterpret_cast<char*>(&temp.lancamento), sizeof(temp.lancamento));
        entrada.read(reinterpret_cast<char*>(&temp.bilheteria), sizeof(temp.bilheteria));
        entrada.read(reinterpret_cast<char*>(&temp.deletado), sizeof(temp.deletado));

        // Lê o nome
        size_t tamanhoNome;
        entrada.read(reinterpret_cast<char*>(&tamanhoNome), sizeof(tamanhoNome));
        char* bufferNome = new char[tamanhoNome + 1];
        entrada.read(bufferNome, tamanhoNome);
        bufferNome[tamanhoNome] = '\0';
        temp.nome = bufferNome;
        delete[] bufferNome;

        // Lê o diretor
        size_t tamanhoDiretor;
        entrada.read(reinterpret_cast<char*>(&tamanhoDiretor), sizeof(tamanhoDiretor));
        char* bufferDiretor = new char[tamanhoDiretor + 1];
        entrada.read(bufferDiretor, tamanhoDiretor);
        bufferDiretor[tamanhoDiretor] = '\0';
        temp.diretor = bufferDiretor;
        delete[] bufferDiretor;

        // Verifica se houve erro durante a leitura dos campos
        if (!entrada) {
            cout << "Erro ao ler os dados do arquivo binário." << endl;
        }

        // Redimensiona o vetor se necessário
        if (indice >= *capacidade) {
            RedimensionarVetor(filme, capacidade);
        }

        // Adiciona o filme ao vetor
        filme[indice] = temp;
        indice++;
    }

    entrada.close();
}

void imprimirPorIntervalo(MBC filme[], int indice) {
    int inicio, fim;

    cout << "Digite o ranking inicial: ";
    
    //Verifica se o campo 'e preenchido correntamente  
    while (!(cin >> inicio) || inicio < 1 || inicio > indice) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Entrada inválida. Digite um ranking válido (1 a " << indice << "): ";
    }

    //Verifica se o campo 'e preenchido correntamente  
    cout << "Digite o ranking final: ";
    while (!(cin >> fim) || fim < inicio || fim > indice) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Entrada inválida. Digite um ranking válido (" << inicio << " a " << indice << "): ";
    }
    
    //Imprime os filmes nos intervalos desejados
    cout  << endl << "Filmes no intervalo de ranking [" << inicio << " - " << fim << "]:" << endl;
    for (int i = 0; i < indice; i++) {
        if (filme[i].ranking >= inicio && filme[i].ranking <= fim) {
            imprime(filme, i);
        }
    }
}

//Salva as modificacoes feitas no arquivo
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
//Salva as modificacoes feitas no arquivo em binario
void salvarEmBinario(ofstream &arquivo, const MBC &filme) {
    arquivo.write(reinterpret_cast<const char*>(&filme.ranking), sizeof(filme.ranking));
    arquivo.write(reinterpret_cast<const char*>(&filme.lancamento), sizeof(filme.lancamento));
    arquivo.write(reinterpret_cast<const char*>(&filme.bilheteria), sizeof(filme.bilheteria));
    arquivo.write(reinterpret_cast<const char*>(&filme.deletado), sizeof(filme.deletado));

    size_t tamanhoNome = filme.nome.size();
    arquivo.write(reinterpret_cast<const char*>(&tamanhoNome), sizeof(size_t));
    arquivo.write(filme.nome.c_str(), tamanhoNome);

    size_t tamanhoDiretor = filme.diretor.size();
    arquivo.write(reinterpret_cast<const char*>(&tamanhoDiretor), sizeof(size_t));
    arquivo.write(filme.diretor.c_str(), tamanhoDiretor);
}

// Merge Sort para ordenação por título
void mergePorTitulo(MBC *filmes, int inicio, int meio, int fim) {
    int n1 = meio - inicio + 1;
    int n2 = fim - meio;

    MBC *L = new MBC[n1];
    MBC *R = new MBC[n2];

    for (int i = 0; i < n1; i++)
        L[i] = filmes[inicio + i];
    for (int j = 0; j < n2; j++)
        R[j] = filmes[meio + 1 + j];

    int i = 0, j = 0, k = inicio;
    while (i < n1 && j < n2) {
        if (L[i].nome <= R[j].nome) {
            filmes[k] = L[i];
            i++;
        } else {
            filmes[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        filmes[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        filmes[k] = R[j];
        j++;
        k++;
    }

    delete[] L;
    delete[] R;
}

void mergeSortPorTitulo(MBC *filmes, int inicio, int fim) {
    if (inicio >= fim) return;
    int meio = inicio + (fim - inicio) / 2;
    mergeSortPorTitulo(filmes, inicio, meio);
    mergeSortPorTitulo(filmes, meio + 1, fim);
    mergePorTitulo(filmes, inicio, meio, fim);
}

//Ordena a lista em ordem alfabetica
void ordenarPorTitulo(MBC *filmes, int indice) {
    mergeSortPorTitulo(filmes, 0, indice - 1);
}

// Merge Sort para ordenação por bilheteria
void mergePorBilheteria(MBC *filmes, int inicio, int meio, int fim) {
    int n1 = meio - inicio + 1;
    int n2 = fim - meio;

    MBC *L = new MBC[n1];
    MBC *R = new MBC[n2];

    for (int i = 0; i < n1; i++)
        L[i] = filmes[inicio + i];
    for (int j = 0; j < n2; j++)
        R[j] = filmes[meio + 1 + j];

    int i = 0, j = 0, k = inicio;
    while (i < n1 && j < n2) {
        if (L[i].bilheteria >= R[j].bilheteria) { // Ordenação decrescente
            filmes[k] = L[i];
            i++;
        } else {
            filmes[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        filmes[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        filmes[k] = R[j];
        j++;
        k++;
    }

    delete[] L;
    delete[] R;
}

void mergeSortPorBilheteria(MBC *filmes, int inicio, int fim) {
    if (inicio >= fim) return;
    int meio = inicio + (fim - inicio) / 2;
    mergeSortPorBilheteria(filmes, inicio, meio);
    mergeSortPorBilheteria(filmes, meio + 1, fim);
    mergePorBilheteria(filmes, inicio, meio, fim);
}

//Ordena a lista em ordem decrescente
void ordenarFilmes(MBC filmes[], int indice) {
    mergeSortPorBilheteria(filmes, 0, indice - 1);
    // Atualiza rankings
    for (int i = 0; i < indice; i++) {
        filmes[i].ranking = i + 1;
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

//Remove fisicamente os filmes marcados como deletados, compactando o vetor.
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

//Marca um filme com flag de deletado, tirando ele da listagem
void DeletarFilme(MBC *&filmes, int &indice) {
    int rankingProcurado;
    cout << "Digite o ranking do filme que deseja deletar: ";
    
    //Verifica se o campo 'e preenchido correntamente  
    while (!(cin >> rankingProcurado) || rankingProcurado < 1 || rankingProcurado > indice) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Entrada inválida. Digite um ranking válido (1 a " << indice << "): ";
    }

    //Busca o filme pelo ranking
    int posicao = BuscaBinariaRecursivaPorRanking(filmes, 0, indice - 1, rankingProcurado);
    if (posicao != -1) {
        filmes[posicao].deletado = true;
        cout << "Filme com ranking " << rankingProcurado << " foi marcado como deletado." << endl << endl;

        //Remove o filme e ordena a lista novamente
        compactarVetor(filmes, indice);
        ordenarFilmes(filmes, indice);

    } else {
        cout << "Filme com ranking " << rankingProcurado << " não encontrado." << endl << endl;
    }
}

void registrarNovo(MBC *&filme, int &indice, int *capacidade) {
    if (indice == *capacidade) {
        RedimensionarVetor(filme, capacidade);
    }

    cout << "Digite o ano de lançamento: ";

    //Verifica se o campo 'e preenchido correntamente  
    while (!(cin >> filme[indice].lancamento)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Entrada inválida. Digite um ano válido: ";
    }
    cin.ignore();

    cout << "Digite o nome do filme: ";
    getline(cin, filme[indice].nome);

    cout << "Digite o nome do diretor: ";
    getline(cin, filme[indice].diretor);

    cout << "Digite a bilheteria: ";

    //Verifica se o campo 'e preenchido correntamente  
    while (!(cin >> filme[indice].bilheteria)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Entrada inválida. Digite um valor válido: ";
    }
    cin.ignore();

    filme[indice].deletado = false;
    cout << "Filme registrado com sucesso!" << endl;

    indice++;
    ordenarFilmes(filme, indice);
}

void listarFilmes(MBC filme[], int indice) {
    int escolha;
    cout << "Escolha a ordem de listagem:" << endl;
    cout << "1) Por Ranking" << endl;
    cout << "2) Por Ordem Alfabética" << endl;
    cout << "Digite sua escolha: ";

    //Verifica se o campo 'e preenchido correntamente  
    while (!(cin >> escolha) || (escolha != 1 && escolha != 2)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Opção inválida! Digite 1 ou 2: ";
    }

    if (escolha == 1) {
        
        //Ordem decrescente
        ordenarFilmes(filme, indice);
        for (int i = 0; i < indice; i++) {
            imprime(filme, i);
        }
    } else if (escolha == 2) {

        //Ordem alfabetica
        ordenarPorTitulo(filme, indice);
        for (int i = 0; i < indice; i++) {
            imprime(filme, i);
        }
        ordenarFilmes(filme, indice);
    }
}

void AlterarFilme(MBC *filmes, int indice) {
    if (indice <= 0 || filmes == nullptr) {
        cout << "Não há filmes para alterar." << endl;
        return;
    }

    int rankingProcurado;
    cout << "Digite o ranking do filme que deseja alterar: ";

    // Verifica se a entrada é válida e dentro do intervalo
    while (!(cin >> rankingProcurado) || rankingProcurado < 1 || rankingProcurado > indice) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Entrada inválida. Digite um ranking válido (1 a " << indice << "): ";
    }

    int posicao = BuscaBinariaRecursivaPorRanking(filmes, 0, indice - 1, rankingProcurado);

    if (posicao >= 0 && posicao < indice ) {
        cout << "Filme encontrado: " << filmes[posicao].nome << endl;

        cout << endl << "Digite os novos dados do filme:" << endl;

        cout << "Nome: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, filmes[posicao].nome);

        cout << "Diretor: ";
        getline(cin, filmes[posicao].diretor);

        cout << "Ano de Lançamento: ";

        // Verifica se a entrada é válida para o ano de lançamento
        while (!(cin >> filmes[posicao].lancamento)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Entrada inválida. Digite um ano válido: ";
        }

        cout << "Bilheteria: ";

        // Verifica se a entrada é válida para a bilheteira
        while (!(cin >> filmes[posicao].bilheteria)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Entrada inválida. Digite um valor válido: ";
        }

        cout << "Dados alterados com sucesso!" << endl;
    } else {
        cout << "Filme com ranking " << rankingProcurado << " não encontrado." << endl;
    }
}

void menu(MBC filme[], int &indice, int *capacidade) {
    int opcao = 999;
    do {
        cout << "╔════════════════════════════════════════════╗" << endl;
        cout << "║       MAIORES BILHETERIAS DO CINEMA        ║" << endl;
        cout << "╚════════════════════════════════════════════╝" << endl;
        cout << "Escolha uma opção:" << endl;
        cout << endl;
        cout << "1) Listar Filmes" << endl;
        cout << "2) Cadastrar novo Filme" << endl;
        cout << "3) Buscar Filme" << endl;
        cout << "4) Deletar Filme" << endl;
        cout << "5) Imprimir Filmes por Intervalo" << endl;
        cout << "6) Alterar Dados de um Filme" << endl;
        cout << "7) Salvar alterações" << endl;
        cout << "0) Sair" << endl;
        cout << endl;
        cout << "Digite sua opção: ";

        //Verifica se o campo 'e preenchido correntamente
        while (!(cin >> opcao) || opcao < 0 || opcao > 7) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Opção inválida! Digite um número entre 0 e 7: ";
        }

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
                ofstream arquivoBin("MaioresBilheteriasCinema.bin", ios::binary | ios::trunc);
                if (arquivoBin.is_open()) {
                    for (int i = 0; i < indice; i++) {
                        if (!filme[i].deletado) {
                            salvarEmBinario(arquivoBin, filme[i]);
                        }
                    }
                    arquivoBin.close();
                    cout << "Dados salvos no arquivo binário com sucesso!" << endl;
                } else {
                    cout << "Erro ao salvar no arquivo binário." << endl;
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

int main() {
    int indice = 0, capacidade = 40;
    MBC *filme = new MBC[capacidade];

    //Tenta carregar os dados do arquivo binário
    lerArquivoBinario(filme, indice, &capacidade);

    //Se não houver dados no binário, carrega do CSV
    if (indice == 0) {
        lerArquivo(filme, indice, &capacidade);
    }

    menu(filme, indice, &capacidade);

    delete[] filme;
    return 0;
}
