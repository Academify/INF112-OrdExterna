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

//void gravarArquivo(char *nome, Elemento *&elementos, int n);

void gravarArquivo2(Elemento *&elementos,  int n, const char *nome);

// ---------------------------------------------------------------------------------------------------------------------------------------

/*
void ordena(int nDispositivos, int nLinhasArquivos, int n){
    
    ifstream *dispositivos = new ifstream[nDispositivos];
    bool *semLinha = (bool*)calloc(nDispositivos,sizeof(bool));
    Elemento* elementos = new Elemento[nDispositivos];
    string buffer;
    char*linha;
    char*final;
    char*numero;
    char*nome = new char[20];
    
    ofstream resultado;
    resultado.open("resultado.txt");

    //Roda só uma vez
    for(int i = 0; i < nDispositivos; i++){
        sprintf(nome, "%d.txt", i);
        dispositivos[i].open(nome);
    }
    
    
    for(int i=0;i<nDispositivos;i++){
        getline(dispositivos[i],buffer);
        linha = (char*)buffer.c_str();
        elementos[i].chave = strtok_r(linha,",",&final);
        elementos[i].valor = atoi(final);
    
    }
    // Achar o primeiro não nulo
    Elemento menor = elementos[0];
    for(int i=1;i<nDispositivos;i++){
        if(strcmp(elementos[i].chave,menor.chave)<0){
            menor = elementos[i];
        }
    }
    resultado << menor.chave << ',' << menor.valor << endl;
    cout<<menor;



}
*/

void ordena(int nDispositivos, int nLinhasArquivos, int n){
    string buffer;
    ifstream* dispositivos = new ifstream[nDispositivos];
    Elemento* elementos = new Elemento[nDispositivos];
    char*linha;
    char*menor;
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
        
        getColumn(0, linha, elementos[i].chave);
        char*valorTemp;
        getColumn(1, linha, valorTemp);
        elementos[i].valor = atoi(valorTemp);
    }

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
            getColumn(0, linha, elementos[pmenor].chave);
            char*valorTemp;
            getColumn(1, linha, valorTemp);
        }
    }
    

}


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

    char *chaveAux;
    char *valorAux;
    char* chavetmp;
    char* valortmp;
    Elemento *elementos = new Elemento [n];
    char *nomeArquivo;

    // Lê e ordena arquivo
    while (true)
    {
        getline(file, buffer);
        linha = (char *)buffer.c_str();
        //cout<<linha<<endl;

        getColumn(posChave, linha, chaveAux);
        getColumn(posValor, linha, valorAux);
        
        //cout << posLinha << endl;
        
        //continue;
        elementos[posLinha].chave = chaveAux;
        elementos[posLinha].valor = atoi(valorAux);

        

        if (posLinha == n - 1 || file.eof()){
            quickSort(elementos, n);
            //cout << "Aqui" << endl;        
            //int aux = 5 + log10(nLinhasArquivo+1/n);
            //out<<aux;
            nomeArquivo = new char[20];
            sprintf(nomeArquivo,"%d.txt",nLinhasArquivo/n);
            
            // Ordena linhas e salva ordenado num arquivo
            //gravarArquivo(nomeArquivo, elementos, posLinha+1);

            /*
            for (int i = 0; i < posLinha + 1; i++){
               cout << elementos[i].chave << ":" << elementos[i].valor << endl;
            }
             */

            gravarArquivo2(elementos, posLinha+1, nomeArquivo);
            
            posLinha = -1;
            if(file.eof())
                break;
        }
        nLinhasArquivo++;
        posLinha++;
    }

    ordena(nLinhasArquivo/n+1,nLinhasArquivo,n);
    return 0;
    // Intercalação
    int nDispositivos = nLinhasArquivo/n+1;
    ifstream* dispositivos = new ifstream[nDispositivos];
    Elemento *linhasElementos = new Elemento[nDispositivos];

    bool* isEmpty = new bool[nDispositivos];
    char *chaveTemp;
    char *valorTemp;

    // Abre os arquivos temporarios
    for(int i=0;i<nDispositivos;i++){
        char *nome = new char[20];
        sprintf(nome, "%d.txt", i);
        dispositivos[i].open(nome);
        isEmpty[i] = false;
        getline(dispositivos[i], buffer);
        linha = (char *)buffer.c_str();
        getColumn(0, linha, chaveTemp);
        getColumn(1, linha, valorTemp);
        linhasElementos[i].chave=chaveTemp;
        linhasElementos[i].valor = atoi(valorTemp);
    }
    
    // Salva as linhas ordenadas em um arquivo
    ofstream fout("ordenado.txt");
    for(int i=0;i<nLinhasArquivo+1;i++){
        int posicaoMenor;
        char*menor;
        string buffer;
        char*linha;
        // char *chave;
        // char *valor;

        // Supor o menor valor sendo o primeiro que encontrar
        for(int j=0;j<nDispositivos;j++){
            if(!dispositivos[j].eof()){
                posicaoMenor = j;
                menor = new char[strlen(linhasElementos[j].chave)+1];
                strcpy(menor, linhasElementos[j].chave);
                break;
            }
        }
        
        for (int k = 0; k < nDispositivos; k++){
            if (dispositivos[k].eof()){
                continue;
            }
            
            if (strcmp(linhasElementos[k].chave, menor) < 0)
            {
                strcpy(menor, linhasElementos[k].chave);
                posicaoMenor = k;
                //cout << menor;
                //cout << linhasElementos[k].valor<<endl;
            }
        }
        cout<<linhasElementos[posicaoMenor].chave<<endl;
        
        getline(dispositivos[posicaoMenor], buffer);
        linha = (char *)buffer.c_str();
        getColumn(0, linha, chaveTemp);
        getColumn(1, linha, valorTemp);
        linhasElementos[posicaoMenor].chave = chaveTemp;
        linhasElementos[posicaoMenor].valor = atoi(valorTemp); 
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
    while (linha[fim] != ',' && linha[fim] != '\0' && linha[fim]!='\n')
    {
        fim++;
    }
    int tam = fim - inicio;
    palavra = new char[tam+1];
    //cout<<inicio+1<<" "<<tam-1<<" ";
    strncpy(palavra, linha + inicio + 1, tam - 1);
    palavra[tam-1]='\0';
    //cout<<palavra<<endl;
}

/* void gravarArquivo(char *nome, Elemento *&elementos, int n){
    //cout<<n<<endl;
    ofstream fout(nome);
    for (int i = 0; i < n; i++){
        fout << elementos[i].chave << "," << elementos[i].valor << endl;
        //cout << elementos[i].chave << "," << elementos[i].valor << endl;
        fout.flush();
    }  
} */

void gravarArquivo2( Elemento *&elementos, int n, const char *nome){
    ofstream fout(nome);
    for (int i = 0; i < n-1; i++){
        fout << elementos[i].chave << ","<< elementos[i].valor << endl;
    }
    int i = n-1;
    fout << elementos[i].chave << ","<< elementos[i].valor;
    fout.close();
}

