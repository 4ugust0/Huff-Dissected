#ifndef _BYTECODE
#define _BYTECODE

/***********************************************************************
****************************** BYTECODE ********************************
BYTECODE Implementation. It will be use to store the size of the codifi-
cation and the codification itself.
************************************************************************/

typedef struct BYTECODE{
	int codeSize;
	char *codeString;
}BYTECODE;


/***********************************************************************
Creates a pointer to a BYTECODE type and allocates memmory dinamically.
Then returns the pointer.
************************************************************************/
BYTECODE *BYTECODE_Create(){
	BYTECODE *newByteCode = (BYTECODE *) malloc(sizeof(BYTECODE *));
	newByteCode->codeSize = 0;
	newByteCode->codeString = (char *) realloc(newByteCode->codeString, (newByteCode->codeSize + 1) * sizeof(char));
	newByteCode->codeString[newByteCode->codeSize] = '\n';
	return newByteCode;
}

#endif