#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdio>
#include <cmath>

using namespace std;
// ---------------------------------------------------------------------------------------------------------------------------------------
struct Elemento
{
    char *chave;
    int valor;
};
// ---------------------------------------------------------------------------------------------------------------------------------------
// particiona o subvetor v[beg, ..., end - 1]
int particiona(Elemento *&v, int beg, int end, int pivo);

int findColumn(int column, const char *line, int lineSize, char delim);

int getCountColumns(const char *linha);

void getColumsPositions(const char *linha, int numCols, const char *chave, const char *valor, int &poschave, int &posvalor);

void getColumn(int col, const char *linha, char *&palavra);

void quickSort2(Elemento *&v, int beg, int end);

void quickSort(Elemento *&v, int n);

void gravarArquivo(char *nome, Elemento *&elementos, int n);

// ---------------------------------------------------------------------------------------------------------------------------------------



int main(int argc, char **argv)
{

    char *fileIn = argv[1];
    int n = atoi(argv[2]);
    const char *chave = argv[3];
    const char *valor = argv[4];

    // Váriaveis
    ifstream file(fileIn);
    string buffer;

    char *linha;

    // Saber qual coluna está a chave e o valor
    getline(file, buffer);

    linha = (char *)buffer.c_str(); // TRANSFORMA PRA CHAR

    // Saber número de colunas
    int numCols = getCountColumns(linha);

    // Saber qual coluna está a chave e o valor
    int posChave, posValor;
    getColumsPositions(linha, numCols, chave, valor, posChave, posValor);

    // Separa em vários arquivos
    int posLinha = 0;
    int nLinhasArquivo = 0;
    bool fimArquivo = false;

    char **chaveAux = new char *[n];
    char **valorAux = new char *[n];
    char* chavetmp;
    char* valortmp;
    Elemento *elementos = new Elemento [n];
    char *nomeArquivo;
    int z=0;
    while (true && z < 10000)
    {
        z++;
        getline(file, buffer);
        linha = (char *)buffer.c_str();


        getColumn(posChave, linha, chaveAux[posLinha]);
        getColumn(posValor, linha, valorAux[posLinha]);
        //cout<<chaveAux[posLinha];
        //cout << posLinha << endl;
        
        //continue;
        elementos[posLinha].chave = chaveAux[posLinha];
        elementos[posLinha].valor = atoi(valorAux[posLinha]);

    
        if (posLinha == n - 1 || file.eof()){
            quickSort(elementos, n);
            //cout << "Aqui" << endl;
            int aux = 5 + log10(nLinhasArquivo+1/n);
            nomeArquivo = new char[aux];
            sprintf(nomeArquivo,"%d.txt",nLinhasArquivo/n);

            //cout << nLinhasArquivo/n << endl;
            //cout << nomeArquivo << endl;
            // Ordena linhas e salva ordenado num arquivo
            //gravarArquivo(nomeArquivo, elementos, n);
            
            ofstream fout(nomeArquivo);
            for (int i = 0; i < posLinha+1; i++){
                fout << elementos[i].chave << "," << elementos[i].valor << endl;
                //cout << elementos[i].chave << "," << elementos[i].valor << endl;
                fout.flush();
            }
            for (int i = 0; i < posLinha + 1; i++){
                //cout << elementos[i].chave << " " << elementos[i].valor << endl;
            }
            posLinha = -1;
        }
        if (file.eof()){
            break;
        }
        nLinhasArquivo++;
        posLinha++;
    }

    // Intercalação

    return 0;
}

int particiona(Elemento *&v, int beg, int end, int pivo){
    char *valorPivo = v[pivo].chave;
    //colocamos o pivo temporariamente na ultima posição
    swap(v[pivo], v[end - 1]);
    // ao acharmos um elemento menor do que o pivo, vamos coloca-lo
    // na posicao "pos"
    int pos = beg;
    for (int i = beg; i < end - 1; i++){
        if (strcmp(v[i].chave, valorPivo) < 0){
            swap(v[pos], v[i]);
            pos++;
        }
    }
    //coloque o pivo depois do ultimo elemento menor que ele
    swap(v[pos], v[end - 1]);
    return pos;
}

void quickSort2(Elemento *&v, int beg, int end){
    if (beg == end)
        return;
    int pos = particiona(v, beg, end, beg);
    quickSort2(v, beg, pos);
    quickSort2(v, pos + 1, end);
}

void quickSort(Elemento *&v, int n){
    quickSort2(v, 0, n);
}

int findColumn(int column, const char *line, int lineSize, char delim){
    int aux = 0;
    return 0;
}

int getCountColumns(const char *linha){
    int numCols = 1;
    int i = 0;
    while (linha[i] != '\0')
    {
        if (linha[i] == ',')
        {
            numCols++;
        }
        i++;
    }
    return numCols;
}

void getColumsPositions(const char *linha, int numCols, const char *chave, const char *valor, int &poschave, int &posvalor){
    int inicio = -1;
    int fim = 0;
    char *palavra;
    for (int i = 0; i < numCols; i++)
    {
        // Obter a palavra por coluna
        while (linha[fim] != ',' && linha[fim] != '\0')
        {
            fim++;
        }
        int tam = fim - inicio;
        palavra = new char[tam];
        strncpy(palavra, linha + inicio + 1, tam - 1);

        // Verificar a palavra é igual a chave
        if (strcmp(chave, palavra) == 0)
        {
            poschave = i;
        }
        // Verificar a palavra é igual ao valor
        if (strcmp(valor, palavra) == 0)
        {
            posvalor = i;
        }
        inicio = fim;
        fim++;
        delete palavra;
    }
}

void getColumn(int col, const char *linha, char *&palavra){
    char *pch;
    pch = strtok ((char*)linha,",");
    int i=0;
    while (pch != NULL)
    {
        if(i==col)
            return;
        pch = strtok (NULL, ",");
        i++;
    }
    /*
    int inicio = -1;
    int fim = 0;
    const char *column;
    
    for (int i = 0; i < col; i++)
    {
        while (linha[fim] != ',' && linha[fim] != '\0')
        {
            fim++;
        }
        inicio = fim;
        fim++;
    }
    while (linha[fim] != ',' && linha[fim] != '\0')
    {
        fim++;
    }
    int tam = fim - inicio;
    palavra = new char[tam];
    strncpy(palavra, linha + inicio + 1, tam - 1);
    cout<<palavra<<endl;
    */
}

void gravarArquivo(char *nome, Elemento *&elementos, int n){
    cout << nome << endl;
    ofstream fout(nome);
    for (int i = 0; i < n; i++){
        fout << elementos[i].chave << "," << elementos[i].valor << endl;
        cout << elementos[i].chave << "," << elementos[i].valor << endl;
        fout.flush();
    }  
}

