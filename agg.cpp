#include <iostream>
#include <fstream>

    
using namespace std;
    
int main(){
    int n = 10;
    ifstream fin("notas.txt");
    ofstream fout("notas2.txt");

    fin >> n;

    float *notas = new float[n];

    for (int i = 0; i < n; i++){
        fin >> notas[i];
    }
    float mx =0;

    for (int i = 0; i < n; i++){
        mx = max(mx, notas[i]);
    }
    
    for (int i = 0; i < n; i++){
        fout << 100 * notas[i] / mx << endl;
    }

    fin.close();
    fout.close();
    delete []notas;
        
        
    return 0;
}