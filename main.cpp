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



int main(){
    /*
    int n=10;
    
    char*teste=(char*)n;
    //sprintf(teste,"oi %d",10);
    cout<<teste;
    return 0;
    */
    // Váriaveis
    string arquivo = "file.txt";
    int n = 3; // Número de linhas suportada
    const char *chave = "categoria";
    const char *valor = "idade";
    string buffer;
    ifstream file(arquivo.c_str());
    const char *linha;
    int tamLinha;

    // Saber qual coluna está a chave e o valor
    getline(file,buffer);
    linha = buffer.c_str();
    tamLinha = strlen(linha);

    // Saber número de linhas
    int numCols = getCountColumns(linha);

    // Saber qual coluna está a chave e o valor
    int colChave,colValor;
    getColumsPositions(linha,numCols,chave,valor,colChave,colValor);

    // Separa em vários arquivos
    char **linhas = new char*[n];
    char **palavras = new char*[n];
    int ordemLinhas[n];
    int palavra[n];
    int plinha = 0;
    int nLinhasArquivo=-1;
    bool fimArquivo = false;

    char **chaveAux = new char*[n];
    char **valorAux = new char*[n];

    while(true){
        nLinhasArquivo++;
        getline(file,buffer);
        linhas[plinha] = (char*)buffer.c_str();

        getColumn(colChave,linha,chaveAux[plinha]);
        getColumn(colValor,linha,valorAux[plinha]);

        plinha++;
        if(plinha==n || file.eof()){
            cout<<"Salva no arquivo"<<endl;
            // Ordena linhas e salva ordenado num arquivo
            for(int i=0;i<n;i++){
                cout<<chaveAux[i]<<" "<<valorAux[i]<<endl;
            }
            plinha=0;
            
        }
        if(file.eof()){
            break;
        }
    }

    cout<<nLinhasArquivo<<endl;

    // Intercalação


    return 0;
}