#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define VALORUNITARIO 4.50
#define MAXSTRINGCLI 100
#define ICMS 0.18
#define IPI 0.04
#define PIS 0.0186
#define COFINS 0.0854


/* Mesmo C não sendo a linguagem ideal para realizar o exercício optei por ela porque é a linguagem que mais tenho experiência, 
	pois já realizei 3 cadeiras de algoritimos em C na faculdade*/


struct CARGA{
	char cliente[MAXSTRINGCLI];
	int quantidade;
	double icms;
	double ipi;
	double pis;
	double cofins;
	double totalImpostos;
	double valorMercadoria;
	double valorComImpostos;
};
int verificaInt(int *x){ // função para digitar um inteiro de forma segura
	if(scanf("%d", x) == 0){ // testa se foi digitado um inteiro
		while(fgetc(stdin)!='\n'); // se não foi limpa a ultima coisa digitadada e retorna 1
		return 1;
	}else{
		if(*x <= 0){ // se o numero for 0 ou menor retorna 2
			return 2;
		}else{
			return 0; // retorna 0 se for lido um inteiro corretamente
		}
	}	
}
int leString(char *s, int max){ // função um pouco mais segura pra ler string
	int i;
	do {
		fgets(s, max, stdin);
		for (i = 0; s[i]; i++);
		if (s[i - 1] == '\n') { // se no final da string tiver um /n ele é retirado
			s[--i] = 0;
		}
	} while (i < 1); // não sai do laço enquanto algo não for digitado
	return (i); // retorna o tamanho da string
}
int errosInt(int *x){ // função apenas para imprimir os erros da função verificaInt na tela
	int erro;
	while((erro = verificaInt(x)) != 0){
		if(erro == 1){
			printf("Voce digitou um valor invalido\n");
		}else{
			printf("Voce digitou um valor negativo\n");
		}
		printf("Digite novamente (Apenas Numeros Positivos)\n");
	}
}
int calcImpostos(struct CARGA *venda){ // função que calcula os impostos de uma carga 
	(*venda).icms = (*venda).valorMercadoria * ICMS;
	(*venda).ipi = (*venda).valorMercadoria * IPI;
	(*venda).pis = (*venda).valorMercadoria * PIS;
	(*venda).cofins = (*venda).valorMercadoria * COFINS;
	(*venda).totalImpostos = (*venda).icms + (*venda).ipi + (*venda).pis + (*venda).cofins;
	(*venda).valorComImpostos = (*venda).totalImpostos + (*venda).valorMercadoria;
}

 // Função do desafio
 
