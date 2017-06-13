#include "grafos.h"

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
