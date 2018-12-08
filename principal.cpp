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


struct Personagem{
    Ponto pos;
    int vV,vH;
};


struct Vertice{
    Ponto pos;
    Vertice *prox;
};

struct Jogo{
    Personagem *Pacman;
    Retangulo *listaR;
    Vertice *listaV;
    char matriz [19][21];
};



void adiciona_vertice(char matriz[][21],int i, int j){
    Vertice *v = new Vertice;

}

void desenha_tela(Jogo * jogo, Tela t){
    Cor azul = {0.2, 0.3, 0.8};
    Cor amarelo = {0.9, 0.9, 0.0};
    t.cor(azul);

    t.limpa();
    Retangulo * r = jogo->listaR;
    while(r != nullptr){
        t.retangulo(*r);
        r = r->prox;
    }
    Circulo *c = new Circulo;
    c->centro.x = jogo->Pacman->pos.x;
    c->centro.y = jogo->Pacman->pos.y;
    c->raio = 10;
    t.cor(amarelo);
    t.circulo(*c);
    t.mostra();
    return;
}

void adiciona_retangulo(Jogo * jogo, int i, int j){
    Retangulo *r = new Retangulo;
    r->pos.x = j*20;
    r->pos.y = i*20;
    r->prox = jogo->listaR;
    r->tam.alt = 20;
    r->tam.larg = 20;
    jogo->listaR = r;
    return;
}
void inicia_personagem(Personagem *Pacman){
    Pacman->pos.x = 10;
    Pacman->pos.y = 10;
    Pacman->vH = 0;
    Pacman->vV = 0;
    return;
}

void preenche_matriz(Jogo * jogo){
    ifstream arq;
    string mapa;
    arq.open("mapa.txt");
    if(!arq){
        cout << "não abriu o arquivo\n";
        exit(1);
    }
    for(int i = 0; i < 19; i++){
        getline(arq, mapa);
        for(int j = 0; j < 21; j++){
            jogo->matriz[i][j] = mapa[j];
            if(mapa[j] == 0)
                adiciona_vertice(jogo->matriz,i,j);
            if(mapa[j] == '1'){
                adiciona_retangulo(jogo,i,j);
            }
        }
    }
}

int main(int argc, char **argv) {
    Tela t;
    Jogo *jogo = new Jogo;
    jogo->listaR = nullptr;
    jogo->Pacman = new Personagem;
    inicia_personagem(jogo->Pacman);
    t.inicia(420, 380, "janela teste");
    Vertice * listaV = nullptr;
    preenche_matriz(jogo);
    desenha_tela(jogo,t);
    system("pause");

  return 0;
}