double desconto(int qtd, double *valor){ /* Função que aplica o desconto crescente em uma carga (Struct CARGA) dependendo da quantidade comprada com no maxino 25% de desconto possivel.
											A função retornara sempre o valor do desconto aplicado.*/				
	double desconto;
	if(qtd > 500){ // Aplica desconto apenas em cargas com mais de 500 unidades compradas
		desconto = (qtd - 500) * 0.05;
		if(desconto > 25){ // Se o desconto for maior que 25% ele irá aplicar o desconto de 25% apenas
			*valor -= *valor * 0.25;
			return 25;
		}
		*valor -= *valor * (desconto / 100);
		return desconto;
	}else{ // Se a carga não for maior que 500 retornara o valor 0
		return 0; 
	}
	 
}
int main(){
	int qtdCli, i, houveDesconto = 0;
	double totalDeMercadorias, totalDeImpostos, totalComDescontos, valorDesconto;
	char escolha;
	struct CARGA *vendas;
	printf("Quantos clientes serao cadastrados?\n");
	errosInt(&qtdCli);
	vendas = (struct CARGA *)malloc(sizeof(struct CARGA) * qtdCli); // criando um vetor dinamicamente
	while(vendas == NULL){ // testando se foi cricado com sucesso se não for ele tentará criar um vetor com o maior número possivel
		qtdCli--;
		vendas = (struct CARGA *)malloc(sizeof(struct CARGA) * qtdCli); 
		printf("Tentando alocar %d Clientes\r", qtdCli);
		if(qtdCli == 0){
			printf("Nao foi possivel alocar\n");
			fprintf(stderr,"ERRO DE MEMORIA\n");
			free(vendas);
			return 3; // se não conseguir alocar nenhuma carga encerra o programa retornando 3
		}
		if(vendas != NULL){
			printf("Foi possivel alocar apenas %d\n", qtdCli);
		}
	}
	for(i = 0; i < qtdCli; i++){ // fazendo a digitação e computação dos dados do cliente
		printf("Digite o nome do Cliente %d\n", i + 1);
		leString(vendas[i].cliente, MAXSTRINGCLI);
		printf("Digite a quantidade vendida para %s\n", vendas[i].cliente);
		errosInt(&vendas[i].quantidade); // chama errosInt para se acaso algo for digitado erroneamente os erros já serão apontados
		vendas[i].valorMercadoria = vendas[i].quantidade * VALORUNITARIO;
		calcImpostos(&vendas[i]); // faz os calculos dos impostos da carga no indice em questão
		totalDeMercadorias += vendas[i].valorMercadoria;
		totalDeImpostos += vendas[i].totalImpostos;	
	}
	for(i = 0; i < qtdCli; i++){ // printa na tela as informações de cada cliente
		printf("\nCliente: %s\n", vendas[i].cliente);
		printf("ICMS: R$ %.2lf\n", vendas[i].icms);
		printf("IPI: R$ %.2lf\n", vendas[i].ipi);
		printf("PIS: R$ %.2lf\n", vendas[i].pis);
		printf("COFINS: R$ %.2lf\n", vendas[i].cofins);
		printf("Total: R$ %.2lf\n", vendas[i].valorComImpostos);
	}
	 // printa as informações totais na tela
	printf("\nTotal Impostos: R$ %.2lf\n", totalDeImpostos);
	printf("Total Mercadorias: R$ %.2lf\n", totalDeMercadorias);
	printf("Total Geral: R$ %.2lf\n\n", totalDeImpostos + totalDeMercadorias);
	
	 // Ddaqui para baixo é apenas a parte do desafio
	 
	printf("Voce deseja aplicar os possiveis descontos? (s/n)\n"); // como o desafio não é obtigatorio no projeto pergunta se os descontos querem ser aplicados ou não
	escolha = tolower(getch());
	while(escolha != 's' && escolha != 'n'){ // não deixa colocar um caracter a não ser 's' ou 'n'
		printf("Caracter invalido\n");
		printf("Voce deseja aplicar os possiveis descontos?\n");
		printf("Digite 's' para sim e 'n' para nao\n");
		escolha = tolower(getch());
	}
	if(escolha == 's'){ // só aplica os descontos se o usuario digitou que queria ver os descontos
		for(i = 0; i < qtdCli; i++){
			valorDesconto = desconto(vendas[i].quantidade, &vendas[i].valorComImpostos); // chama a função que aplica os descontos
			if(valorDesconto != 0){ // printa apenas os clientes que obtiveram desonto
				printf("\nCliente %s obteve desconto de %.2lf%%\n", vendas[i].cliente, valorDesconto);
				printf("Total com impostos e descontos: R$ %.2lf\n", vendas[i].valorComImpostos);
				houveDesconto++; // conta quantos clientes obtiveram desconto
				totalComDescontos += vendas[i].valorComImpostos;
			}else{
				continue;
			}		
		}
		if(houveDesconto > 0){ // printa na tela apenas se algum cilente obteve desconto
			printf("\nHouve um total de %d clientes que obtiveram desconto\n", houveDesconto);
			printf("Total Geral com desconto: R$ %.2lf\n", totalComDescontos);
		}else{ // informa se nenhum cliente obteve desconto
			printf("Nenhum cliente teve desconto\n");
		}
	}
	free(vendas); // desaloca o vetor vendas
	return 0;
}

