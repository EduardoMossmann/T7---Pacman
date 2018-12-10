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
#include <map>

#define VELOCIDADE_PACMAN 1
#define RAIO_PACMAN 10

using namespace tela;
using namespace geom;
using namespace std;


struct Personagem{
    Ponto pos;
    int vV,vH,f;
};


struct Vertice{
    int cor;
    Ponto pos;
    list<int> adj;
};

struct Jogo{
    Personagem *Pacman;
    Retangulo *listaR;
    map<int, Vertice> grafo;
    int ultima_tecla;
    char matriz [19][21];
    Circulo *listaP;
    int pontos;
};

void desenha_tela(Jogo * jogo, Tela t){
    Cor branco = {1, 1, 1};
    Cor azul = {0.2, 0.3, 0.8};
    Cor amarelo = {0.9, 0.9, 0.0};
    Cor preto = {0,0,0};

    t.limpa();
    t.cor(preto);
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

    t.cor(branco);
    Circulo *c = jogo->listaP;
    while(c != nullptr){
        t.circulo(*c);
        c = c->prox;
    }


    Circulo *c2 = new Circulo;
    c2->centro.x = jogo->Pacman->pos.x;
    c2->centro.y = jogo->Pacman->pos.y;
    c2->raio = RAIO_PACMAN;
    t.cor(amarelo);
    t.circulo(*c2);



    t.mostra();
    return;
}

