/*
	PROGRAMADORES: JHONATAN FRADE e MARLON DIETRICH
	DATA: 26/06/2018
	VERSION: 3.0.0
	BUILD: 0.0.1
*/
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define TAM 10 //Tamanho da string
#define QTDE 100 //Quantidade de cartas

typedef struct{
	char tipo;  //N: normal - S: especial - C: coringa
	char cor[TAM];
	char acao[TAM];
	int num;
	int local; //0 esta no monte - 1 esta na mao - 2 esta na pilha
}ATRIBUTOS;

typedef struct{
	ATRIBUTOS carta[QTDE];
	int quant = 0;
}BARALHO;

typedef struct{
    char nome[30];
    BARALHO mao;
    int pontos;
}USUARIO;

void menu();
void executa(int opcao);
void mostrarCartasDoArquivo();
BARALHO getBaralho();
void jogar();
FILE* abreArquivo(char modo, char caminho[30]);
void fecharArquivo(FILE *arquivo);
void criarCartas();
void criarCartasNormais(char corDaCarta[10], BARALHO *cartas);
void criarCartasEspeciais(char acaoDaCarta[10], BARALHO *cartas);
void criarCartasCoringa(char acaoDaCarta[10], BARALHO *cartas);
void comprarCartas(USUARIO *jogador, BARALHO *monte, int qtde);
void mostrarCartasTeste(BARALHO *cartas);
void iniciarCartaNaPilha(BARALHO *monte, BARALHO *pilha);
void mostrarCartaDaPilha(ATRIBUTOS *carta);
void analisarCartas(BARALHO *pilha, BARALHO *jogadorP, int op, int *vez, BARALHO *jogadorS, BARALHO *monte);
void jogarCartaNaPilha(BARALHO *pilha, BARALHO *jogador, int op);
void removerVetor (int k, BARALHO *cartas);
void cartaInvalida(int *vez);
void perdeAVez(int *vez);
void escolherCor(BARALHO *pilha, BARALHO *jogador, int op);
void mostrarCartas(BARALHO *cartas);
void maiusculo(USUARIO *jogador);
int verificarSePodeJogarNaMesa(BARALHO *pilha, BARALHO *jogador);
void mostrarAtributosDaCarta(ATRIBUTOS *carta);
void revelarPontos(USUARIO *jogador);
void listarUsuarios();
void salvarDadosDoUsuario(char nome[30], int pontos);

int main(){
//	printf("\n *--- criando baralho ---*\n");
//	criarCartas();
//	printf("\n *--- fim da criacao do baralho ---*\n");
//	mostrarCartasDoArquivo();
//	getch();
	menu();
	return 0;
}

void menu(){
	int opcao;
	do{
		system("cls");
		fflush(stdin);
		printf("\n\n\t\tBem Vindo ao programa UNO\n");
		printf("\nMENU");
		printf("\n 1 - JOGAR.");
		printf("\n 2 - CLASSIFICACAO DOS USUARIOS.");
		printf("\n 0 - SAIR.\n");
		
		printf("\nDigite uma opcao: ");
		scanf("%d",&opcao);
		system("cls");
		
		executa(opcao);
	}while(opcao!=0);
}

void executa(int opcao){
	switch(opcao){
		case 1:{
			jogar();
			break;
		}
		case 2:{
			listarUsuarios();
			break;
		}
		case 0:{
			printf("\n Ate mais!...");
			getch();
			break;
		}
		default:{
			printf("\n Opcao incorreta!\n");
			getch();
			break;
		}
	}
}
	
void mostrarCartasDoArquivo(){
	FILE *arq;
	arq = abreArquivo('r', "cartas.txt");
	
	BARALHO uno;
	
	fread(&uno, sizeof(BARALHO), 1, arq);
	
	mostrarCartasTeste(&uno);
	
//	for(int i=0; i<QTDE; i++){
//		if(uno.carta[i].tipo == 'N'){
//			printf(" [%i] Carta = %s do numero %i \n\n",i,uno.carta[i].cor,uno.carta[i].num);
////			getch();	
//		}else if(uno.carta[i].tipo == 'S'){
//			printf(" [%i] Carta = %s da cor %s \n\n",i,uno.carta[i].acao,uno.carta[i].cor);
////			getch();
//		}else if(uno.carta[i].tipo == 'C'){
//			printf(" [%i] Carta = %s \n\n",i,uno.carta[i].acao);
////			getch();
//		}else{
//			printf("\n Erro 123 \n");
////			getch();
//		}
//	}
	printf("\n A quantidade = %i\n",uno.quant);
	
	fecharArquivo(arq);
}

