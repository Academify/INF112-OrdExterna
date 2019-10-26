#include<iostream>
#include<fstream>
#include<cstring>
using namespace std;

int main(){
    char *t = new char[1];
    t[0]=(char)255;
    cout<<strcmp(t,"A")<<endl;
    return 0;
}