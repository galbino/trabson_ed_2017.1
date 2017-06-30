#include "grafos.h"
#include <stdio.h>

int menu(TG *grafo);
TG *opcao(int opcao, TG *grafo);
TG *carregaArquivo(TG *grafo);

int main(void){
	int escolha;
	TG *grafo = NULL;
	grafo = carregaArquivo(grafo);
	if (bothWays(grafo, grafo)) {
		printf("\nO grafo e nao orientado\n");
	} else {
		printf("\nO grafo e orientado\n");
	}	
	do {
		pintarGrafoDesconexos(grafo);
		escolha = menu(grafo);
		grafo = opcao(escolha, grafo);
	} while(escolha);
	
	return 0;
}

int menu(TG *grafo){
	int opt;
	printf("\n---MENU---\n");
	printf("0 - Sair\n");
	printf("1 - Exibir\n");
	printf("2 - Adicionar no\n");
	printf("3 - Retirar no\n");
	printf("4 - Buscar no\n");	
	printf("5 - Adicionar aresta\n");
	printf("6 - Retirar aresta\n");	
	printf("7 - Busca aresta\n");
	printf("8 - Eh orientado?\n");
	if (!bothWays(grafo, grafo)) {
		printf("9 - Eh fortemente conexo?\n");
	} else {
		printf("9 - Eh conexo?\n");
		if (verificarConectividade(grafo) == 1) {
		 	printf("10 - Pontes\n");
		 	printf("11 - Articulacoes\n");
		}
	}
	printf("----------\nSelecione uma opcao\n");
	scanf("%d", &opt);
	return opt;
}

TG *opcao(int opcao, TG *grafo){
	int info;
	int destino;
	
	switch(opcao){
		case 0:
			printf("programa finalizado\n");
			break;
		case 1:
			imprimeGrafo(grafo);
			break;
		case 2:
			printf("\n\nAdicionar no:\n \tdigite a informcao do no: ");
			scanf("%d", &info);
			grafo = insereNo(grafo, info);
			break;
		case 3:
			printf("\n\nRetirar no:\n \tdigite a informacao do no: ");
			scanf("%d", &info);
			grafo = retiraNo(grafo, info);
			break;
		case 4:
			printf("\n\nBuscar no:\n \tdigite a informacao do no: ");
			scanf("%d", &info);
			if(buscaNo(grafo, info)){
				imprimeNo(buscaNo(grafo, info));
			}else{
				printf("\n\nNo nao existe \n");
			}
			break;
		case 5:
			printf("\n\nAdicionar aresta:\n \tdigite o no de onde sai a aresta: ");
			scanf("%d", &info);
			if (!buscaNo(grafo, info)){
				printf("\n\nDesculpe, o no digitado nao foi encontrado =/");
				break;
			}
			printf("\n \tdigite o no para onde a aresta aponta: ");
			scanf("%d", &destino);
			if(!buscaNo(grafo, destino)){
				printf("\n\nDesculpe, o no digitado nao foi encontrado =/");
				break;
			}
			insereAresta(grafo, info, destino);
			break;
		case 6:
			printf("\n\nRetira aresta:\n \tdigita o no de onde sai a aresta: ");
			scanf("%d", &info);
			if(!buscaNo(grafo, info)){
				printf("\n\nDesculpe, o no digitado nao foi encontrado =/");
				break;
			}
			printf("\n \tdigite o nó para onde a aresta aponta: ");
			scanf("%d",&destino);
			if(!buscaNo(grafo, destino)){
				printf("\n\nDesculpe, o no digitado nao foi encontrado =/");
				break;
			}
			retiraAresta(grafo, info, destino);
			break;
		case 7:
			printf("\n\nBusca aresta aresta:\n \tdigita o no de onde sai a aresta: ");
			scanf("%d", &info);
			if(!buscaNo(grafo, info)){
				printf("\n\nDesculpe, o no digitado nao foi encontrado =/");
				break;
			}
			printf("\n \tdigite o no para onde a aresta aponta: ");
			scanf("%d",&destino);
			if(!buscaNo(grafo, destino)){
				printf("\n\nDesculpe, o no digitado nao foi encontrado =/");
				break;
			}
			imprimeAresta(buscaAresta(grafo, info, destino));
			break;	
		case 8:
			if (bothWays(grafo, grafo)) {
				printf("\nO grafo e nao orientado\n");
			} else {
				printf("\nO grafo e orientado\n");
			}
			break;
		case 9:
			if (!bothWays(grafo, grafo)) {
				if (fortementeConexo(grafo)){
					printf("\nO grafo inteiro e fortemente conexo: \n");
					encontrarPontosFortementeConexos(grafo);
					printf("\n");
				} else {
					printf("\nO grafo inteiro nao e fortemente conexo: \n");
					encontrarPontosFortementeConexos(grafo);
					printf("\n");
				}
			} else {
				if (verificarConectividade(grafo) == 1) {
					printf("\nO grafo e conexo!");
				} else {
					printf("\nO grafo nao e conexo!");
				}
			}
			break;
		case 10:
			if (bothWays(grafo, grafo) && verificarConectividade(grafo) == 1) {
				encontrarPontes(grafo);
				break;
			}
		case 11:
			if (bothWays(grafo, grafo) && verificarConectividade(grafo) == 1) {
				encontrarPontoArticulacao(grafo);
				break;
			}
		default:
			printf("\nOpcao nao encontrada");
			break;
	}
	return grafo;
}

TG *carregaArquivo(TG *grafo){
	FILE *arq = fopen("exemplo_EDI.txt", "r");
	
	if (arq != NULL){
		int qtdNo;
		fscanf(arq,"%d", &qtdNo);
		printf("Carregando arquivo:\n");
		printf("\nQuantidade de nos: %d\n", qtdNo);
		int i;
		for(i = 1; i <= qtdNo ; i++){
			grafo = insereNo(grafo, i);
		}
		
		while(!feof(arq)){
			int origem;
			int destino;
			
			fscanf(arq,"%d%d", &origem, &destino);
			printf("origem: %d ; destino: %d\n", origem, destino);
			insereAresta(grafo, origem, destino);
		}
		
		
		fclose(arq);
	} else {
		printf("\nErro ao tentar abrir o arquivo");
	}
	return grafo;
}





