BARALHO getBaralho(){
	FILE *arq;
	arq = abreArquivo('r', "cartas.txt");
	
	BARALHO cartas;
	
	fread(&cartas, sizeof(BARALHO), 1, arq);	
	
	fecharArquivo(arq);
	
	return cartas;
}

void comprarCartas(BARALHO *jogador, BARALHO *monte, int qtde){
	int ind;
	for(int i=0; i<qtde; i++){
		ind = rand()%monte->quant-1;
		if(monte->carta[ind].local == 0){
			monte->carta[ind].local = 1;
			monte->quant--;
			jogador->carta[jogador->quant] = monte->carta[ind];		
			jogador->quant++;
			removerVetor(ind, monte);		
		}else{
			i--;
		}
	}
}

void removerVetor (int k, BARALHO *cartas){
	int n = cartas->quant;
	for (int j = k+1; j < n; ++j){
		cartas->carta[j-1] = cartas->carta[j];
	}
}

void iniciarCartaNaPilha(BARALHO *monte, BARALHO *pilha){
	int ind;
	int x = 0;
	while(x == 0){
		ind = rand()%monte->quant-1;
		if((monte->carta[ind].local == 0) && (monte->carta[ind].tipo != 'C') && (monte->carta[ind].tipo != 'S')){
			monte->carta[ind].local = 2;
			monte->quant--;
			pilha->carta[pilha->quant] = monte->carta[ind];
			pilha->quant++;
			removerVetor(ind, monte);
			x = 1; 		
		}
	}
}

void mostrarCartaDaPilha(ATRIBUTOS *carta){
	if(carta->tipo == 'N'){
		printf(" %s do numero %i \n\n",carta->cor,carta->num);	
	}else if(carta->tipo == 'S'){
		printf(" %s da cor %s \n\n",carta->acao,carta->cor);
	}else if(carta->tipo == 'C'){
		if(!strcmp(carta->cor, "azul")){
			printf(" JOGUE A COR AZUL \n\n");
		}else if(!strcmp(carta->cor, "verde")){
			printf(" JOGUE A COR VERDE \n\n");
		}else if(!strcmp(carta->cor, "vermelho")){
			printf(" JOGUE A COR VERMELHA \n\n");
		}else if(!strcmp(carta->cor, "amarelo")){
			printf(" JOGUE A COR AMARELA \n\n");
		}else{
			printf(" Erro \n\n");
		}
	}else{
		printf("\n Erro! Na funcao mostrarCartaDaPilha \n");
		getch();
	}
}

void cartaInvalida(int *vez){
	printf("\n Carta invalida!");
	printf("\n Favor escolher carta correspondente!\n");
	getch();
	perdeAVez(vez);
}

void perdeAVez(int *vez){
	if(*vez == 0){
		*vez = 1;
	}else{
		*vez = 0;
	}
}

void jogarCartaNaPilha(BARALHO *pilha, BARALHO *jogador, int op){
	if(jogador->carta[op].local == 1){
		jogador->carta[op].local = 2;
		pilha->carta[pilha->quant] = jogador->carta[op];
		removerVetor(op, jogador);
		jogador->quant--;
		pilha->quant++;		
	}else{
		printf("\n Erro! Na funcao jogarCartaNaPilha\n");
		getch();
	}
}

