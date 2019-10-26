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

            for(int i=0;i<aux+1;i++){
                cout<<elementos[i].chave<<",";
                cout<<elementos[i].valor<<endl;
            }
            cout<<endl;
            // Ordena vetor
            quickSort(elementos,aux);
            for(int i=0;i<aux+1;i++){
                cout<<elementos[i].chave<<",";
                cout<<elementos[i].valor<<endl;
            }
            cout<<endl<<endl;
            //
            char *nomeArquivo = new char[20];
            sprintf(nomeArquivo,"%d.txt",nLinhas/n);
            fout.open(nomeArquivo);
            for(int i=0;i<aux+1;i++){
                fout<<elementos[i].chave<<",";
                fout<<elementos[i].valor<<endl;
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

void ordenacaoExterna(int nLinhasArquivos, int n){
    int nDispositivos = nLinhasArquivos/n+1;
    string buffer;
    ifstream* dispositivos = new ifstream[nDispositivos];
    Elemento* elementos = new Elemento[nDispositivos];
    char*linha;
    char*menor=NULL;
    int pmenor;
    bool *hasLine = new bool[nDispositivos];
    // Abre os arquivos
    for(int i=0;i<nDispositivos;i++){
        hasLine[i]=true;
        char *nome = new char[20];
        sprintf(nome, "%d.txt", i);
        dispositivos[i].open(nome);
        getline(dispositivos[i],buffer);
        linha = (char *)buffer.c_str();
        obterElemento(linha,0,1,elementos[i]);
        if(menor==NULL){
            menor = strdup(elementos[i].chave);
            pmenor = i;
        }
        if(strcmp(elementos[i].chave,menor)<0){
            menor = strdup(elementos[i].chave);
            pmenor = i;
        }
        //cout<<elementos[i].chave<<" "<<elementos[i].valor<<endl;       
    }
    for(int i=0;i<nLinhasArquivos;i++){
        
        if(hasLine){
            getline(dispositivos[pmenor],buffer);
            linha = (char *)buffer.c_str();
            obterElemento(linha,0,1,elementos[pmenor]);
            menor = strdup(elementos[i].chave);
            if(dispositivos[pmenor].peek()==EOF){
                hasLine[pmenor]=false;
            }
        }
        for(int j=0;j<nDispositivos;j++){
            if(hasLine[j]){
                cout<<elementos[j].chave<<" "<<menor<<endl;
                if(strcmp(elementos[j].chave,menor)<0){
                    menor = strdup(elementos[j].chave);
                    pmenor = j;
                }
            }
        }
    }


    /*

    // Acima ta tudo correto
    for(int i=0;i<nLinhasArquivos;i++){

        // Achar o menor não nulo
        for(int j=0;j<nDispositivos;j++){
            if(hasLine[j]){
                menor = elementos[j].chave;
                pmenor = j;
                break;
            }
        }
        
        // Achar o menor
        for(int j=0;j<nDispositivos;j++){
            if(hasLine[j]){
                if(strcmp(elementos[j].chave,menor)<0){
                    menor = elementos[j].chave;
                    pmenor = j;
                }
            }
        }

        cout<<elementos[pmenor].chave<<",";
        cout<<elementos[pmenor].valor<<endl;

        if(dispositivos[pmenor].peek()==-1){
            hasLine[pmenor]=false;
        }else{
            getline(dispositivos[pmenor],buffer);
            linha = (char *)buffer.c_str();
            obterElemento(linha,0,1,elementos[i]);
        }

    }
    */
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
    int nLinhas = ordenarESepararEmArquivos(file,n,posChave,posValor);
    //ordenacaoExterna(nLinhas,n);
    file.close();
    return 0;
}