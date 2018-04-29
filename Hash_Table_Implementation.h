#ifndef _HASHTABLE
#define _HASHTABLE

/***********************************************************************
***************************** HASH TABLE *******************************
HASH TABLE Implementation. It will be used to store the frequency of each
byte representation in the file to compress and used to store the codifi-
cation of each byte representation as well.
************************************************************************/

#define MAX_HASHTABLE_SIZE 256

typedef struct HASHTABLE{
	GENERIC *dataTable;
}HASHTABLE;

/***********************************************************************
Creates a pointer to HASHTABLE type and allocates memmory dinamicaly to it.
them, allocates and array of long long integers, it will be use to count
the frequencies of all byte representations in the file to be compressed.
Then, return the pointer.
************************************************************************/
GENERIC HASHTABLE_CreateFrequencyTable(){
	HASHTABLE *newHashTable = (HASHTABLE *) malloc(sizeof(HASHTABLE));
	newHashTable->dataTable = (long long *) calloc(MAX_HASHTABLE_SIZE, sizeof(long long));
	return newHashTable;
}

/***********************************************************************
Simple hash function, just take the int value of the byte representation,
then returns it.
************************************************************************/
int HASHTABLE_HashFunction(unsigned char byteKey){
	return (int) byteKey;
}

/***********************************************************************
Stores the frequency of any byte that appears in the file to compress.
************************************************************************/
void HASHTABLE_PutFrequency(HASHTABLE *myHashTable, unsigned char byteKey){
	myHashTable->dataTable[(int)(byteKey)]++;
}

/***********************************************************************
Destroy function to prevent memmory leaks.
************************************************************************/
void HASHTABLE_Destroy(HASHTABLE *myHashTable){
	free(myHashTable->dataTable);
	free(myHashTable);
}

/***********************************************************************
Creates a pointer to BINARYTREE type and allocates memmory dinamically.
************************************************************************/
void HASHTABLE_CountFrequencies(HASHTABLE *myHashTable, char *filePath){
	FILE *fileToCompress = fopen(filePath, "rb");
	unsigned char auxiliarByte;
	while(1){
		auxiliarByte = fgetc(fileToCompress);
		if(feof(fileToCompress)) break;
		HASHTABLE_PutFrequency(myHashTable, auxiliarByte);
	}

	fclose(fileToCompress);
}

#endif