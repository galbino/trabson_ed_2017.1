#include "grafos.h"
#include <stdlib.h>
#include <stdio.h>

TG *buscaNo (TG *g, int no) {
	if (!g) return NULL;
	TG *p = g;
	while (p && p->id_grafo != no) {
		p = p->prox;
	}
	return p;
}
TG *insereNo(TG *g, int no) {
	TG *p = buscaNo(g, no);
	if (p) return g;
	p = (TG *) malloc(sizeof(TG));
	p->id_grafo = no;
	p->cor = 0;
	p->viz = NULL;
	p->ant = NULL;
	p->prox = g;
	if (g) g->ant = p;
	resetaCores(g);
	return p;
}
TG *retiraNo(TG *g, int no){
	TG *p = buscaNo(g, no);
	if (!p) return g;
	if (p->ant) p->ant->prox = p->prox;
	else g = p->prox;
	if (p->prox) p->prox->ant = p->ant;
	liberaNo(g, p);
	resetaCores(g);
	return g;
}
TViz *buscaAresta(TG *g, int no1, int no2) {
	TG *p = buscaNo(g, no1);
	if (!p) return NULL;
	TViz *viz = p->viz;
	while (viz && viz->id != no2) {
		viz = viz->prox_viz;
	}
	return viz;
}
void insereAresta(TG *g, int no1, int no2){
	TViz *t = buscaAresta(g, no1, no2);
	if (t) return;
	TG *p = buscaNo(g, no1);
	if (!p) return;
	TViz *novaAresta = (TViz *) malloc (sizeof(TViz));
	novaAresta->id = no2;
	novaAresta->prox_viz = p->viz;
	novaAresta->viz_ant = NULL;
	novaAresta->id_pai = p->id_grafo;
	
	novaAresta->jaPassou = 0;
	
	if (p->viz) p->viz->viz_ant = novaAresta;
	p->viz = novaAresta;
	resetaCores(g);
}
void insereArestaAmbosSentidos(TG *g, int no1, int no2){
	insereAresta(g, no1, no2);
	insereAresta(g, no2, no1);
}
void retiraAresta(TG *g, int no1, int no2) {
	TG *p = buscaNo(g, no1);
	TViz *ar = buscaAresta(g, no1, no2);
	if(!ar) return;
	if (ar->viz_ant) ar->viz_ant->prox_viz = ar->prox_viz;
	else p->viz = ar->prox_viz;
	if (ar->prox_viz) ar->prox_viz->viz_ant = ar->viz_ant;
	free(ar);
	resetaCores(g);
}
void removeArestaConectada(TG *g, int no) {
	if (!g) return;
	retiraAresta(g, g->id_grafo, no);
	removeArestaConectada(g->prox, no);
}
void retiraArestaAmbosSentidos(TG *g, int no1, int no2){
	retiraAresta(g, no1, no2);
	retiraAresta(g, no2, no1);
}
void imprimeAresta(TViz *ar){
	printf("\t%d -> %d\n", ar->id_pai, ar->id);
}
void imprimeNo(TG *g) {
	if (!g) return;
	TViz *ar = g->viz;
	printf("No: %d \tCor: %d\n", g->id_grafo, g->cor);
	if(!ar){
		printf("Este no nao possui arestas\n");
	}
	while (ar){
		imprimeAresta(ar);
		ar = ar->prox_viz;
	}
}
void imprimeGrafo(TG *g){
	TG *p = g;
	while (p) {
		imprimeNo(p);
		p = p->prox;
	}
}
void liberaNo(TG *g, TG *p) {
	TViz *ant = p->viz, *prox;
	if (!ant) {
		removeArestaConectada(g, p->id_grafo);
	} else {
		while (ant) {
		prox = ant->prox_viz;
		retiraArestaAmbosSentidos(g, p->id_grafo, ant->id);
		ant = prox;
		}
	}
	free(p);
}
void libera(TG *g){
	if(!g) return;	
	retiraNo(g, g->id_grafo);
	libera(g->prox);
}

TG *copia(TG *g) {
	TG *q = g;
	TG *p = NULL;
	while (q) {
		p = insereNo(p, q->id_grafo);
		q = q->prox;
	}
	q = g;
	while (q){
		TViz *viz = q->viz;
		while (viz) {
			insereAresta(p, q->id_grafo, viz->id);
			viz = viz->prox_viz;
		}
		q = q->prox;
	}
	return p;
}

TG *pintaGrafo(TG *grafo, TG *grafoAux, int qtdNos){
	if(!grafo){
		return grafo;
	}
	if(grafoAux){
		int i;
		for(i = 1; i <= qtdNos + 1; i++){
			grafoAux->cor = i;
			if(semConflito(grafo, grafoAux, i)){
				return pintaGrafo(grafo, grafoAux->prox, qtdNos);
			}
			grafoAux->cor = 0;
		}
	} else {
		imprimeGrafo(grafo);
		printf("\nGrafo pintado:");
		return grafo;
	}
}

