#include<iostream>
#include<fstream>
using namespace std;

int main(){
    ofstream fout("oi.txt");

    for(int i=0;i<3;i++){
        fout<<"Deu certo?"<<endl;
    }
    fout.close();
    return 0;
}