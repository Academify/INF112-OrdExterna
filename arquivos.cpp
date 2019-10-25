#include<iostream>
#include<string>
#include<cstring>
#include<fstream>
using namespace std;

int main(){
    int nLinhasArquivo = 11;
    int nDispositivos = 4;
    ifstream* dispositivos = new ifstream[nDispositivos];
    string buffer;
    //Elemento *elementos = new Elemento[nDispositivos];
    bool flag;
    char**linhas = new char*[nDispositivos];
    char**chaves = new char*[nDispositivos];
    char*menor;
    int pmenor;
    bool *hasLine = new bool[nDispositivos];

    for(int i=0;i<nDispositivos;i++){
        hasLine[i]=true;
        char *nome = new char[20];
        sprintf(nome, "%d.txt", i);
        dispositivos[i].open(nome);
        getline(dispositivos[i],buffer);
        linhas[i] = new char[strlen(buffer.c_str())];
        strcpy(linhas[i],buffer.c_str());        
        chaves[i] = new char[strlen(linhas[i])];
        strcpy(chaves[i],linhas[i]);
        chaves[i] = strtok(chaves[i],",");
    }

    menor = new char[300];
    for(int j=0;j<nLinhasArquivo;j++){
        menor[0]='\0';
        for(int i=0;i<nDispositivos;i++){
            if(!hasLine[i]){
                continue;
            }
            
            if(strcmp(chaves[i],menor)<0){
                strcpy(menor,chaves[i]);
                pmenor = i;
            }
    
        }
        cout<<pmenor<<endl;

    }


    return 0;
}