void escolherCor(BARALHO *pilha, BARALHO *jogador, int op){
	int x;
	char c[1];
	do{
		system("cls");
		printf("\n Escolha a cor desejada:\n");
		printf("\n 1. Azul");
		printf("\n 2. Verde");
		printf("\n 3. Vermelho");
		printf("\n 4. Amarelo");
//		scanf("%i",&x);
		c[0] = getch();
		x = atoi(c);
	}while((x<1) && (x>4));
	
	char corDaCarta[4][10] = {"azul", "verde", "vermelho", "amarelo"};
	
	if(jogador->carta[op].local == 1){
		jogador->carta[op].local = 2;
		jogador->carta[op].num = 11;
		if(x==1){
			strcpy(jogador->carta[op].cor, corDaCarta[0]);
		}else if(x==2){
			strcpy(jogador->carta[op].cor, corDaCarta[1]);
		}else if(x==3){
			strcpy(jogador->carta[op].cor, corDaCarta[2]);
		}else if(x==4){
			strcpy(jogador->carta[op].cor, corDaCarta[3]);
		}
		pilha->carta[pilha->quant] = jogador->carta[op];
		removerVetor(op, jogador);
		jogador->quant--;
		pilha->quant++;		
	}else{
		printf("\n Erro! na funcao escolher carta!\n");
		getch();
	}
}

int verificarSePodeJogarNaMesa(BARALHO *pilha, BARALHO *jogador){
	for(int i=0; i<jogador->quant; i++){		
		if(jogador->carta[i].tipo == 'N'){
			if(!strcmp(pilha->carta[pilha->quant-1].cor, jogador->carta[i].cor)){
				return 1;
			}else if(pilha->carta[pilha->quant-1].num == jogador->carta[i].num){
				return 1;
			}
		}else if(jogador->carta[i].tipo == 'S'){
			if(!strcmp(pilha->carta[pilha->quant-1].cor, jogador->carta[i].cor)){
				return 1;
			}else if(!strcmp(pilha->carta[pilha->quant-1].acao, jogador->carta[i].acao)){
				return 1;
			}
		}else if(jogador->carta[i].tipo == 'C'){
			return 1;
		}
	}
	return 0;
}

