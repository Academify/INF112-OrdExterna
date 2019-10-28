#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <iomanip>

using namespace std;

// ---------------------------------------------------------------------------------------------------------------------------------------
/* 
    Este trabalho foi desenvolvido pelos alunos Gabriel Bezerra - 98891, Guilherme Dias - 95673 e Thiago Ferreira - 98893.

    Como pedido, este trabalho lẽ, pela linha de comando do terminal, um arquivo para o qual se deseja fazer a média dos valores agrupados
    por chave e imprime na tela os resultados. 

    Os métodos de ordenação usados foram o quick sort para a ordenação dos valores dentro de pequenos arquivos e o mergeSort para ordenar 
    todos os valores que irão para o arquivo final ordenado.

    
*/
// ---------------------------------------------------------------------------------------------------------------------------------------

struct Elemento{
    char *chave;
    char *valor;
};

// Esta função tem o objetivo de descobrir em qual posição da linha obtida do arquivo estão chave e valor
void saberQualColunaEstaChaveEValor(
    ifstream &file, char *chave, char* valor,int &posChave, int &posValor){

    string buffer;
    getline(file,buffer);
    char *linha = (char*)buffer.c_str();
    char *coluna = strtok(linha,",");
    int cont = 0;
    while (coluna!=NULL){

        if(strcmp(coluna,chave)==0)  posChave = cont;
        if(strcmp(coluna,valor)==0)  posValor = cont;
        coluna = strtok(NULL,",");
        cont++;
    }
}

// A função particiona é utilizada dentro dos métodos de ordenação de valores (QuickSort) 
int particiona (Elemento elementos[], int menor, int maior){
    char *pivo = elementos[maior].chave; 
    int i = (menor - 1);
    for (int j = menor; j <= maior - 1; j++){

        if (strcmp(elementos[j].chave,pivo)<0){
            i++;
            swap(elementos[i], elementos[j]);
        }
    }
    swap(elementos[i + 1], elementos[maior]);
    return (i + 1);
}

// -------------------------- QuickSort -----------------------------------------------------------------

void quickSort2(Elemento elementos[], int menor, int maior){
    if (menor < maior){
        int pivo = particiona(elementos, menor, maior);
        quickSort2(elementos, menor, pivo - 1);
        quickSort2(elementos, pivo + 1, maior);
    }
}

void quickSort(Elemento *&v, int n){
    quickSort2(v, 0, n);
}

// ----------------------------------------------------------------------------------------------------

// Essa função é usada para obter um elemento de uma linha retirada de um arquivo
void obterElemento(char*&linha, int posChave, int posValor, Elemento &elemento){
    int maior = posChave>posValor?posChave:posValor;
    char* s = strdup(linha);
    char* coluna = strtok( s, ",");
    int i=0;
    while (coluna)
    {
        if(i==posChave){
            elemento.chave = strdup(coluna);
        }
        if(i==posValor){
            elemento.valor = strdup(coluna);
        }
        coluna = strtok( NULL,",");
        i++;
    }
    free(s);

}

// Essa função pega n linhas e as ordena, em seguida, cria um arquivo e as salva.
int ordenarESepararEmArquivos(ifstream &file,int n, int posChave, int posValor){
    string buffer;
    char*linha;
    char*chave;
    char*valor;
    int nLinhas=0;
    Elemento *elementos = new Elemento[n];
    int cont=0;
    
    ofstream fout;
    while(getline(file,buffer)){
        linha = strdup(buffer.c_str());
        int aux = nLinhas%n;
        
        obterElemento(linha,posChave,posValor,elementos[aux]);
        
        if(aux==n-1|| file.peek()==-1){
            // Ordena vetor
            quickSort(elementos,aux);
            
            //
            char *nomeArquivo = new char[20];
            sprintf(nomeArquivo,"%d.txt",nLinhas/n);
            fout.open(nomeArquivo);
            for(int i=0;i<aux+1;i++){
                fout<<elementos[i].chave<<","<<elementos[i].valor<<endl;
                free(elementos[i].chave);
                free(elementos[i].valor);
            }
            fout.close();
            delete[] elementos;
            delete[] nomeArquivo;
            elementos = new Elemento[n];
        }
        
        
        nLinhas++;
        free(linha);
    }
    
    delete[]elementos;
    return nLinhas;
}

// Essa função copia um elemento
void copiaElemento(Elemento &dest, Elemento &source){
    dest.chave = source.chave;
    dest.valor = source.valor;
}

