#include <stdio.h>
#include <stdlib.h>

void debugar(unsigned char c){
	int i;
	for(i = 7; i >= 0; --i){
		if(c & (1<<i)) printf("1");
		else printf("0");
	}
	printf(" ");
}

int main(void){
	char string[100];
	scanf("%s", string);
	FILE *myFile = fopen(string, "rb");
	unsigned char c;
	while(1){
		c = fgetc(myFile);
		if(feof(myFile)) break;
		debugar(c);
	}
	printf("\n");
	return 0;
}