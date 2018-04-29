#ifndef _CODIFICATION
#define _CODIFICATION

void Codification(char *fileToCompressPath, char *fileCompressedPath){
	HASHTABLE *frequencyHashTable = HASHTABLE_CreateFrequencyTable();
	HASHTABLE_CountFrequencies(frequencyHashTable, fileToCompressPath);

	HEAP *nodeHeap = HEAP_Create();
	HEAP_BuildHuffmanNodeHeap(nodeHeap, frequencyHashTable);

	BINARYTREE *HuffmanTree = BINARYTREE_Create();
	BINARYTREE_BuildHuffmanTree(HuffmanTree, nodeHeap);

	HASHTABLE *byteCodeHashTable = HASHTABLE_CreateByteCodeTable();
	HASHTABLE_StartToHashTheCodification(byteCodeHashTable, HuffmanTree);

	BuildCompressedFile(fileToCompressPath, fileCompressedPath, HuffmanTree, byteCodeHashTable);
}

#endif