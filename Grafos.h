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

TG *insereNo(TG *g, int no);
TG *buscaNo(TG *g, int no);
void *retiraNo(TG *g, int no);
TG *insereNo(TG *g, int no);
TViz *buscaAresta(TG *g, int no, int no2);
void *retiraAresta(TG *g, int no, int no2);

