#ifndef _OTHERFUNCTIONDECODIFICATION
#define _OTHERFUNCTIONDECODIFICATION

/***********************************************************************
***************** OTHER FUNCTIONS FOR DECODIFICATION *******************
Hybrids function that takes more than one data structure or utility
functions used in decodification will be implemented in this header.
************************************************************************/

/***********************************************************************
This function will filter the size of the bit thrash that will be used
to decodificate the last byte of the compressed file. After filtering,
returns the bit thrash size.
************************************************************************/
int BITMANIPULATION_FilterBitThrash(char *compressedPath){
	FILE *compressedFile = fopen(compressedPath, "rb");
	unsigned char firstHeaderByte = fgetc(compressedFile);
	fclose(compressedFile);

	return firstHeaderByte>>5;
}

/***********************************************************************
Similar to filter the bit thrash, this function will filter the huffman
tree size and return it.
************************************************************************/
int BITMANIPULATION_FilterHuffmanTreeSize(char *compressedPath){
	FILE *compressedFile = fopen(compressedPath, "rb");
	unsigned char firstHeaderByte = fgetc(compressedFile);
	unsigned char secondHeaderByte = fgetc(compressedFile);
	fclose(compressedFile);

	int firstInt = (int) (firstHeaderByte & 31);
	int secondInt = (int) secondHeaderByte;

	return (firstInt<<8) | secondInt;
}

/***********************************************************************
Function used to check if a bit is up. It will be necessary to check
what bit is up or down and traversal the huffman three.
************************************************************************/
int BITMANIPULATION_IsBitSet(unsigned char byteKey, int bitToCheck){
	return byteKey & (1<<bitToCheck);
}

/***********************************************************************
Recursive function to read all the huffman tree nodes in the header of the
compressed file. After, returns the huffman tree root.
************************************************************************/
NODE *NODE_ReadHuffmanTree(FILE *compressedFile, unsigned char byteKey){
	unsigned char scapeKey = '\\';
	unsigned char internalDivisionKey = '*';

	NODE *newNode = NODE_CreateSimpleByteNode(byteKey);
	if(byteKey == scapeKey){
		newNode->nodeData = fgetc(compressedFile);
	}
	if(byteKey == internalDivisionKey){
		newNode->leftNode = NODE_ReadHuffmanTree(compressedFile, fgetc(compressedFile));
		newNode->rightNode = NODE_ReadHuffmanTree(compressedFile, fgetc(compressedFile));
	}

	return newNode;
}

/***********************************************************************
Setting things to begin with the reading of huffman tree, here we use
NODE_ReadHuffmanTree. Then, returns a pointer to BINARYTREE, it's the
HuffmanTree.
************************************************************************/
BINARYTREE *BINARYTREE_RebuildHuffmanTree(FILE *compressedFile, int huffmanTreeSize){
	BINARYTREE *newBinaryTree = BINARYTREE_Create();
	fgetc(compressedFile); fgetc(compressedFile);

	unsigned char byteKey = fgetc(compressedFile);
	newBinaryTree->treeRoot = NODE_ReadHuffmanTree(compressedFile, byteKey);
	return newBinaryTree;
}
#endif