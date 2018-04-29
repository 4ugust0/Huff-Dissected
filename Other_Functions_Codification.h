#ifndef _OTHERFUNCTIONSCODIFICATION
#define _OTHERFUNCTIONSCODIFICATION

/***********************************************************************
******************* OTHER FUNCTIONS FOR CODIFICATION *******************
Hybrids function that takes more than one data structure or utility
functions used in codification will be implemented in this header.
************************************************************************/

/***********************************************************************
Iterates over the Hash Table that stores the frequency of each byte and,
when it has frequency (positive value -> true value) creates an internal
huffman node wifh the frequency of the byte, the byte representation and
the insertion index (order of insertion). Insertion index will be helpfull
in the comparison function used in heap.
************************************************************************/
void HEAP_BuildHuffmanNodeHeap(HEAP *nodeHeap, HASHTABLE *frequencyHashTable){
	int i;
	for(i = 0; i < MAX_HASHTABLE_SIZE; ++i){
		if(frequencyHashTable->dataTable[HASHTABLE_HashFunction(i)]){
			long long frequencyValue = frequencyHashTable->dataTable[HASHTABLE_HashFunction(i)];
			unsigned char byteKey = (unsigned char) i;
			int insertionIndex = nodeHeap->insertionIndexCounter;

			NODE *newNode = NODE_HuffmanCodificationNode(frequencyValue, byteKey, insertionIndex);
			HEAP_Enqueue(nodeHeap, newNode);
		}
	}
}

/***********************************************************************
Following the Huffman Coding Algorithm, takes the two nodes with higher
priority and merge them creating a new internal node with '*' representation
and enqueueing back in the heap. The algorithm stops when the heap has only
one element, and this element will be the root of Huffman Tree.
************************************************************************/
void BINARYTREE_BuildHuffmanTree(BINARYTREE *HuffmanTree, HEAP *nodeHeap){
	while(HEAP_GetSize(nodeHeap) > 1){
		NODE *firstDequeued = HEAP_Dequeue(nodeHeap);
		NODE *secondDequeued = HEAP_Dequeue(nodeHeap);
		NODE *mergedNode = NODE_MergeHuffmanCodificationNodes(nodeHeap->insertionIndexCounter, firstDequeued, secondDequeued);
		HEAP_Enqueue(nodeHeap, mergedNode);
	}
	HuffmanTree->treeRoot = HEAP_Dequeue(nodeHeap);
}

/***********************************************************************
Manipulates the byte to set the required bit.
************************************************************************/
unsigned char BITMANIPULATION_SetBit(unsigned char byteToSet, unsigned int bitToSet){
	unsigned char bitMask = 1<<bitToSet;
	return byteToSet | bitMask;
}

/***********************************************************************
Defines the first header byte of the compressed file. The first 3 bits will
store the bit thrash size, and the remaining bits will be part of the huffman
tree size.
************************************************************************/
unsigned char BITMANIPULATION_DefineFirstHeaderByte(int bitThrash, int numNodes){
	int auxiliarVariable = (bitThrash<<13) | numNodes;
	return auxiliarVariable>>8; 
}

/***********************************************************************
Defines the seconde header byte of the compressed file. All bits will be
used to determine the Huffman Tree size.
************************************************************************/
unsigned char BITMANIPULATION_DefineSecondHeaderByte(int bitThrash, int numNodes){
	int auxiliarVariable = (bitThrash<<13) | numNodes;
	return auxiliarVariable; 
}

/***********************************************************************
Creates an pointer to HASHTABLE type. This function will return a Hash
Table that will store BYTECODE type, a struct define to store a size of
bit representation of an byt and his codification as well.
************************************************************************/
HASHTABLE *HASHTABLE_CreateByteCodeTable(){
	HASHTABLE *newHashTable = (HASHTABLE *) malloc(sizeof(HASHTABLE));
	newHashTable->dataTable = (BYTECODE **) calloc(MAX_HASHTABLE_SIZE, sizeof(BYTECODE *));
	return newHashTable;
}

/***********************************************************************
Simple function to put in the hash table the representation of each codi-
fied byte.
************************************************************************/
void HASHTABLE_PutByteCodeInHashTable(BYTECODE *codificationByteCode, HASHTABLE *byteCodeHashTable, unsigned char byteKey){
	BYTECODE *newByteCode = BYTECODE_Create();
	newByteCode->codeSize = codificationByteCode->codeSize;
	newByteCode->codeString = (char *) realloc(newByteCode->codeString, (newByteCode->codeSize + 1)* sizeof(char));
	strcpy(newByteCode->codeString, codificationByteCode->codeString);

	byteCodeHashTable->dataTable[HASHTABLE_HashFunction(byteKey)] = newByteCode;
}