void analisarCartas(BARALHO *pilha, BARALHO *jogadorP, int op, int *vez, BARALHO *jogadorS, BARALHO *monte){
	
	// *** CARTA DA PILHA NORMAL ***
	if(pilha->carta[pilha->quant-1].tipo == 'N'){
		
		// *** CARTA DO JOGADOR NORMAL ***
		if(jogadorP->carta[op].tipo == 'N'){
			if(!strcmp(pilha->carta[pilha->quant-1].cor, jogadorP->carta[op].cor)){
				jogarCartaNaPilha(pilha, jogadorP, op);
			}else if(pilha->carta[pilha->quant-1].num == jogadorP->carta[op].num){
				jogarCartaNaPilha(pilha, jogadorP, op);
			}else{
				cartaInvalida(vez);
			}
			
			// *** CARTA DO JOGADOR ESPECIAL ***	
		}else if(jogadorP->carta[op].tipo == 'S'){
			if(!strcmp(pilha->carta[pilha->quant-1].cor, jogadorP->carta[op].cor)){
				if(!strcmp(jogadorP->carta[op].acao, "pular")){
					perdeAVez(vez);
					jogarCartaNaPilha(pilha, jogadorP, op);
				}else if(!strcmp(jogadorP->carta[op].acao, "pescar")){
					int qtde = 2;
					comprarCartas(jogadorS, monte, qtde);
					perdeAVez(vez);
					jogarCartaNaPilha(pilha, jogadorP, op);
				}else{
					printf("\n Erro. A carta eh do tipo especial mas nao eh pular e nem pescar. X1\n");
					getch();
				}
			}else if(!strcmp(pilha->carta[pilha->quant-1].acao, jogadorP->carta[op].acao)){
				if(!strcmp(jogadorP->carta[op].acao, "pular")){
					perdeAVez(vez);
					jogarCartaNaPilha(pilha, jogadorP, op);
				}else if(!strcmp(jogadorP->carta[op].acao, "pescar")){
					int qtde = 2;
					comprarCartas(jogadorS, monte, qtde);
					perdeAVez(vez);
					jogarCartaNaPilha(pilha, jogadorP, op);
				}else{
					printf("\n Erro. A carta eh do tipo especial mas nao eh pular e nem pescar. X2\n");
					getch();
				}
			}else{
				cartaInvalida(vez);
			}
			
			// *** CARTA DO JOGADOR CORINGA ***	
		}else if(jogadorP->carta[op].tipo == 'C'){
			if(!strcmp(jogadorP->carta[op].acao, "coringa")){
				escolherCor(pilha, jogadorP, op);
			}else if(!strcmp(jogadorP->carta[op].acao,"joker")){
				if(verificarSePodeJogarNaMesa(pilha, jogadorP)){
					int qtde = 4;
					comprarCartas(jogadorS, monte, qtde);
					escolherCor(pilha, jogadorP, op);
					perdeAVez(vez);
				}else{
					system("cls");
					printf("\n\n\n\t Nao pode jogar \"coringa comprar\" 4 enquanto possuir carta disponivel para jogar na mesa!\n");
					getch();
				}
			}else{
				printf("\n Erro na funcao analisarCartas: 093\n");
				getch();
			}
		}else{
			printf("\n Erro na funcao analisarCartas: 743\n");
			getch();
		}
		
		
	// *** CARTA DA PILHA ESPECIAL ***
	}else if(pilha->carta[pilha->quant-1].tipo == 'S'){
		
		// *** CARTA DO JOGADOR NORMAL ***	
		if(jogadorP->carta[op].tipo == 'N'){
			if(!strcmp(pilha->carta[pilha->quant-1].cor, jogadorP->carta[op].cor)){
				jogarCartaNaPilha(pilha, jogadorP, op);
			}else{
				cartaInvalida(vez);
			}
			
		// *** CARTA DO JOGADOR ESPECIAL ***
		}else if(jogadorP->carta[op].tipo == 'S'){
			if(!strcmp(pilha->carta[pilha->quant-1].cor, jogadorP->carta[op].cor)){
				if(!strcmp(jogadorP->carta[op].acao, "pular")){
					perdeAVez(vez);
					jogarCartaNaPilha(pilha, jogadorP, op);
				}else if(!strcmp(jogadorP->carta[op].acao, "pescar")){
					int qtde = 2;
					comprarCartas(jogadorS, monte, qtde);
					perdeAVez(vez);
					jogarCartaNaPilha(pilha, jogadorP, op);
				}else{
					printf("\n Erro. A carta eh do tipo especial mas nao eh pular e nem pescar. X3\n");
					getch();
				}
			}else if(!strcmp(pilha->carta[pilha->quant-1].acao, jogadorP->carta[op].acao)){
				system("cls");
				printf("\n pode jogar!\n");
				getch();
				if(!strcmp(jogadorP->carta[op].acao, "pular")){
					perdeAVez(vez);
					jogarCartaNaPilha(pilha, jogadorP, op);
				}else if(!strcmp(jogadorP->carta[op].acao, "pescar")){
					comprarCartas(jogadorS, monte, 2);
					perdeAVez(vez);
					jogarCartaNaPilha(pilha, jogadorP, op);
				}else{
					printf("\n Erro. A carta eh do tipo especial mas nao eh pular e nem pescar. X4\n");
					getch();
				}
			}else{
				cartaInvalida(vez);
			}
			
		// *** CARTA DO JOGADOR CORINGA ***	
		}else if(jogadorP->carta[op].tipo == 'C'){
			if(!strcmp(jogadorP->carta[op].acao, "coringa")){
				escolherCor(pilha, jogadorP, op);
			}else if(!strcmp(jogadorP->carta[op].acao, "joker")){
				if(verificarSePodeJogarNaMesa(pilha, jogadorP)){
					int qtde = 4;
					comprarCartas(jogadorS, monte, qtde);
					escolherCor(pilha, jogadorP, op);
					perdeAVez(vez);
				}else{
					system("cls");
					printf("\n\n\n\t Nao pode jogar \"coringa comprar\" 4 enquanto possuir carta disponivel para jogar na mesa!\n");
					getch();
				}
			}else{
				printf("\n Erro na funcao analisarCartas: 094\n");
				getch();
			}
		}else{
			printf("\n Erro na funcao analisarCartas: 744\n");
			getch();
		}
		
		
	// *** CARTA DA PILHA CORINGA ***
	}else if(pilha->carta[pilha->quant-1].tipo == 'C'){
		
		// *** CARTA DO JOGADOR NORMAL ***
		if(jogadorP->carta[op].tipo == 'N'){
			if(!strcmp(pilha->carta[pilha->quant-1].cor, jogadorP->carta[op].cor)){
				jogarCartaNaPilha(pilha, jogadorP, op);
			}else{
				cartaInvalida(vez);
			}
			
		// *** CARTA DO JOGADOR ESPECIAL ***	
		}else if(jogadorP->carta[op].tipo == 'S'){
			if(!strcmp(pilha->carta[pilha->quant-1].cor, jogadorP->carta[op].cor)){
				if(!strcmp(jogadorP->carta[op].acao, "pular")){
					perdeAVez(vez);
					jogarCartaNaPilha(pilha, jogadorP, op);
				}else if(!strcmp(jogadorP->carta[op].acao, "pescar")){
					int qtde = 2;
					comprarCartas(jogadorS, monte, qtde);
					perdeAVez(vez);
					jogarCartaNaPilha(pilha, jogadorP, op);
				}else{
					printf("\n Erro. A carta eh do tipo especial mas nao eh pular e nem pescar. X5\n");
					getch();
				}
			}else{
				cartaInvalida(vez);
			}
			
		// *** CARTA DO JOGADOR CORINGA ***	
		}else if(jogadorP->carta[op].tipo == 'C'){
			if(!strcmp(jogadorP->carta[op].acao, "coringa")){
				escolherCor(pilha, jogadorP, op);
			}else if(!strcmp(jogadorP->carta[op].acao, "joker")){
				if(verificarSePodeJogarNaMesa(pilha, jogadorP)){
					int qtde = 4;
					comprarCartas(jogadorS, monte, qtde);
					escolherCor(pilha, jogadorP, op);
					perdeAVez(vez);
				}else{
					system("cls");
					printf("\n\n\n\t Nao pode jogar \"coringa comprar\" 4 enquanto possuir carta disponivel para jogar na mesa!\n");
					getch();
				}
			}else{
				printf("\n Erro na funcao analisarCartas: 094\n");
				getch();
			}
		}else{
			printf("\n Erro na funcao analisarCartas: 744\n");
			getch();
		}
		
		
		
	}else{
		printf("\n Erro na funcao analisarCartas: 534\n");
		getch();
	}
}

