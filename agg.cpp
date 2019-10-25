#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdio>
#include <cmath>

using namespace std;

struct Elemento
{
    char *chave;
    char *valor;
};

// Saber qual coluna está a chave e o valor
void saberQualColunaEstaChaveEValor(
    ifstream &file, char *chave, char* valor,int &posChave, int &posValor
)
{
    string buffer;
    getline(file,buffer);
    char *linha = (char*)buffer.c_str();
    char *coluna = strtok(linha,",");
    int cont = 0;
    while (coluna!=NULL)
    {
        if(strcmp(coluna,chave)==0)  posChave = cont;
        if(strcmp(coluna,valor)==0)  posValor = cont;
        coluna = strtok(NULL,",");
        cont++;
    }
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

Elemento obterElemento(char*linha,int posChave, int posValor){
    Elemento elemento;
    char *coluna = strtok(linha,",");
    int maior = posChave>posValor?posChave:posValor;
    for(int i=0;i<maior+1;i++){
        if(i==posChave){
            elemento.chave = coluna;
        }
        if(i==posValor){
            elemento.valor = coluna;
        }
        coluna = strtok(NULL,",");
    }
    return elemento;
}

void ordenarESepararEmArquivos(ifstream &file,int n, int posChave, int posValor){
    string buffer;
    char*linha;
    char*chave;
    char*valor;
    int nLinhas=0;
    Elemento *elementos = new Elemento[n];
    int cont=0;
    while(getline(file,buffer)){
        linha = (char*)buffer.c_str();
        int aux = nLinhas/n+1;
        //cout<<aux;
        elementos[cont]=obterElemento(linha,posChave,posValor);
        cont++;
        if(cont==4){
            cout<<"Teste";
        }
        nLinhas++;
    }
    delete[]elementos;
}

// Salvar arquivos ordenados

int main(int argc, char **argv){
    char *fileIn = argv[1];
    int n = atoi(argv[2]);
    const char *chave = argv[3];
    const char *valor = argv[4];
    ifstream file(fileIn);
    int posChave,posValor;
    saberQualColunaEstaChaveEValor(file,(char*)chave,(char*)valor,posChave,posValor);
    ordenarESepararEmArquivos(file,n,posChave,posValor);
    return 0;
}