/***********************************************************************
Recursing to, when reachs a leaf node, put in the codification hash table
the byte representation of each leaf.
************************************************************************/
void HASHTABLE_HashTheCodification(NODE *currentRoot, HASHTABLE *byteCodeHashTable, BYTECODE *codificationByteCode){
	if(currentRoot){
		//when it's a leaf, just copy the current BYTECODE in the hash table
		if(currentRoot->leftNode == NULL && currentRoot->rightNode == NULL){
			unsigned char byteKey = ((HUFFMANDATA *) currentRoot->nodeData)->byteKey;
			HASHTABLE_PutByteCodeInHashTable(codificationByteCode, byteCodeHashTable, byteKey);
		}
		//when it's not a leaf, then recurse to find a leaf
		else{
			//raise the size of the code by 1 and put '\n' in the and
			codificationByteCode->codeSize++;
			codificationByteCode->codeString = (char *) realloc(codificationByteCode->codeString, (codificationByteCode->codeSize + 1) * sizeof(char));
			codificationByteCode->codeString[codificationByteCode->codeSize] = '\n';

			//when going to left, put '0' before '\n'
			codificationByteCode->codeString[codificationByteCode->codeSize - 1] = '0';
			HASHTABLE_HashTheCodification(currentRoot->leftNode, byteCodeHashTable, codificationByteCode);

			//when going to right, put '1' before '\n'
			codificationByteCode->codeString[codificationByteCode->codeSize - 1] = '1';
			HASHTABLE_HashTheCodification(currentRoot->rightNode, byteCodeHashTable, codificationByteCode);

			//return to initial local state, we're handling pointer so be careful
			codificationByteCode->codeSize--;
			codificationByteCode->codeString[codificationByteCode->codeSize] = '\n';
		}
	}
}

/***********************************************************************
Defines the seconde header byte of the compressed file. All bits will be
used to determine the Huffman Tree size.
************************************************************************/
void HASHTABLE_StartToHashTheCodification(HASHTABLE *byteCodeHashTable, BINARYTREE *HuffmanTree){
	BYTECODE *newByteCode = BYTECODE_Create();
	HASHTABLE_HashTheCodification(HuffmanTree->treeRoot, byteCodeHashTable, newByteCode);
}

/***********************************************************************
Writes the representation of the huffman tree generated for the file.
************************************************************************/
void NODE_WriteHuffmanTreeInCompressedFIle(NODE *currentRoot, FILE *compressedFile, int *numNodes){
	if(currentRoot){
		(*numNodes)++;
		unsigned char scapeCharacter = '\\';
		unsigned char starByte = '*';

		if(currentRoot->leftNode == NULL && currentRoot->rightNode == NULL){
			if(((HUFFMANDATA *) currentRoot->nodeData)->byteKey == starByte || ((HUFFMANDATA *) currentRoot->nodeData)->byteKey == scapeCharacter){
				fputc(scapeCharacter, compressedFile);
			}
		}
		fputc(((HUFFMANDATA *) currentRoot->nodeData)->byteKey, compressedFile);
		NODE_WriteHuffmanTreeInCompressedFIle(currentRoot->leftNode, compressedFile, numNodes);
		NODE_WriteHuffmanTreeInCompressedFIle(currentRoot->rightNode, compressedFile, numNodes);
	}
}

/***********************************************************************
Time to build the compressed file. Takes the path of the file to compress
and the name of the new file.
************************************************************************/
void BuildCompressedFile(char *fileToCompressPath, char *fileCompressedPath, BINARYTREE *HuffmanTree, HASHTABLE *byteCodeHashTable){
	//Open the file to compress and creating the codified file
	FILE *fileToCompress = fopen(fileToCompressPath, "rb");
	FILE *fileCompressed = fopen(fileCompressedPath, "wb");

	//self-explained variables
	unsigned char byteToRead, byteToBrush = 0;
	int bitCounter = 7, numNodes = 0;

	//here is printed in the codified file the first two byte that will be part of
	//the header
	fputc(0, fileCompressed);
	fputc(0, fileCompressed);

	//Calls the function to write the huffman three in the header
	NODE_WriteHuffmanTreeInCompressedFIle(HuffmanTree->treeRoot, fileCompressed, &numNodes);

	//reads each byte in the file to compress and codifie it, printing in the
	//new file when the byteToBrush is fully brushed with the code.
	while(1){
		byteToRead = fgetc(fileToCompress);
		if(feof(fileToCompress)) break;

		BYTECODE *myBytecode = byteCodeHashTable->dataTable[HASHTABLE_HashFunction(byteToRead)];
		int codeSize = myBytecode->codeSize;
		char *codeString = myBytecode->codeString;

		int i;
		for(i = 0; i < codeSize; ++i){
			if(codeString[i] == '1'){
				byteToBrush = BITMANIPULATION_SetBit(byteToBrush, bitCounter);
			}
			bitCounter--;
			if(bitCounter == -1){
				fputc(byteToBrush, fileCompressed);
				bitCounter = 7;
				byteToBrush = 0;
			}
		}
	}

	//here is decided when print the last byte if there is trash in the last byte
	int bitThrash = 0;
	if((bitCounter+1) != 7){
		bitThrash = bitCounter + 1;
		fputc(byteToBrush, fileCompressed);
	}  

	//Closing files to set the header with the bit thrash and huffman tree size
	fclose(fileToCompress);
	fclose(fileCompressed);

	//defining the first two bytes
	unsigned char firstHeaderByte = BITMANIPULATION_DefineFirstHeaderByte(bitThrash, numNodes);
	unsigned char secondHeaderByte = BITMANIPULATION_DefineSecondHeaderByte(bitThrash, numNodes);

	//here is opened the file compressed and updated the first two bytes, then close
	//the file
	fileCompressed = fopen(fileCompressedPath, "r+b");

	fputc(firstHeaderByte, fileCompressed);
	fputc(secondHeaderByte, fileCompressed);

	fclose(fileCompressed);
}


#endif