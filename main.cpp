#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<cstdio>

using namespace std;

int findColumn(int column, const char *line, int lineSize, char delim){
    int aux=0;
    return 0;
}

int getCountColumns(const char*linha){
    int numCols = 1;
    int i=0;
    while(linha[i]!='\0'){
        if(linha[i]==','){
            numCols++;
        }
        i++;
    }
    return numCols;
}

void getColumsPositions(const char*linha, int numCols, const char*chave,const char*valor,int &poschave,int &posvalor){
    int inicio = -1;
    int fim = 0;
    char *palavra;
    for(int i=0;i<numCols;i++){
        // Obter a palavra por coluna
        while(linha[fim]!=',' && linha[fim]!='\0'){
            fim++;
        }
        int tam = fim-inicio;
        palavra = new char [tam];
        strncpy(palavra, linha+inicio+1, tam-1);

        // Verificar a palavra é igual a chave
        if(strcmp(chave,palavra)==0){
            poschave = i;
        }
        // Verificar a palavra é igual ao valor
        if(strcmp(valor,palavra)==0){
            posvalor = i;
        }
        inicio = fim;
        fim++;
        delete palavra;
    }
}

void getColumn(int col, const char*linha, char*&palavra){
    int inicio = -1;
    int fim = 0;
    const char* column;
    for(int i=0;i<col;i++){
        while(linha[fim]!=',' && linha[fim]!='\0'){
            fim++;
        }
        inicio = fim;
        fim++;
    }
    while(linha[fim]!=',' && linha[fim]!='\0'){
        fim++;
    }
    int tam = fim-inicio;
    palavra = new char [tam];
    strncpy(palavra, linha+inicio+1, tam-1);
}



int main(int argc, char **argv){
    
    char *fileIn = argv[1];
    int n = atoi(argv[2]);
    const char *chave = argv[3];
    const char *valor = argv[4];

    
    // Váriaveis
    ifstream file(fileIn);
    string buffer;

    
    char *linha;

    // Saber qual coluna está a chave e o valor
    getline(file,buffer);

    linha = (char*)buffer.c_str(); // TRANSFORMA PRA CHAR

    // Saber número de colunas
    int numCols = getCountColumns(linha);

    // Saber qual coluna está a chave e o valor
    int posChave,posValor;
    getColumsPositions(linha,numCols,chave,valor,posChave,posValor);

    // Separa em vários arquivos
    int posLinha = 0;
    int nLinhasArquivo=0;
    bool fimArquivo = false;

    char **chaveAux = new char*[n];
    char **valorAux = new char*[n];

    while(true){
        getline(file,buffer);
        linha = (char*)buffer.c_str();

        getColumn(posChave,linha,chaveAux[posLinha]);
        getColumn(posValor,linha,valorAux[posLinha]);
        cout << posLinha << endl;

        if(posLinha==n-1 || file.eof()){
            //cout<<"Salva no arquivo"<<endl;
            // Ordena linhas e salva ordenado num arquivo
            for(int i=0;i<posLinha+1;i++){
                cout<<chaveAux[i]<<" "<<valorAux[i]<<endl;
            }
            posLinha=-1;
        }
        if(file.eof()){
            break;
        }
        nLinhasArquivo++;
        posLinha++;
    }

    //cout<<nLinhasArquivo<<endl;

    // Intercalação


    return 0;
}