void adiciona_vertice(Jogo * jogo, int i, int j, int nCol){
    Vertice v;
    v.cor = 0;
    v.pos.x = j * 20;
    v.pos.y = i * 20;
    jogo->grafo.insert ( pair<int,Vertice>( i*nCol + j, v) );
}
void cria_adjacencia(Jogo * jogo){
    for(auto i = jogo->grafo.begin(); i != jogo->grafo.end(); i++){
        for(auto j = jogo->grafo.begin(); j != jogo->grafo.end(); j++){
            if(( (i->second.pos.x == j->second.pos.x + 20 || i->second.pos.x == j->second.pos.x - 20)
                && i->second.pos.y == j->second.pos.y )
               ||
               ( (i->second.pos.y == j->second.pos.y + 20 || i->second.pos.y == j->second.pos.y - 20)
                && i->second.pos.x == j->second.pos.x )
            ){
                i->second.adj.push_back(j->first);
            }
        }
    }
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
void adiciona_circulo(Jogo * jogo, int i, int j){
    Circulo *c = new Circulo;
    c->centro.x = (j*20)+RAIO_PACMAN;
    c->centro.y = (i*20)+RAIO_PACMAN;
    c->raio = 2;
    c->prox = jogo->listaP;
    jogo->listaP = c;
    return;
}

void inicia_personagem(Personagem *Pacman){
    Pacman->pos.x = 10;
    Pacman->pos.y = 10;
    Pacman->vH = 0;
    Pacman->vV = 0;
    Pacman->f = 0;
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
            if(mapa[j] == '0'){
                adiciona_vertice(jogo, i, j, 21);
                adiciona_circulo(jogo,i,j);
            }if(mapa[j] == '1'){
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
            if(jogo->Pacman->f == 2)
                jogo->Pacman->vV = 0;
            break;
        case 1:
        case 82:
            jogo->Pacman->vH = VELOCIDADE_PACMAN * -1;
            if(jogo->Pacman->f == 2)
                jogo->Pacman->vV = 0;
            break;
        case 23:
        case 84:
            jogo->Pacman->vV = VELOCIDADE_PACMAN * -1;
            if(jogo->Pacman->f == 1)
                jogo->Pacman->vH = 0;
            break;
        case 19:
        case 85:
            jogo->Pacman->vV = VELOCIDADE_PACMAN;
            if(jogo->Pacman->f == 1)
                jogo->Pacman->vH = 0;
            break;
    }

    int Cima;
    int Baixo;
    int Direita = (jogo->Pacman->pos.x+9)/20;;
    int Esquerda = (jogo->Pacman->pos.x-9)/20;
    if(jogo->Pacman->vV > 0){
            Baixo = (jogo->Pacman->pos.y+VELOCIDADE_PACMAN+9)/20;
            if(jogo->matriz[Baixo][Direita] == '0' && jogo->matriz[Baixo][Esquerda] == '0'
               && jogo->Pacman->pos.y < 380 - RAIO_PACMAN){
                jogo->Pacman->pos.y += VELOCIDADE_PACMAN;
                jogo->Pacman->f=1;
               }
    }else if(jogo->Pacman->vV < 0){
            Cima = (int)(jogo->Pacman->pos.y-VELOCIDADE_PACMAN-10)/20;
            if(jogo->matriz[Cima][Direita] == '0' && jogo->matriz[Cima][Esquerda] == '0'
               && jogo->Pacman->pos.y > 0 + RAIO_PACMAN){
                jogo->Pacman->pos.y -= VELOCIDADE_PACMAN;
               jogo->Pacman->f=1;
               }

    }
    Cima = (jogo->Pacman->pos.y - 9)/20;
    Baixo = (jogo->Pacman->pos.y + 9)/20;
    if(jogo->Pacman->vH > 0){
            Direita = (jogo->Pacman->pos.x+10)/20;
            if(jogo->matriz[Cima][Direita] == '0' && jogo->matriz[Baixo][Direita] == '0'
               && jogo->Pacman->pos.x < 420 - RAIO_PACMAN){
                jogo->Pacman->pos.x += VELOCIDADE_PACMAN;
                jogo->Pacman->f=2;
               }

    }else if(jogo->Pacman->vH < 0){
            Esquerda = (int)(jogo->Pacman->pos.x-11)/20;
            if(jogo->matriz[Cima][Esquerda] == '0' && jogo->matriz[Baixo][Esquerda] == '0'
               && jogo->Pacman->pos.x > 0 + RAIO_PACMAN){
                jogo->Pacman->pos.x -= VELOCIDADE_PACMAN;
                jogo->Pacman->f=2;
               }
    }
}
void conta_pontos(Jogo * jogo, Tela t){
     if(jogo->listaP != nullptr){
            int Direita = (jogo->Pacman->pos.x+9);
            int Esquerda = (jogo->Pacman->pos.x-9);
            int Cima = (jogo->Pacman->pos.y-9);
            int Baixo = (jogo->Pacman->pos.y+9);
            Circulo *c = jogo->listaP;
            Circulo *prox = jogo->listaP->prox;
            Circulo *aux;
            if(c->centro.x < Direita && c->centro.x > Esquerda && c->centro.y < Baixo && c->centro.y > Cima){
                jogo->listaP = prox;
                delete[] c;

            }
            while(prox != nullptr){
                if(prox->centro.x < Direita && prox->centro.x > Esquerda && prox->centro.y < Baixo && prox->centro.y > Cima){
                    aux = prox->prox;
                    delete[] prox;
                    c->prox = aux;
                    jogo->pontos++;
                    break;
                }
                prox = prox->prox;
                c = c->prox;
            }
    }
}
/*void menor_caminho (Jogo * Jogo, Vertice * origem, Vertice * destino){
    Lista * lista = cria_lista();
    adiciona_item(lista, origem);

    Item * item = lista->primeiro_item;
    while (item!=NULL){

        int cont = 0;
        Vertice ** vertices_destino = grafo_busca_vertices_saida(grafo, item->vertice, &cont);
        int i = 0;
        for (i=0;i<cont;i++){
            Item * item_ = busca_item(lista,  vertices_destino[i]);
            if (item_==NULL){
               item_ = adiciona_item(lista, vertices_destino[i]);
               item_->precedente = item;
            }
            if (vertices_destino[i] == destino){
                printf("Menor Caminho: ");
                exibe_menor_caminho(item_);
                return;
            }
        }
    item = item->prox_item;
    }
    printf("Nao existe caminho entre %s e %s", grafo_retorna_nome(origem), grafo_retorna_nome(destino));

}
*/
int main(int argc, char **argv) {
    Tela t;
    Jogo *jogo = new Jogo;
    jogo->pontos = 0;
    jogo->listaR = nullptr;
    jogo->listaP = nullptr;
    jogo->Pacman = new Personagem;
    inicia_personagem(jogo->Pacman);
    t.inicia(420, 380, "janela teste");
    preenche_matriz(jogo);
    cria_adjacencia(jogo);

    while(1){ // Atualiza acontecimentos do jogo
        desenha_tela(jogo, t);

        movimenta_personagem(jogo, t);

        conta_pontos(jogo, t);

        t.espera(12);
    }

    return 0;
}
