#include "myIncludes.h"

int main(void){

	unsigned x;
	scanf("%u\n", &x);
	if(x == 0) Codification();
	else if(x == 1) Decodification();
	else printf("xD\n");
	return 0;
}