void mostrarCartas(BARALHO *cartas){
	for(int i = 0; i<cartas->quant; i++){
		if(cartas->carta[i].tipo == 'N'){
			printf(" [%i] Carta => COR: %s \t NUMERO: %i \n\n", i+1, cartas->carta[i].cor, cartas->carta[i].num);	
		}else if(cartas->carta[i].tipo == 'S'){
			printf(" [%i] Carta => COR: %s \t ACAO: %s \n\n", i+1, cartas->carta[i].cor, cartas->carta[i].acao);
		}else if(cartas->carta[i].tipo == 'C'){
			if(!strcmp(cartas->carta[i].acao, "coringa")){
				printf(" [%i] Carta => Coringa  \n\n", i+1);
			}else if(!strcmp(cartas->carta[i].acao, "joker")){
				printf(" [%i] Carta => Coringa comprar 4 \n\n", i+1);
			}else{
				printf(" [%i] Carta => %s \n\n", i+1, cartas->carta[i].acao);
			}
		}else{
			printf("\n Erro 123 \n");
		}
	}
}

void mostrarCartasTeste(BARALHO *cartas){
	system("cls");
	for(int i = 0; i<cartas->quant; i++){
		if(cartas->carta[i].tipo == 'N'){
			printf(" [%i] Carta = %s do numero %i \n\n", i+1, cartas->carta[i].cor, cartas->carta[i].num);	
		}else if(cartas->carta[i].tipo == 'S'){
			printf(" [%i] Carta = %s da cor %s \n\n", i+1, cartas->carta[i].acao, cartas->carta[i].cor);
		}else if(cartas->carta[i].tipo == 'C'){
			if(!strcmp(cartas->carta[i].acao, "coringa")){
				printf(" [%i] Carta => Coringa  \n\n", i+1);
			}else if(!strcmp(cartas->carta[i].acao, "joker")){
				printf(" [%i] Carta => Coringa comprar 4 \n\n", i+1);
			}else{
				printf(" [%i] Carta => %s \n\n", i+1, cartas->carta[i].acao);
			}
		}else{
			printf("\n Erro 123 \n");
		}
	}
	getch();
}

