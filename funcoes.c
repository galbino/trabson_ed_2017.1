#include "grafos.h"
#include <stdlib.h>
#include <stdio.h>

TG *buscaNo (TG *g, int no) {
	if (!g) return g;
	if (g->id_grafo == no) return g;
	buscaNo(g->prox, no);
}
TG *insereNo(TG *g, int no) {
	TG *p = buscaNo(g, no);
	if (p) return g;
	p = (TG *) malloc(sizeof(TG));
	p->id_grafo = no;
	p->viz = NULL;
	p->prox = g;
	return p;
}
TG *retiraNo(TG *g, int no){
	TG *p = buscaNo(g, no);
	if (!p) return g;
	p->ant->prox = p->prox;
	p->prox->ant = p->ant;
	liberaNo(g, p);
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
	TViz *ar = buscaAresta(g, no1, no2);
	if(!ar) return;
	ar->viz_ant->prox_viz = ar->prox_viz;
	ar->prox_viz->viz_ant = ar->viz_ant;
}
void retiraArestaAmbosSentidos(TG *g, int no1, int no2){
	retiraAresta(g, no1, no2);
	retiraAresta(g, no2, no1);
}
void imprimeAresta(TViz *ar){
	printf("%d -> %d\n", ar->id_pai, ar->id);
}
void imprimeNo(TG *g) {
	TViz *ar = g->viz;
	while (g && ar){
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
	while (p && ant->prox_viz) {
		prox = ant->prox_viz;
		retiraArestaAmbosSentidos(g, p->id_grafo, ant->id);
		free(ant);
		ant = prox;
	}
	free(ant);
	free(p);
}
void libera(TG *g){
	if(!g) return;	
	retiraNo(g, g->id_grafo);
	libera(g->prox);
}
