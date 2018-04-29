#ifndef _DECODIFICATION
#define _DECODIFICATION

/***********************************************************************
*************************** DECODIFICATION *****************************
In this header will be implemented the decodification itself. Auxiliar
functions will be implemented in Other_Functions_Decodification header.
Some bugs to fix: when an empty file is request to be decodified or the 
compressed file has only one node. In other cases, it worked fine.
************************************************************************/

/***********************************************************************
This is the core implementation of decodification. As i said, other functions
was implemented in another file just to make it easy to debug if it is needed
in the future.
************************************************************************/
void Decodification(char *compressedPath, char *decompressedPath){
	//creating the file pointers to the file to decompress and the new file
	FILE *compressedFile;
	FILE *decompressedFile;

	//here we filter the bit thrash and the size of huffman tree
	//the flag is to handle the case when the codified part has only one byte
	int bitThrash = BITMANIPULATION_FilterBitThrash(compressedPath);
	int huffmanTreeSize = BITMANIPULATION_FilterHuffmanTreeSize(compressedPath);
	int auxiliarFlag = 0;
	
	//making the file pointers point to the compressed file and the new file(creates a new file)
	compressedFile = fopen(compressedPath, "rb");
	decompressedFile = fopen(decompressedPath, "wb");

	//rebuilding the huffman tree, then return a pointer to BINARYTREE.
	BINARYTREE *huffmanTree = BINARYTREE_RebuildHuffmanTree(compressedFile, huffmanTreeSize);

	//this NODE will pointer to the root of huffman tree and we will traverse in
	//for the left or right, depends on the state of each byte readed int the
	//compressed file
	NODE *auxiliarNode = huffmanTree->treeRoot;

	//creating the 2 bytes that will help to decodificate the compressed file
	unsigned char byteToBrush0 = fgetc(compressedFile);
	unsigned char byteToBrush1 = fgetc(compressedFile);

	//here is the verification if the file has only one codified byte.
	if(feof(compressedFile)) auxiliarFlag = 1;

	//the actual bit that we are looking for the start. Will help on traversal
	//in the tree
	int bitCounter = 7;

	while(1 && !auxiliarFlag){

		//when we reach a leaf, just print the byte in the decompressed file
		//and set the auxiliar node to the root of huffman tree
		if(auxiliarNode->leftNode == NULL && auxiliarNode->rightNode == NULL){
			fputc(auxiliarNode->nodeData, decompressedFile);
			auxiliarNode = huffmanTree->treeRoot;
		}

		//checkin if the bit is setled up. in this case, just move to the right
		//of the when. Decrements the bit counter and when the bit counter is 
		// -1, reset the bit counter, switch byte0 and byte1 and read the next
		//byte.
		if(BITMANIPULATION_IsBitSet(byteToBrush0, bitCounter)){
			auxiliarNode = auxiliarNode->rightNode;
			bitCounter--;
			if(bitCounter == -1){
				bitCounter = 7;
				byteToBrush0 = byteToBrush1;
				byteToBrush1 = fgetc(compressedFile);
				if(feof(compressedFile)) break;
			}
		}
		//Similar to the last part, but moving to the left of the node.
		else{
			auxiliarNode = auxiliarNode->leftNode;
			bitCounter--;
			if(bitCounter == -1){
				bitCounter = 7;
				byteToBrush0 = byteToBrush1;
				byteToBrush1 = fgetc(compressedFile);
				if(feof(compressedFile)) break;
			}
		}

	}

	//here we start to filtering the last readed byte. It depends to the size of
	//bit thrash filtered before.
	int i;
	for(i = 7; i >= (bitThrash-1); --i){
		if(auxiliarNode->leftNode == NULL && auxiliarNode->rightNode == NULL){
			fputc(auxiliarNode->nodeData, decompressedFile);
			auxiliarNode = huffmanTree->treeRoot;
		}

		if(BITMANIPULATION_IsBitSet(byteToBrush0, i)){
			auxiliarNode = auxiliarNode->rightNode;
		}
		else{
			auxiliarNode = auxiliarNode->leftNode;
		}
	}

	//here we close the files. The huffman decodification is finished.
	fclose(compressedFile);
	fclose(decompressedFile);
}

#endif