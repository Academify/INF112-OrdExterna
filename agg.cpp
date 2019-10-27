#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <iomanip>

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


int partition (Elemento arr[], int low, int high)
{
    char *pivot = arr[high].chave;    //taking the last element as pivot
    int i = (low - 1);
    for (int j = low; j <= high- 1; j++)
    {
        // If current element is smaller than or
        // equal to pivot
        if (strcmp(arr[j].chave,pivot)<0)
        {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSort2(Elemento arr[], int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);
        quickSort2(arr, low, pi - 1);
        quickSort2(arr, pi + 1, high);
    }
}

void quickSort(Elemento *&v, int n){
    quickSort2(v, 0, n);
}

void obterElemento2(char*linha,int posChave, int posValor, Elemento &elemento){
    char *coluna = strtok(linha,",");
    int maior = posChave>posValor?posChave:posValor;
    for(int i=0;i<maior+1;i++){
        if(i==posChave){
            elemento.chave = strdup(coluna);
        }
        if(i==posValor){
            elemento.valor = strdup(coluna);
        }
        coluna = strtok(NULL,",");
    }
    free(coluna);
    free(linha);   
}

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

void copiaElemento(Elemento &dest, Elemento &source){
    dest.chave = source.chave;
    dest.valor = source.valor;
}

void ordenacaoExterna(ofstream &fout, int nLinhasArquivos, int n){
    
    int nDispositivos = nLinhasArquivos/n+1;
    string buffer;
    
    ifstream* dispositivos = new ifstream[nDispositivos];
    Elemento* elementos = new Elemento[nDispositivos];
    char*linha;
    Elemento* menor;
    int posmenor;
    char* maiorValor = new char[2];
    maiorValor[0]=(char)255;
    maiorValor[1]='\0';
    int *hasLine = new int[nDispositivos];
    // Abre os arquivos e copia para um vetor auxiliar a primeira linha de cada arquivo
    for(int i=0;i<nDispositivos;i++){
        // Abre os arquivos
        char *nome = new char[20];
        sprintf(nome, "%d.txt", i);
        dispositivos[i].open(nome);
        delete[]nome;

        //
        hasLine[i]=n;
        // 
        getline(dispositivos[i],buffer);
        linha = (char *)buffer.c_str();
        obterElemento(linha,0,1,elementos[i]);
        //cout<<elementos[i].chave<<" "<<elementos[i].valor<<endl;
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
                //cout<<j<<": "<<elementos[j].chave<<"<"<<menor->chave<<endl;
                if(strcmp(elementos[j].chave,menor->chave)<0){
                    menor = &elementos[j];
                    posmenor=j;
                    break;
                }
                /*
                posmenor = j;
                break;
                */
            }
            menor = &elementos[posmenor];
        }
        
        fout<<menor->chave<<","<<menor->valor<<endl;
        hasLine[posmenor]--;
        
        getline(dispositivos[posmenor],buffer);
        linha = (char *)buffer.c_str();
        obterElemento(linha,0,1,elementos[posmenor]);
        menor = &elementos[posmenor];
        
    }
    
    /*
    for(int i=0;true;i++){
        // Acha o menor vetor que ainda tem linha
        for(int j=0;j<nDispositivos;j++){
            if(hasLine[j]){
                menor = &elementos[j];
            }
        }
        // achar o menor elemento
        for(int j=0;j<nDispositivos;j++){
            if(hasLine[j]){
                if(strcmp(elementos[j].chave,menor->chave)<0){
                    menor = &elementos[j];
                    posmenor = j;
                    //cout<<elementos[j].chave<<" "<<menor->chave<<endl;
                }
            }
        }

        // Escreve-o na linha
        fout<<menor->chave<<","<<menor->valor<<endl;

        // Lê a próxima linha
        getline(dispositivos[posmenor],buffer);
        linha = strdup(buffer.c_str());
        cout<<linha<<endl;
        if(strlen(linha)==0){
            hasLine[posmenor] = false;
        }else{            
            obterElemento(linha,0,1,*menor);
        }
        free(linha);
        
    }
    */
    
    delete[]maiorValor;
    
    delete[]dispositivos;
    delete[]elementos;
}

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
            chave = e.chave;
            cont++;
        }else{  
            media = soma/cont;
            cout << fixed << chave << ", " << setprecision(15) << media << endl;
            soma = 0.0;
            soma +=stold(e.valor);
            chave = e.chave;
            cont = 1;
        }
    }
    media = soma/cont;
    cout << fixed << chave << ", " << setprecision(15) << media << endl;
    free(e.chave);
    free(e.valor);
    
//-----------------------------------------------------

}


// Salvar arquivos ordenados

int main(int argc, char **argv){
    char *fileIn = argv[1];
    int n = atoi(argv[2]);
    const char *chave = argv[3];
    const char *valor = argv[4];
    ifstream file(fileIn);
    ifstream ordenado("ordenado.txt");
    ofstream fout("ordenado.txt");
    int posChave,posValor;
    saberQualColunaEstaChaveEValor(file,(char*)chave,(char*)valor,posChave,posValor);
    int nLinhas = ordenarESepararEmArquivos(file,n,posChave,posValor);
    ordenacaoExterna(fout,nLinhas,n);
    calculaMedia(ordenado);
    ordenado.close();
    fout.close();
    file.close();
    return 0;
}