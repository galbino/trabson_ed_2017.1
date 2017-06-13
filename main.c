#include "Grafos.h"
#include <stdio.h>

int menu(void);
void opcao(int opcao);

int main(void){
	int escolha;
	TG *grafo = inicializa();
	do
	{
		escolha = menu();
		opcao(escolha, grafo);
	}while(escolha);
	
	return 0;
}

int menu(){
	int opt;
	printf("\n---MENU---\n");
	printf("0 - sair\n");
	printf("1 - exibir\n");
	printf("2 - adcionar nó\n");
	printf("3 - retirar nó\n");
	printf("4 - buscar nó\n");	
	printf("5 - adcionar aresta\n");
	printf("6 - retirar aresta\n");	
	printf("7 - busca aresta\n");
	printf("8 - carrega arquivo\n");	
}

void opcao(int opcao, TG grafo){
	int info;
	int destino;
	int custo;	
	
	switch(opcao){
		case 0:
			printf("programa finalizado\n");
			break;
		case 1:
			imprime(grafo);
			break;
		case 2:
			printf("\n\nAdicionar nó:\n \tdigite a informção do nó: ");
			scanf("%d", &info);
			insereNo(grafo, info);
			break;
		case 3:
			printf("\n\nRetirar nó:\n \tdigite a informação do nó: ");
			scanf("%d", &info);
			retiraNo(grafo, info);
			break;
		case 4:
			prinf("\n\nBuscar nó:\n \tdigite a informação do nó: ");
			scanf("%d", info);
			buscaNo(grafo, info);
			break;
		case 5:
			printf("\n\nAdicionar aresta:\n \tdigite o nó de onde sai a aresta: ");
			scanf("%d", &info);
			if (!buscaNo(grafo, info)){
				printf("\n\nDesculpe, o nó digitado não foi encontrado =/");
				break;
			}
			printf("\n \tdigite o nó para onde a aresta aponta: ");
			scanf("%d", &destino);
			if(!buscaNo(grafo, destino)){
				printf("\n\nDesculpe, o nó digitado não foi encontrado =/");
				break;
			}
			printf("\n \tdigite o custo da aresta: ");
			scanf("%d",&custo);
			insereAresta(grafo, info, destino);
			break;
		case 6:
			printf("\n\nRetira aresta:\n \tdigita o nó de onde sai a aresta: ");
			scanf("%d", &info);
			if(!buscaNo(grafo, info)){
				printf("\n\nDesculpe, o nó digitado não foi encontrado =/");
				break;
			}
			printf("\n \tdigite o nó para onde a aresta aponta: ");
			scanf("%d",&destino);
			if(!buscaNo(grafo, destino)){
				printf("\n\nDesculpe, o nó digitado não foi encontrado =/");
				break;
			}
			retiraAresta(grafo, info, destino);
			break;
						
	}
}





















