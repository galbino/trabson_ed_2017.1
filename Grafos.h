typedef struct viz {
	int id;
	struct viz *prox_viz;	
}TViz;
typedef struct grafo {
	int id_grafo;
	TViz *viz;
	struct grafo *prox;
};
