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
void retiraNo(TG *g, int no);
void insereArestaUmSentido(TG *g, int no1, int no2);
void insereAresta(TG *g, int no1, int no2);
TViz *buscaAresta(TG *g, int no1, int no2);
void retiraArestaUmSentido(TG *g, int no1, int no2);
void retiraAresta(TG *g, int no1, int no2);

