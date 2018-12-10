#include <chrono>
#include <list>
#include <random>
#include <vector>
#include <iostream>
#include <functional>
#include "Tela.hpp"
#include "geom.hpp"
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <fstream> //para acesso a  arquivos
#include <cstdlib> //para usar o exit
#include <map>
#include <queue>

#define VELOCIDADE_PACMAN 1
#define RAIO_PACMAN 10

#define BRANCO 0
#define CINZA 1
#define PRETO 2

using namespace tela;
using namespace geom;
using namespace std;


struct Personagem{
    Ponto pos;
    int vV,vH, f;
    int objetivo;
};

struct Vertice{
    int cor;
    Ponto pos;
    int distancia;
    list<int> adj;
    Vertice * ant;
};


struct Jogo{
    Personagem *Pacman;
    Personagem *Fantasma;
    Personagem *Fantasma2;
    Personagem *Fantasma3;
    Retangulo *listaR;
    map<int, Vertice> grafo;
    int ultima_tecla;
    char matriz [19][21];
    Circulo *listaP;
    int pontos;
    int quantFantasmas;
};

void desenha_tela(Jogo * jogo, Tela t){
    Cor branco = {1, 1, 1};
    Cor azul = {0.2, 0.3, 0.8};
    Cor amarelo = {0.9, 0.9, 0.0};
    Cor preto = {0,0,0};
    Cor cinza = {0.2,0.2,0.2};
    Cor fantasma = {0.6,0.7,0.2};
    Cor fantasma2 = {0.6,0.1,0.7};
    Cor fantasma3 = {0.2,0.5,0.9};

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

    t.cor(cinza);
    Retangulo *r2 = new Retangulo;
    r2->pos.x = 0;
    r2->pos.y = 380;
    r2->tam.alt = 60;
    r2->tam.larg = 420;
    t.retangulo(*r2);


    Circulo *c2 = new Circulo;
    c2->centro.x = jogo->Pacman->pos.x;
    c2->centro.y = jogo->Pacman->pos.y;
    c2->raio = RAIO_PACMAN;
    t.cor(amarelo);
    t.circulo(*c2);

    Circulo *c3 = new Circulo;
    c3->centro.x = jogo->Fantasma->pos.x;
    c3->centro.y = jogo->Fantasma->pos.y;
    c3->raio = RAIO_PACMAN;
    t.cor(fantasma);
    t.circulo(*c3);


    Circulo *c4 = new Circulo;
    if(jogo->quantFantasmas>1){

        c4->centro.x = jogo->Fantasma2->pos.x;
        c4->centro.y = jogo->Fantasma2->pos.y;
        c4->raio = RAIO_PACMAN;
        t.cor(fantasma2);
        t.circulo(*c4);
    }
    Circulo *c5 = new Circulo;

    if(jogo->quantFantasmas>1){

        c5->centro.x = jogo->Fantasma3->pos.x;
        c5->centro.y = jogo->Fantasma3->pos.y;
        c5->raio = RAIO_PACMAN;
        t.cor(fantasma3);
        t.circulo(*c5);
    }


    Ponto *p = new Ponto;
    p->x = 150;
    p->y = 410;
    string pont = "Pontos: ";
    pont +=  to_string(jogo->pontos);
    t.cor(amarelo);
    t.texto(*p,pont.c_str());

    t.mostra();

    delete[] r;
    delete[] c;
    delete[] r2;
    delete[] p;
    delete[] c2;
    delete[] c3;
    delete[] c4;
    delete[] c5;

    return;
}

