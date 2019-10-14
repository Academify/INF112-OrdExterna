#include <iostream>
#include <fstream>
#include <stdlib.h>
    
using namespace std;
    
int main(int argc, char **argv){

    //Pega os valores da linha de comando

    char *fileIn = argv[1];
    int m = atoi(argv[2]);
    char *chave = argv[3];
    char *valor = argv[4];

    ifstream fin(fileIn);
    ofstream fout("fileOut");



    // Fecha os aquivos

    fin.close();
    fout.close();

    return 0;
}
