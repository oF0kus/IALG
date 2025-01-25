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

void lerArquivo() {
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
        }

        delete[] vetorMBC;
    } else {
        cout << "Erro na leitura do arquivo" << endl;
    }


}


void menu(){
    int opcao = 999;
   do{ 
    cout<<"Escolha uma opção:"<<endl;
        cout<<"\t1. Listar Filmes"<<endl;
        cout<<"\t2. Cadastrar novo Filme"<<endl;
        cout<<"\t3. Buscar Filme"<<endl;
        cout<<"\t4. Deletar Filme"<<endl;
        cout<<"\t5. Alterar Dados de um Filme"<<endl;
        cout<<"\t6. Imprimir Filmes por Intervalo"<<endl;
        cout<<"\t7. Salvar alterações"<<endl;
        cout<<"\t8. Sair"<<endl;
        cin >> opcao;

    switch (opcao)
    {
    
    case 1:{

        imprime();
    }
        break;

    case 0:{
        cout << "Saindo" << endl;
    }
        break;

    default:
        cout << "Opcao invalida" << endl;
        break;
    }

   
} while( opcao != 0 );

}



void menu(){
    int opcao = 999;
   do{ 
    cout<<"Escolha uma opção:"<<endl;
        cout<<"\t1. Listar Filmes"<<endl;
        cout<<"\t2. Cadastrar novo Filme"<<endl;
        cout<<"\t3. Buscar Filme"<<endl;
        cout<<"\t4. Deletar Filme"<<endl;
        cout<<"\t5. Alterar Dados de um Filme"<<endl;
        cout<<"\t6. Imprimir Filmes por Intervalo"<<endl;
        cout<<"\t7. Salvar alterações"<<endl;
        cout<<"\t8. Sair"<<endl;
        cin >> opcao;

    switch (opcao)
    {
    
    case 1:{

        cout << 'a' << endl;
    }
        break;

    case 0:{
        cout << "Saindo" << endl;
    }
        break;

    default:
        cout << "Opcao invalida" << endl;
        break;
    }

   
} while( opcao != 0 );

}

int main(){


    menu();

    return 0;
}