void adiciona_vertice(Jogo * jogo, int i, int j, int nCol){
    Vertice v;
    v.cor = 0;
    v.distancia = 0;
    v.pos.x = j * 20;
    v.pos.y = i * 20;
    v.ant = nullptr;
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
/*
void inicializa_vertices_branco(map<int, Vertice> grafo){
    for (auto& v: grafo){
        v.second.cor = BRANCO;
        v.second.distancia = 0;
    }
}

void grafo_busca_largura(map<int, Vertice> grafo, Vertice * s){
    queue<Vertice*> f;
    inicializa_vertices_branco(grafo);
    f.push(s);

    grafo[136].cor = PRETO;
    while(f.empty() == false){
        Vertice * u = f.front();
        f.pop();
        for (auto& v: u->adj){
            if(grafo[v].cor == 0){
                grafo[v].cor = CINZA;
                grafo[v].distancia = u->distancia + 1;
                grafo[v].ant = u;
                f.push(&grafo[v]);
            }
        }
        u->cor = PRETO;
    }
}
int grafo_retorna_chave(map<int, Vertice> grafo, Vertice * v){
    for(auto i = grafo.begin(); i != grafo.end(); i++){
        if(&i->second == v){
            printf("%d", i->first);
            return i->first;
        }
    }
}

int caminho_curto(map<int, Vertice> grafo, int fonte, int destino){
    Vertice * s = &grafo[fonte];
    Vertice * v = &grafo[destino];

    if(s == v){
        printf("Incidencia");
        return 1;
    }
    if(v->ant == NULL){
        printf("Nao existe caminho");
    } else {
        caminho_curto(grafo, fonte, grafo_retorna_chave(grafo, v->ant));
    }
}


int descobre_vertice(Personagem * p, map<int, Vertice> grafo){
    int coluna = p->pos.x / 20;
    int linha = p->pos.y / 20;
    return linha*21 + coluna;
}
int descobre_linha(int x){
    return x/21;
}
int descobre_coluna(int x){
    return x%21;
}



/*

void movimenta_fantasma(Jogo * jogo, Personagem * fantasma){
    if(fantasma->objetivo == -1){
        int v_fantasma = descobre_vertice(fantasma, jogo->grafo);
        grafo_busca_largura(jogo->grafo, &jogo->grafo[descobre_vertice(jogo->Pacman, jogo->grafo)]);
        fantasma->objetivo = caminho_curto(jogo->grafo, descobre_vertice(fantasma, jogo->grafo), descobre_vertice(jogo->Pacman, jogo->grafo));

        int colD = descobre_coluna(fantasma->objetivo);
        int linD = descobre_linha(fantasma->objetivo);
        int colA = descobre_coluna(v_fantasma);
        int linA = descobre_linha(v_fantasma);
        colD = colD - colA;
        linD = linD - linA;
        fantasma->vH = colD;
        fantasma->vV = linD;
    }else{
        if(fantasma->pos.x % 20 - 10 + fantasma->vH == 0 && fantasma->pos.y % 20 - 10 + fantasma->vV == 0){
            fantasma->objetivo = -1 ;
        }
            fantasma->pos.x += fantasma->vH;
            fantasma->pos.y += fantasma->vV;

    }

}
//if(fantasma->objetivo == -1){
        int v_fantasma = descobre_vertice(fantasma, jogo->grafo);
        auto i = jogo->grafo[v_fantasma];
	
        
        
        fantasma->objetivo = 1;

	int v_pacman = descobre_vertice(jogo->Pacman, jogo->grafo);
	int colP = descobre_coluna(v_pacman);
	int linP = descobre_linha(v_pacman);


        int colA = descobre_coluna(v_fantasma);

        int linA = descobre_linha(v_fantasma);
	
	if(colP > colA && jogo->matriz[linA][colA+1] == 0){
		fantasma->vH = 1;
		fantasma->vH = 1;
	}else if(colP < colA && jogo->matriz[linA][colA-1] == 0){
		fantasma->vH = -1;
		fantasma->vH = 1;
	}else if(linP > linA && jogo->matriz[linA+1][colA] == 0){
		fantasma->vV = 1;
		fantasma->vH = 1;
	}else if(linP < linA && jogo->matriz[linA-1][colA] == 0){
		fantasma->vV = -1;]
	
	fantasma->pos.x += fantasma->vH;
        fantasma->pos.y += fantasma->vV;  
/*
	int colD = descobre_coluna(fantasma->objetivo);
        int linD = descobre_linha(fantasma->objetivo);


	

        colD = colD - colA;
        linD = linD - linA;
        fantasma->vH = colD;
        fantasma->vV = linD;
*/
    /*}else{
        if(fantasma->pos.x % 20 - 10 + fantasma->vH == 0 && fantasma->pos.y % 20 - 10 + fantasma->vV == 0){
            fantasma->objetivo = -1 ;

        }
	fantasma->pos.x += fantasma->vH;
        fantasma->pos.y += fantasma->vV;  

    }*/





int descobre_vertice(Personagem * p, map<int, Vertice> grafo){
    int coluna = p->pos.x / 20;
    int linha = p->pos.y / 20;

    return linha*21 + coluna;
}
int descobre_linha(int x){
    return x/21;
}
int descobre_coluna(int x){
    return x%21;
}

void movimenta_fantasma(Jogo * jogo, Personagem * fantasma){
    if(fantasma->objetivo == -1){
        int v_fantasma = descobre_vertice(fantasma, jogo->grafo);
        auto i = jogo->grafo[v_fantasma];
	
        
        
        fantasma->objetivo = 1;

	int v_pacman = descobre_vertice(jogo->Pacman, jogo->grafo);
	int colP = descobre_coluna(v_pacman);
	int linP = descobre_linha(v_pacman);

        int colA = descobre_coluna(v_fantasma);
        int linA = descobre_linha(v_fantasma);
	
	if(colP > colA && jogo->matriz[linA][colA+1] == '0'){
		fantasma->vH = 1;
		fantasma->vV = 0;
	}else if(colP < colA && jogo->matriz[linA][colA-1] == '0'){
		fantasma->vH = -1;
		fantasma->vV = 0;
	}else if(linP > linA && jogo->matriz[linA+1][colA] == '0'){
		fantasma->vV = 1;
		fantasma->vH = 0;
	}else if(linP < linA && jogo->matriz[linA-1][colA] == '0'){
		fantasma->vV = -1;
		fantasma->vH = 0;
	}else if(colP < colA && jogo->matriz[linA][colA-1] == '1'){
		fantasma->vV = 1;
		fantasma->vH = 0;
	}else if(colP > colA && jogo->matriz[linA][colA+1] == '1'){
		fantasma->vV = 0;
		fantasma->vH = -1;
	}else if(linP > linA && jogo->matriz[linA+1][colA] == '1'){
		fantasma->vV = 0;
		fantasma->vH = -1;
	}else if(linP < linA && jogo->matriz[linA-1][colA] == '1'){
		fantasma->vV = 0;
		fantasma->vH = 1;
	}else{
		fantasma->vV = 0;
		fantasma->vH = 0;
	}


	
	
/*
	int colD = descobre_coluna(fantasma->objetivo);
        int linD = descobre_linha(fantasma->objetivo);


	

        colD = colD - colA;
        linD = linD - linA;
        fantasma->vH = colD;
        fantasma->vV = linD;
*/
    }else{
        if(fantasma->pos.x % 20 - 10 + fantasma->vH == 0 && fantasma->pos.y % 20 - 10 + fantasma->vV == 0){
            fantasma->objetivo = -1 ;

        }
            fantasma->pos.x += fantasma->vH;
            fantasma->pos.y += fantasma->vV;

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
void adiciona_circulo(Jogo * jogo, int i, int j, int raio){
    Circulo *c = new Circulo;
    c->centro.x = (j*20)+RAIO_PACMAN;
    c->centro.y = (i*20)+RAIO_PACMAN;
    c->raio = raio;
    c->prox = jogo->listaP;
    jogo->listaP = c;
    return;
}

void inicia_pacman(Personagem *Pacman){
    Pacman->pos.x = 10;
    Pacman->pos.y = 10;
    Pacman->vH = 0;
    Pacman->vV = 0;
    Pacman->f = 0;
    return;
}
void inicia_fantasma(Personagem * Fantasma){
    Fantasma->pos.x = 210;
    Fantasma->pos.y = 130;
    Fantasma->vH = 0;
    Fantasma->vV = 0;
    Fantasma->f = 0;
    Fantasma->objetivo = -1;
    return;
}

void preenche_matriz(Jogo * jogo){
    ifstream arq;
    string mapa;
    arq.open("mapa.txt");
    if(!arq){
        cout << "nÃ£o abriu o arquivo\n";
        exit(1);
    }
    for(int i = 0; i < 19; i++){
        getline(arq, mapa);
        for(int j = 0; j < 21; j++){
            jogo->matriz[i][j] = mapa[j];
            if(mapa[j] == '0'){
                adiciona_vertice(jogo, i, j, 21);
                if(j == 10 && i == 10){
                    adiciona_circulo(jogo,i,j,8);
                }else{
                    adiciona_circulo(jogo,i,j,2);
                }

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
                jogo->pontos++;
            }
            while(prox != nullptr){
                if(prox->centro.x < Direita && prox->centro.x > Esquerda && prox->centro.y < Baixo && prox->centro.y > Cima){
                    jogo->pontos++;
                    aux = prox->prox;
                    delete[] prox;
                    c->prox = aux;
                    break;
                }
                prox = prox->prox;
                c = c->prox;
            }
    }else{
        t.finaliza();
        cout << "Parabens, voce ganhou \nPontuacao: " << jogo->pontos;
        exit(1);
    }
}
void testa_colisao(Jogo * jogo, Tela t){
    int perdeu = 0;
    if(descobre_vertice(jogo->Fantasma, jogo->grafo) == descobre_vertice(jogo->Pacman, jogo->grafo)){
        perdeu = 1;
    }
    if(jogo->quantFantasmas > 1){
        if(descobre_vertice(jogo->Fantasma2, jogo->grafo) == descobre_vertice(jogo->Pacman, jogo->grafo))
                perdeu = 1;
        if(jogo->quantFantasmas > 2){
            if(descobre_vertice(jogo->Fantasma3, jogo->grafo) == descobre_vertice(jogo->Pacman, jogo->grafo))
                perdeu = 1;
        }
    }
    if(perdeu == 1){
        printf("\n\nVoce Perdeu");
        exit(1);
    }
}

int main(int argc, char **argv) {
    srand(time(NULL));
    Tela t;
    Jogo *jogo = new Jogo;
    jogo->pontos = 0;
    jogo->listaR = nullptr;
    jogo->listaP = nullptr;
    jogo->Pacman = new Personagem;
    jogo->Fantasma = new Personagem;
    jogo->Fantasma2 = new Personagem;
    jogo->Fantasma3 = new Personagem;
    jogo->quantFantasmas = 1;
    inicia_pacman(jogo->Pacman);
    inicia_fantasma(jogo->Fantasma);
    t.inicia(420, 440, "janela teste");
    preenche_matriz(jogo);
    cria_adjacencia(jogo);


    cout << jogo->grafo[0].adj.size();
    while(1){ // Atualiza acontecimentos do jogo
        desenha_tela(jogo, t);

        movimenta_personagem(jogo, t);

        conta_pontos(jogo, t);

        movimenta_fantasma(jogo, jogo->Fantasma);

        if(jogo->pontos > 30 && jogo->quantFantasmas == 1){
            inicia_fantasma(jogo->Fantasma2);
            jogo->quantFantasmas++;
        }

        if(jogo->pontos > 100 && jogo->quantFantasmas == 2){
            inicia_fantasma(jogo->Fantasma3);
            jogo->quantFantasmas++;
        }

        if(jogo->quantFantasmas > 1){
            movimenta_fantasma(jogo, jogo->Fantasma2);
        }
        if(jogo->quantFantasmas > 2){
            movimenta_fantasma(jogo, jogo->Fantasma3);
        }


        testa_colisao(jogo, t);

        t.espera(14);
    }

    return 0;
}
