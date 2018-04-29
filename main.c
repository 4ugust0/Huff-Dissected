#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GENERIC void*

#include "Binary_Tree_Implementation.h"
#include "Heap_implementation.h"
#include "Hash_Table_Implementation.h"
#include "Byte_Code_Implementation.h"
#include "Other_Functions_Codification.h"
#include "Codification_Implementation.h"
#include "Other_Functions_Decodification.h"
#include "Decodification_Implementation.h"

int main(void){

	int choiceNumber;
	char firstPath[100], secondPath[100];
	scanf("%d", &choiceNumber);
	scanf(" %[^\n]s", firstPath);
	scanf(" %[^\n]s", secondPath);

	if(choiceNumber == 0) Codification(firstPath, secondPath);
	else if(choiceNumber == 1) Decodification(firstPath, secondPath);

	return 0;
}