void mostrarAtributosDaCarta(ATRIBUTOS *carta){
	system("cls");
	printf("\n acao = %s \n",carta->acao);
	printf("\n cor = %s \n",carta->cor);
	printf("\n numero = %i \n",carta->num);
	printf("\n tipo = %c \n",carta->tipo);
	printf("\n local = %i \n",carta->local);
	getch();
}

void maiusculo(USUARIO *jogador){
	for(int i=0;i<=strlen(jogador->nome);i++){
		jogador->nome[i] = toupper(jogador->nome[i]);
	}
}

void revelarPontos(USUARIO *jogador){
	int total = 0;
	int especial = 10;
	int coringa = 20;
	for(int i = 0; i<jogador->mao.quant; i++){
		if(jogador->mao.carta[i].tipo == 'N'){
			printf(" [%i] Carta => COR: %s \t NUMERO: %i", i+1, jogador->mao.carta[i].cor, jogador->mao.carta[i].num);
			printf("\t PONTOS = %i \n\n", jogador->mao.carta[i].num);
			total += jogador->mao.carta[i].num;
		}else if(jogador->mao.carta[i].tipo == 'S'){
			printf(" [%i] Carta => COR: %s \t ACAO: %s", i+1, jogador->mao.carta[i].cor, jogador->mao.carta[i].acao);
			printf("\t PONTOS = %i \n\n", especial);
			total += especial;
		}else if(jogador->mao.carta[i].tipo == 'C'){
			if(!strcmp(jogador->mao.carta[i].acao, "coringa")){
				printf(" [%i] Carta => Coringa", i+1);
				printf("\t PONTOS = %i \n\n", coringa);
				total += coringa;
			}else if(!strcmp(jogador->mao.carta[i].acao, "joker")){
				printf(" [%i] Carta => Coringa comprar 4 \n\n", i+1);
				printf("\t PONTOS = %i \n\n", coringa);
				total += coringa;
			}else{
				printf(" Erro 321 \n");
			}
		}else{
			printf("\n Erro 123 \n");
		}
	}
	printf("\n\n\t\t\t TOTAL DE PONTOS DO %s = %i",jogador->nome, total);
	jogador->pontos = total;
	getch();
}	

