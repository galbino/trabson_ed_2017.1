typedef struct viz {
	int id;
	int id_pai;
	struct viz *prox_viz;
	struct viz *viz_ant;
	int jaPassou;
}TViz;

typedef struct grafo {
	int id_grafo;
	int cor;
	TViz *viz;
	struct grafo *prox;
	struct grafo *ant;
}TG;

TG *buscaNo(TG *g, int no);
TG *insereNo(TG *g, int no);
TG *retiraNo(TG *g, int no);
TViz *buscaAresta(TG *g, int no1, int no2);	
void insereAresta(TG *g, int no1, int no2);
void insereArestaAmbosSentidos(TG *g, int no1, int no2);
void retiraAresta(TG *g, int no1, int no2);
void retiraArestaAmbosSentidos(TG *g, int no1, int no2);
void imprimeAresta(TViz *aresta);
void imprimeNo(TG *g);
void imprimeGrafo(TG *g);
void liberaNo(TG *g, TG *p);
void libera(TG *g);
TG *copia(TG *g);

TG *pintaGrafo(TG *grafo, TG *grafoAux, int qtdNos);
int semConflito(TG *grafoInicio, TG *grafo, int cor);
int contaNos(TG *grafo);

int bothWays(TG *grafoInicio, TG *grafo);

int procuraCaminho(TG *grafoInicio, TG *grafo, int destino, int resp);
int procuraCaminhoFortemente(TG *grafoInicio, TG *grafo, int destino, int resp);
void pintarGrafoDesconexos(TG *grafoInicio);
void resetaCores(TG *grafoInicio);
void resetaCaminho(TG *grafoInicio);

int fortementeConexo(TG *g);
int verificarConectividade(TG *g);
void encontrarPontoArticulacao(TG *g);
void encontrarPontes(TG *g);
void encontrarPontosFortementeConexos(TG *g);
