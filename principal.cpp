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

#define VELOCIDADE_PACMAN 1
#define RAIO_PACMAN 10

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
    int ultima_tecla;
    char matriz [19][21];
};


void adiciona_vertice(char matriz[][21],int i, int j){
    Vertice *v = new Vertice;

}

void desenha_tela(Jogo * jogo, Tela t){
    Cor preto = {1, 1, 1};
    Cor azul = {0.2, 0.3, 0.8};
    Cor amarelo = {0.9, 0.9, 0.0};

    t.limpa();

    Retangulo fundo_preto;
    fundo_preto.pos = {0, 0};
    fundo_preto.tam = {420, 380};
    t.retangulo(fundo_preto);


    t.cor(azul);
    Retangulo * r = jogo->listaR;
    while(r != nullptr){
        t.retangulo(*r);
        r = r->prox;
    }


    Circulo *c = new Circulo;
    c->centro.x = jogo->Pacman->pos.x;
    c->centro.y = jogo->Pacman->pos.y;
    c->raio = RAIO_PACMAN;
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
void movimenta_personagem(Jogo * jogo, Tela t){
    int tecla = t.tecla();
    if(tecla != 0)
        jogo->ultima_tecla = tecla;

    switch(jogo->ultima_tecla){
        case 4:
        case 83:
            jogo->Pacman->vH = VELOCIDADE_PACMAN;
            break;
        case 1:
        case 82:
            jogo->Pacman->vH = VELOCIDADE_PACMAN * -1;
            break;
        case 23:
        case 84:
            jogo->Pacman->vV = VELOCIDADE_PACMAN * -1;
            break;
        case 19:
        case 85:
            jogo->Pacman->vV = VELOCIDADE_PACMAN;
            break;
    }

    int Cima;
    int Baixo;
    int Direita = (jogo->Pacman->pos.x+9)/20;;
    int Esquerda = (jogo->Pacman->pos.x-9)/20;
    if(jogo->Pacman->vV > 0){
            Baixo = (jogo->Pacman->pos.y+VELOCIDADE_PACMAN+9)/20;
            if(jogo->matriz[Baixo][Direita] == '0' && jogo->matriz[Baixo][Esquerda] == '0'
               && jogo->Pacman->pos.y < 380 - RAIO_PACMAN)
                jogo->Pacman->pos.y += VELOCIDADE_PACMAN;

    }else if(jogo->Pacman->vV < 0){
            Cima = (int)(jogo->Pacman->pos.y-VELOCIDADE_PACMAN-10)/20;
            if(jogo->matriz[Cima][Direita] == '0' && jogo->matriz[Cima][Esquerda] == '0'
               && jogo->Pacman->pos.y > 0 + RAIO_PACMAN)
                jogo->Pacman->pos.y -= VELOCIDADE_PACMAN;

    }
    Cima = (jogo->Pacman->pos.y - 9)/20;
    Baixo = (jogo->Pacman->pos.y + 9)/20;
    if(jogo->Pacman->vH > 0){
            Direita = (jogo->Pacman->pos.x+10)/20;
            if(jogo->matriz[Cima][Direita] == '0' && jogo->matriz[Baixo][Direita] == '0'
               && jogo->Pacman->pos.x < 420 - RAIO_PACMAN)
                jogo->Pacman->pos.x += VELOCIDADE_PACMAN;
    }else if(jogo->Pacman->vH < 0){
            Esquerda = (int)(jogo->Pacman->pos.x-11)/20;
            if(jogo->matriz[Cima][Esquerda] == '0' && jogo->matriz[Baixo][Esquerda] == '0'
               && jogo->Pacman->pos.x > 0 + RAIO_PACMAN)
                jogo->Pacman->pos.x -= VELOCIDADE_PACMAN;
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

    while(1){ // Atualiza acontecimentos do jogo
        desenha_tela(jogo, t);

        movimenta_personagem(jogo, t);

        t.espera(12);
    }

    return 0;
}
