typedef struct viz {
	int id;
	int custo;
	struct viz *prox_viz;	
}TViz;

typedef struct grafo {
	int id_grafo;
	TViz *viz;
	struct grafo *prox;
}TG;

TG *inicializa(void);
TG *insereNo(TG *g, int no);
TG *buscaNo(TG *g, int no);
void retiraNo(TG *g, int no);
void insereAresta(TG *g, int no1, int no2);
TViz *buscaAresta(TG *g, int no1, int no2);
void retiraAresta(TG *g, int no1, int no2);
void imprimeGrafo(TG *g);
void imprimeNo(TG *g);
void imprimeAresta(TG *g);