// Essa função faz a intercalação entre os arquivos ordenados (merge)
void ordenacaoExterna(ofstream &fout, int nLinhasArquivos, int n){
    
    int nDispositivos = nLinhasArquivos/n+1;
    string buffer;
    ifstream* dispositivos = new ifstream[nDispositivos];
    Elemento* elementos = new Elemento[nDispositivos];
    char*linha;
    Elemento* menor;
    int posmenor;
    int *hasLine = new int[nDispositivos];
    
    
    // Abre os arquivos e copia para um vetor auxiliar a primeira linha de cada arquivo
    for(int i=0;i<nDispositivos;i++){
        // Abre os arquivos
        char *nome = new char[20];
        sprintf(nome, "%d.txt", i);
        dispositivos[i].open(nome);
        delete[]nome;


        hasLine[i]=n;
        getline(dispositivos[i],buffer);
        linha = (char *)buffer.c_str();
        obterElemento(linha,0,1,elementos[i]);
    }
    
    hasLine[nDispositivos-1]=nLinhasArquivos%n;
    menor = &elementos[0];
    posmenor = 0;

    
    for(int i=0;i<nLinhasArquivos;i++){

        for(int j=0;j<nDispositivos;j++){
            if(hasLine[j]>0){
                menor = &elementos[j];
                posmenor = j;
                break;
            }
        }

        for(int j=0;j<nDispositivos;j++){
            if(hasLine[j]>0){
                if(strcmp(elementos[j].chave,menor->chave)<0){
                    menor = &elementos[j];
                    posmenor=j;
                    break;
                }
            }
            menor = &elementos[posmenor];
        }
        
        fout<<menor->chave<<","<<menor->valor<<endl;
        free(menor->chave);
        free(menor->valor);
        hasLine[posmenor]--;
        
        getline(dispositivos[posmenor],buffer);
        linha = (char *)buffer.c_str();
        obterElemento(linha,0,1,elementos[posmenor]);
        menor = &elementos[posmenor];
    }
    
    
    for(int i=0; i<nDispositivos;i++){
        dispositivos[i].close();
    }
    delete[]hasLine;
    delete[]dispositivos;
    delete[]elementos;
}

// Essa função é responsável por calcular a média dos valores que compartilham da mesma chave
void calculaMedia(ifstream &ordenado){
    
    long double soma=0.0;
    long double media;
    char *chave;
    int cont =0;
    string linha;
    char *linhaChar;
    Elemento e;

    // - Pegando a chave do primeiro elemento ------
    getline(ordenado, linha);
    linhaChar = (char*)linha.c_str();
    obterElemento(linhaChar, 0, 1, e);
    
    chave = e.chave;
    soma = 0.0;
    soma += stold(e.valor);
    chave = e.chave;
    cont = 1;
    // ---------------------------------------------------
    while(getline(ordenado, linha)){        
        linhaChar = (char*)linha.c_str();
        obterElemento(linhaChar, 0, 1, e);
        if (strcmp(chave, e.chave)==0){
            soma +=stold(e.valor);
            free(chave);
            free(e.valor);
            chave = e.chave;
            cont++;
        }else{  
            media = soma/cont;
            cout << fixed << chave << ", " << setprecision(15) << media << endl;
            soma = 0.0;
            soma +=stold(e.valor);
            free(chave);
            free(e.valor);
            chave = e.chave;
            cont = 1;
        }
    }
    media = soma/cont;
    cout << fixed << chave << ", " << setprecision(15) << media << endl;
    free(chave);
    
//---------------------------------------------------------------------------------------------

}


int main(int argc, char **argv){

    //Pega os dados da linha de comando
    char *fileIn = argv[1];
    int n = atoi(argv[2]);
    const char *chave = argv[3];
    const char *valor = argv[4];

    //Abre o arquivo para leitura
    ifstream file(fileIn);

    //Define um arquivo para saída dos dados
    ofstream fout("ordenado.txt");

    int posChave,posValor;

    //Obtem as posições de chave e valor
    saberQualColunaEstaChaveEValor(file,(char*)chave,(char*)valor,posChave,posValor);

    // Chama a função que ordena em vários arquivos que retorna a quantidade de linhas do arquivo inicial
    int nLinhas = ordenarESepararEmArquivos(file,n,posChave,posValor);

    // Faz o merge dos arquivos ordenados
    ordenacaoExterna(fout,nLinhas,n);

    // Abre o arquivo com os dados ordenados para calcular a média
    ifstream ordenado("ordenado.txt");

    //Calcula e imprime a média
    calculaMedia(ordenado);

    //Fecha os arquivos
    ordenado.close();
    fout.close();
    file.close();
    return 0;
}