#include "includes.h"

//###################################
// FUTURES BUGS: MAY NOT COMPRESS
// IN THE RIGHT WAY BECOUSE OF THE
// HEAPCMP FUNCTIONS
// MAY NEED SOME CHANGES IN THE CASE
// WHEN '*' IS A LEAF OR AN INTERNAL
// NODE.
//###################################

int main(void){

	char x;
	scanf("%c", &x);
	getchar();
	if(x == '0') makeCodification();
	if(x == '1') makeDecodification();
	if(x == '2') printf("XD\n");

	return 0;
}