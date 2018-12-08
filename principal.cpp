#include <chrono>
#include <list>
#include <random>
#include <vector>
#include <iostream>
#include <functional>
#include "Tela.hpp"
#include "geom.hpp"
#include <stdio.h>
#include <iostream>
#include <fstream> //para acesso a  arquivos
#include <cstdlib> //para usar o exit


using namespace tela;
using namespace geom;
using namespace std;

void cria_vertice(int matriz[19][21],int i, int j){

    if(i != 0){
        if(matriz[i-1][j] == '0'){
            //aresta horizontal
        }
    }
    if(j != 0){
        if(matriz[i][j-1] == '0'){
            //aresta vertical;
        }
    }
}

void desenha_tela(Retangulo *r, Tela *t){
    while(r != nullptr){
        t.retangulo(*r);
    }
    return;
}


int main(int argc, char **argv) {
    Tela t;
    t.inicia(600, 400, "janela teste");
    ifstream arq;
    string mapa;
    char matriz [19][21];
    arq.open("mapa.txt");
    if(!arq){
        cout << "não abriu o arquivo\n";
        exit(1);
    }
    Retangulo * lista = nullptr;

    for(int i = 0; i < 19; i++){
        getline(arq, mapa);
        cout << mapa;
        for(int j = 0; j < 21; j++){
            matriz[i][j] = mapa[j];
            if(mapa[j] == 0)
                cria_vertice(matriz,i,j);
            if(mapa[j] == 1){
                //listaretangulos
            }
        }

    }

  return 0;
}
