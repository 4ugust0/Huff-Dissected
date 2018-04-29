#ifndef _BINARYTREE
#define _BINARYTREE

/***********************************************************************
************************** BINARYTREE and NODE *************************
BINARYTREE and NODE implementations. It will be use to build and rebuild
the huffman tree to compress and decompress a file, repectively.
************************************************************************/

typedef struct HUFFMANDATA{
	long long frequencyValue;
	unsigned char byteKey;
	int insertionIndex;
}HUFFMANDATA;

typedef struct NODE{
	GENERIC nodeData;
	struct NODE *leftNode;
	struct NODE *rightNode;
}NODE;

typedef struct BINARYTREE{
	NODE *treeRoot;
}BINARYTREE;


/***********************************************************************
Creates a pointer to BINARYTREE type and allocates memmory dinamically
an returns the pointer.
************************************************************************/
BINARYTREE *BINARYTREE_Create(){
	BINARYTREE *newBinaryTree = (BINARYTREE *) malloc(sizeof(BINARYTREE));
	newBinaryTree->treeRoot = NULL;
	return newBinaryTree;
}

NODE *NODE_CreateSimpleByteNode(unsigned char byteKey){
	NODE *newNode = (NODE *) malloc(sizeof(NODE));
	newNode->nodeData = byteKey;
	newNode->leftNode = newNode->rightNode = NULL;
	return newNode;
}

/***********************************************************************
Creates an internal node for the huffman tree then returns it.
************************************************************************/
NODE *NODE_HuffmanCodificationNode(long long frequencyValue, unsigned char byteKey, int insertionIndex){
	NODE *newNode = (NODE *) malloc(sizeof(NODE));
	newNode->leftNode = newNode->rightNode = NULL;

	HUFFMANDATA *newHuffmanData = (HUFFMANDATA *) malloc(sizeof(HUFFMANDATA));
	newHuffmanData->frequencyValue = frequencyValue;
	newHuffmanData->byteKey = byteKey;
	newHuffmanData->insertionIndex = insertionIndex;

	newNode->nodeData = newHuffmanData;
	return newNode;
}

/***********************************************************************
Takes two internal nodes of the huffman tree and merge them creating a
new one then returns the new node.
************************************************************************/
NODE *NODE_MergeHuffmanCodificationNodes(int newInsertionIndex, GENERIC firstNode, GENERIC secondNode){
	HUFFMANDATA *firstHuffmanData = (HUFFMANDATA *) ((NODE *) firstNode)->nodeData;
	HUFFMANDATA *secondHuffmanData = (HUFFMANDATA *) ((NODE *) secondNode)->nodeData;

	long long newFrequencyValue = firstHuffmanData->frequencyValue + secondHuffmanData->frequencyValue;
	unsigned char newByteKey = '*';

	NODE *newNode = NODE_HuffmanCodificationNode(newFrequencyValue, newByteKey, newInsertionIndex);
	newNode->leftNode = firstNode;
	newNode->rightNode = secondNode;
	return newNode;
}

#endif