void jogar(){
	USUARIO jogador[2];
	BARALHO monte = getBaralho();
	BARALHO pilha;
	system("cls");
	fflush(stdin);
	printf("\n Digite o nome do primeiro jogador:\n");
	gets(jogador[0].nome);
	system("cls");
				fflush(stdin);
//	strcpy(jogador[0].nome, "jhonatan");
	printf("\n Digite o nome do segundo jogador:\n");
	gets(jogador[1].nome);
//	strcpy(jogador[1].nome, "fulano");
	maiusculo(&jogador[0]);
	maiusculo(&jogador[1]);
	printf("\n Nome do jogador 1 = %s\n",jogador[0].nome);
	printf("\n Nome do jogador 2 = %s\n",jogador[1].nome);
	getch();
	
	int qtde = 7; //quantidade inicial de cartas
	comprarCartas(&jogador[0].mao, &monte, qtde);
	comprarCartas(&jogador[1].mao, &monte, qtde);
	
	iniciarCartaNaPilha(&monte, &pilha);
	
//	mostrarCartasTeste(&monte);
	
	char c[1];
	int vez = 0;
	int opcao = 1;
	int escolha = 0;
	int jogada = 1;
	int comprou = 0;
	while( opcao != 0 ) {
		if(jogador[0].mao.quant > 0){
			if(jogador[1].mao.quant > 0){
				if((jogador[0].mao.quant == 1) && (vez == 1)){
					system("cls");
					printf("\n\n\n\t\t\t\t ATENCAO!!!");
					printf("\n\n\t\t\t\t %s ESTA UNO!!!",jogador[0].nome);
					getch();
				}
				if((jogador[1].mao.quant == 1) && (vez == 0)){
					system("cls");
					printf("\n\n\n\t\t\t\t ATENCAO!!!");
					printf("\n\n\t\t\t\t %s ESTA UNO!!!",jogador[1].nome);
					getch();
				}
				system("cls");
				fflush(stdin);
//				mostrarAtributosDaCarta(&pilha.carta[pilha.quant-1]);
				printf("\t\t\t\t\t Jogada de numero [%i]\n",jogada);
				printf("\n\t\t\t ==> A CARTA QUE ESTA NA PILHA:");
				mostrarCartaDaPilha(&pilha.carta[pilha.quant-1]);
				printf("\t Vez do Jogador:  ");
				printf("%s\n\n",jogador[vez].nome);
				mostrarCartas(&jogador[vez].mao);
				
				printf("\n Escolha uma das opcoes:\n");	
				if(comprou == 0){
					printf("\n 1. Comprar uma carta.\n");
				}else if(comprou == 1){
					printf("\n 1. Passar a vez.\n");
				}
				
				if(verificarSePodeJogarNaMesa(&pilha, &jogador[vez].mao)){
					printf("\n 2. Jogar uma carta.\n");
				}else{
					printf("\n Voce nao possui carta para jogar. Favor selecionar a opcao 1.\n");
				}
				printf("\n 0. sair.");
				c[0] = getch();
				escolha = atoi(c);
				
				if(escolha == 1){
					if(comprou == 1){
						perdeAVez(&vez);
						comprou = 0;
					}else{
						int qtde = 1;
						comprarCartas(&jogador[vez].mao, &monte, qtde);
						comprou = 1;
					}
				}else if(escolha == 2){
					comprou = 0;
					printf("\n Escolha uma das cartas!");
					printf("\n Ou digite 0 para sair.\n");
					c[0] = getch();
					opcao = atoi(c);
					if(opcao!=0){
						int x = opcao-1;
						int out;
						if(vez == 0){
							out = 1;
						}else{
							out = 0;
						}
						analisarCartas(&pilha, &jogador[vez].mao, x, &vez, &jogador[out].mao, &monte);
					}
					if(vez == 0){
						vez = 1;
					}else{
						vez = 0;
					}
					jogada++;
				}else if(escolha == 0){
					opcao = 0;
				}else{
					printf("\n opcao invalida!!!\n");
					getch();
				}
			}else{
				system("cls");
				printf("\n\n\n\n\t \\o/ %s eh o(a) vencendor(a)!!!\n\n",jogador[1].nome);
				getch();
				revelarPontos(&jogador[0]);
				salvarDadosDoUsuario(jogador[0].nome, jogador[0].pontos);
				opcao = 0;
				
			}
		}else{
			system("cls");
			printf("\n\n\n\n\t \\o/ %s eh o(a) vencendor(a)!!!\n\n",jogador[0].nome);
			getch();
			revelarPontos(&jogador[1]);
			salvarDadosDoUsuario(jogador[1].nome, jogador[1].pontos);
			opcao = 0;
		}
		
		//analisar como esta o monte
//		mostrarCartasTeste(&monte);
	}
}

FILE* abreArquivo(char modo, char caminho[30]){
    FILE *arquivo;
    switch(modo){
        case 'g':
            arquivo = fopen(caminho,"wt"); //Escrever no arquivo, apaga conteudo anterior.
            break;
        case 'l':
            arquivo = fopen(caminho,"rt"); //modo leitura.
            break;
        case 'a':
            arquivo = fopen(caminho,"a"); //Modo Anexar vai anexar ao fim evitando apagar conteudo.
            break;
        case 'w':
        	arquivo = fopen(caminho, "wb"); //Escrever modo binario. Para o struct.
        	break;
        case 'r':
        	arquivo = fopen(caminho, "rb"); //Ler modo binario. Para o struct.
        	break;
    }
    if(arquivo==NULL){      //Se houver algum erro, o ponteiro apontará para NULL
        printf("\n\n\tNao foi possivel abrir o arquivo!");
        exit(0);
    }
    return arquivo;
}

