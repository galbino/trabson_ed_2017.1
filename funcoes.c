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
	return p;
}
TG *retiraNo(TG *g, int no){
	TG *p = buscaNo(g, no);
	if (!p) return g;
	if (p->ant) p->ant->prox = p->prox;
	if (p->prox) p->prox->ant = p->ant;
	liberaNo(g, p);
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
	if (p->viz) p->viz->viz_ant = novaAresta;
	p->viz = novaAresta;
}
void retiraAresta(TG *g, int no1, int no2) {
	TG *p = buscaNo(g, no1);
	TViz *ar = buscaAresta(g, no1, no2);
	if(!ar) return;
	if (ar->viz_ant) ar->viz_ant->prox_viz = ar->prox_viz;
	else p->viz = ar->prox_viz;
	if (ar->prox_viz) ar->prox_viz->viz_ant = ar->viz_ant;
	free(ar);
}
void retiraArestaAmbosSentidos(TG *g, int no1, int no2){
	printf("%d %d\n", no1, no2);
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
	while (ant->prox_viz) {
		prox = ant->prox_viz;
		retiraArestaAmbosSentidos(g, p->id_grafo, ant->id);
		ant = prox;
	}
	retiraArestaAmbosSentidos(g, p->id_grafo, ant->id);
	free(p);
}
void libera(TG *g){
	if(!g) return;	
	retiraNo(g, g->id_grafo);
	libera(g->prox);
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


















