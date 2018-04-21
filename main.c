#include "includes.h"

void decodification(){
	return;
}

int main(void){

	char x;
	printf("para codificar um arquivo, digite 0.\n");
	printf("para decodificar um arquivo, digite 1.\n");
	printf("digite 2 para sair.\n");
	scanf("%c", &x);
	getchar();
	if(x == '0') codification();
	if(x == '1') decodification();
	if(x == '2') printf("XD\n");

	return 0;
}