int semConflito(TG *grafoInicio, TG *grafo, int cor){
	TViz *aresta = grafo->viz;
	while(aresta){
		if (aresta->id != grafo->id_grafo){ // Evita nós que estão apontando pra si mesmo
			TG *noAux = buscaNo(grafoInicio, aresta->id);
			if (noAux && noAux->cor == cor){
				return 0;
			}
		}
		aresta = aresta->prox_viz;
	}
	TG *grafoAux = grafo;
	while(grafoAux){
		TViz *aresta = grafoAux->viz;
		while(aresta){
			if(aresta->id == grafo->id_grafo && grafoAux->cor == grafo->cor){
				return 0;
			}
			aresta = aresta->prox_viz;
		}
		grafoAux = grafoAux->ant;
	}
	
	return 1;
}

int contaNos(TG *grafo){
	TG *grafoAux = grafo;
	int qtdNos;
	while(grafoAux){
		qtdNos ++;
		grafoAux = grafoAux->prox;
	}
	return qtdNos;	
}

int bothWays(TG *grafoInicio, TG *grafo){
	if (!grafo){
		return 1; //sim, todos os arcos vão e voltam;
	}
	TViz *aresta = grafo->viz;
	while(aresta){
		if(!buscaAresta(grafoInicio, aresta->id, aresta->id_pai)){
			return 0;
		}
		aresta = aresta->prox_viz;
	}
	return bothWays(grafoInicio, grafo->prox);
}

int procuraCaminho(TG *grafoInicio, TG *grafo, int destino, int resp){
	if(grafo->id_grafo == destino){
		return 1;
	}
	TViz *aresta = grafo->viz;
	while(aresta){
		if(!aresta->jaPassou){
			aresta->jaPassou = 1;
			resp = procuraCaminho(grafoInicio, buscaNo(grafoInicio, aresta->id), destino, resp);
		}
		aresta = aresta->prox_viz;
	}
	return resp;
}

void pintarGrafoDesconexos(TG *grafoInicio){
	TG *grafoPercorre = grafoInicio;
	TG *grafoAux = grafoInicio;
	resetaCores(grafoInicio);
	resetaCaminho(grafoInicio);
	
	int cor = 1;
	while(grafoAux){
		
		grafoPercorre = grafoInicio;
		int achou = 0;
		while(grafoPercorre){
			if(grafoPercorre->cor){
				if(procuraCaminho(grafoInicio, grafoPercorre, grafoAux->id_grafo, 0)) {
					grafoAux->cor = grafoPercorre->cor;
					achou = 1;
					resetaCaminho(grafoInicio);
				}
			}				
			grafoPercorre = grafoPercorre->prox;
		}
		if(!achou){
			grafoAux->cor = cor;
			cor++;
		}
		
		grafoAux = grafoAux->prox;
	}
}

void resetaCores(TG *grafoInicio){
	TG *grafoAux = grafoInicio;
	while(grafoAux){
		grafoAux->cor = 0;
		grafoAux = grafoAux->prox;
	}
}

void resetaCaminho(TG *grafoInicio){
	TG *grafoAux = grafoInicio;
	while(grafoAux){
		TViz *aresta = grafoAux->viz;
		while(aresta){
			aresta->jaPassou = 0;
			aresta = aresta->prox_viz;
		}
		grafoAux = grafoAux->prox;
	}
}


int fortementeConexo(TG *g) {
	TG *p = g;
	while (p) {
		TG *q = p->prox;
		while (q) {
			if (!procuraCaminho(g, p, q->id_grafo, 0)) {
				resetaCaminho(g);
				return 0;
			}
			resetaCaminho(g);
			q = q->prox;
		}
		p = p->prox;
	}
	return 1;
}

int verificarConectividade(TG *g) {
	int maior = 0;
	pintarGrafoDesconexos(g);
	TG *p = g;
	while (p) {
		if (maior < p->cor) {
			maior = p->cor;
		}
		p = p->prox;
	}
	return maior;
}
void encontrarPontoArticulacao(TG *g) {
	TG *p = copia(g);
	TG *q = g;
	TViz *viz = NULL;
	while (p && q) {
		p = retiraNo(p, q->id_grafo);
		int t = verificarConectividade(p);
		if (t != 1) printf("O no %d eh um ponto de articulacao.\n", q->id_grafo);
		p = insereNo(p, q->id_grafo);
		viz = q->viz;
		while (viz) {
			insereArestaAmbosSentidos(p, q->id_grafo, viz->id);
			viz = viz->prox_viz;
		}
		q = q->prox;
	}
	libera(p);
}
void encontrarPontes(TG *g) {
	TG *q = copia(g);
	TG *p = g;
	TViz *viz = NULL, *vizP = NULL;
	int id1, id2;
	while(p){
		viz = p->viz;
		while (viz) {
			id1 = p->id_grafo;
			id2 = viz->id;
			retiraArestaAmbosSentidos(q, id1, id2);
			if (verificarConectividade(q) > 1) printf("A aresta %d -> %d eh uma ponte.\n", id1, id2);
			insereArestaAmbosSentidos(q, id1, id2);
			viz = viz->prox_viz;
		}
		p = p->prox;
	}
	libera(q);
}









