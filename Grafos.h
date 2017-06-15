typedef struct viz {
	int id;
	int id_pai;
	struct viz *prox_viz;
	struct viz *viz_ant;
}TViz;

typedef struct grafo {
	int id_grafo;
	TViz *viz;
	struct grafo *prox;
	struct grafo *ant;
}TG;

TG *buscaNo(TG *g, int no);
TG *insereNo(TG *g, int no);
TG *retiraNo(TG *g, int no);
TViz *buscaAresta(TG *g, int no1, int no2);	
void insereAresta(TG *g, int custo, int no1, int no2);
void retiraAresta(TG *g, int no1, int no2);
void retiraArestaAmbosSentidos(TG *g, int no1, int no2);
void imprimeAresta(TViz *aresta);
void imprimeNo(TG *g);
void imprimeGrafo(TG *g);
void liberaNo(TG *g, TG *p);
void libera(TG *g);