void fecharArquivo(FILE *arquivo){
    fclose(arquivo);
}

void listarUsuarios(){
    FILE *arquivo;
    char nome[30];
    int pontos;

    arquivo = abreArquivo('l',"usuarios.txt");

    while(!feof(arquivo)){
        fscanf(arquivo,"%s %d ", &nome, &pontos);
        printf("\n Nome: %s  -  Pontos: %d\n", nome, pontos);
    }
    fecharArquivo(arquivo);
    getch();
}

void salvarDadosDoUsuario(char nomeJogador[30], int pontosJogador){
	FILE *arquivo;
    char nome[30];
    int pontos;
    
    arquivo = abreArquivo('a', "usuarios.txt");
    fprintf(arquivo, "%s %d\n", nomeJogador, pontosJogador);
    fecharArquivo(arquivo);
}

void criarCartas(){
	FILE *arq;
	arq = abreArquivo('w', "cartas.txt");
	
	BARALHO uno;
	
	char corDaCarta[10];
	char acaoDaCarta[10];
	
	int indice = 0;
	
	//****** CARTAS NORMAIS ******	
	//CARTAS AZUL
	strcpy(corDaCarta, "azul");
	criarCartasNormais(corDaCarta, &uno);
	//CARTAS VERDE
	strcpy(corDaCarta, "verde");
	criarCartasNormais(corDaCarta, &uno);
	//CARTAS VERMELHO
	strcpy(corDaCarta, "vermelho");
	criarCartasNormais(corDaCarta, &uno);
	//CARTAS AMARELO
	strcpy(corDaCarta, "amarelo");
	criarCartasNormais(corDaCarta, &uno);

	//****** CARTAS ESPECIAIS ******	
	//CARTAS "pular"
	strcpy(acaoDaCarta, "pular");
	criarCartasEspeciais(acaoDaCarta, &uno);
	
	//CARTAS "pescar 2"
	strcpy(acaoDaCarta, "pescar"); 
	criarCartasEspeciais(acaoDaCarta, &uno);
	//CARTAS "coringas que mudar de cor"
	strcpy(acaoDaCarta, "coringa"); 
	criarCartasCoringa(acaoDaCarta, &uno);
	//CARTAS "coringas pescar quatro"
	strcpy(acaoDaCarta, "joker"); 
	criarCartasCoringa(acaoDaCarta, &uno);
	
	fwrite(&uno, sizeof(BARALHO), 1, arq);
	fecharArquivo(arq);
	printf("\n Criou com sucesso!\n");
	getch();
}

void criarCartasNormais(char corDaCarta[10], BARALHO *uno){
	int valor = 0;
	for(int num = 0; num<19; num++){
		strcpy(uno->carta[uno->quant].cor, corDaCarta);
		uno->carta[uno->quant].tipo = 'N';
		strcpy(uno->carta[uno->quant].acao, "nada");
		uno->carta[uno->quant].num = valor;
		uno->carta[uno->quant].local = 0;
		uno->quant++;
		valor++;
		if(valor == 10){
			valor = 1;
		}
	}
}

void criarCartasEspeciais(char acaoDaCarta[10], BARALHO *uno){
	char corDaCarta[4][10] = {"azul", "verde", "vermelho", "amarelo"};
	for(int i=0; i<2; i++){
		for(int num=0; num<4; num++){
			strcpy(uno->carta[uno->quant].cor, corDaCarta[num]);
			strcpy(uno->carta[uno->quant].acao, acaoDaCarta);
			uno->carta[uno->quant].tipo = 'S';
			uno->carta[uno->quant].local = 0;
			uno->carta[uno->quant].num = 11;
			uno->quant++;
		}
	}
}

void criarCartasCoringa(char acaoDaCarta[10], BARALHO *uno){
	for(int num=0; num<4; num++){
		strcpy(uno->carta[uno->quant].cor, "preta");
		strcpy(uno->carta[uno->quant].acao, acaoDaCarta);
		uno->carta[uno->quant].tipo = 'C';
		uno->carta[uno->quant].local = 0;
		uno->carta[uno->quant].num = 11;
		uno->quant++;
	}
}
