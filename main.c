#include "grafos.h"
#include <stdio.h>

int menu(void);
void opcao(int opcao, TG *grafo);
TG *carregaArquivo(void);

int main(void){
	int escolha;
	TG *grafo = NULL;
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
	printf("2 - adcionar n�\n");
	printf("3 - retirar n�\n");
	printf("4 - buscar n�\n");	
	printf("5 - adcionar aresta\n");
	printf("6 - retirar aresta\n");	
	printf("7 - busca aresta\n");
	printf("8 - carrega arquivo\n");
	printf("----\nSelecione uma op��o\n");
	scanf("%d", &opt);
	return opt;
}

void opcao(int opcao, TG *grafo){
	int info;
	int destino;
	int custo;	
	
	switch(opcao){
		case 0:
			printf("programa finalizado\n");
			break;
		case 1:
			imprimeGrafo(grafo);
			printf("\n\nPressione enter para voltar ao menu");
			getchar();
			break;
		case 2:
			printf("\n\nAdicionar n�:\n \tdigite a inform��o do n�: ");
			scanf("%d", &info);
			grafo = insereNo(grafo, info);
			break;
		case 3:
			printf("\n\nRetirar n�:\n \tdigite a informa��o do n�: ");
			scanf("%d", &info);
			grafo = retiraNo(grafo, info);
			break;
		case 4:
			printf("\n\nBuscar n�:\n \tdigite a informa��o do n�: ");
			scanf("%d", info);
			imprimeNo(buscaNo(grafo, info));
			break;
		case 5:
			printf("\n\nAdicionar aresta:\n \tdigite o n� de onde sai a aresta: ");
			scanf("%d", &info);
			if (!buscaNo(grafo, info)){
				printf("\n\nDesculpe, o n� digitado n�o foi encontrado =/");
				break;
			}
			printf("\n \tdigite o n� para onde a aresta aponta: ");
			scanf("%d", &destino);
			if(!buscaNo(grafo, destino)){
				printf("\n\nDesculpe, o n� digitado n�o foi encontrado =/");
				break;
			}
			printf("\n \tdigite o custo da aresta: ");
			scanf("%d",&custo);
			insereAresta(grafo, info, destino);
			break;
		case 6:
			printf("\n\nRetira aresta:\n \tdigita o n� de onde sai a aresta: ");
			scanf("%d", &info);
			if(!buscaNo(grafo, info)){
				printf("\n\nDesculpe, o n� digitado n�o foi encontrado =/");
				break;
			}
			printf("\n \tdigite o n� para onde a aresta aponta: ");
			scanf("%d",&destino);
			if(!buscaNo(grafo, destino)){
				printf("\n\nDesculpe, o n� digitado n�o foi encontrado =/");
				break;
			}
			retiraAresta(grafo, info, destino);
			break;
		case 7:
			printf("\n\nBusca aresta aresta:\n \tdigita o n� de onde sai a aresta: ");
			scanf("%d", &info);
			if(!buscaNo(grafo, info)){
				printf("\n\nDesculpe, o n� digitado n�o foi encontrado =/");
				break;
			}
			printf("\n \tdigite o n� para onde a aresta aponta: ");
			scanf("%d",&destino);
			if(!buscaNo(grafo, destino)){
				printf("\n\nDesculpe, o n� digitado n�o foi encontrado =/");
				break;
			}
			imprimeAresta(buscaAresta(grafo, info, destino));
			break;
		case 8:
			printf("\n\nCarregando arquivo\n");
			grafo = carregaArquivo();
			break;
		default:
			printf("\nOp��o n�o encontrada");
			break;
	}
}
TG *carregaArquivo(){
		TG *grafoAux = NULL;
		FILE *arq = fopen("exemplo_EDI.txt", "r");
		if (arq != NULL){
			int qtdNo;
			fscanf(arq,"%d", &qtdNo);
			int i;
			for(i = 1; i <= qtdNo ; i++){
				grafoAux = insereNo(grafoAux, i);
			}
			
			while(!feof(arq)){
				int origem;
				int destino;
				
				fscanf(arq,"%d %d", &origem, &destino);
				insereAresta(grafoAux, origem, destino);
			}
			
			
			fclose(arq);
		} else {
			printf("\nErro ao tentar abrir o arquivo");
		}
		
		return grafoAux;
}





















