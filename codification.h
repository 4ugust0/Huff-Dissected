int isbitset(unsigned char byte, unsigned char b){
	unsigned char mask = 1<<b;
	return ( mask & byte );
}

unsigned char setbit(unsigned char byte, unsigned char b){
	unsigned char mask = 1<<b;
	return(mask | byte);
}

void debugarByte(unsigned char byte){
	int i;
	for(i = 7; i >= 0; --i){
		if(isbitset(byte, i)) printf("1");
		else printf("0");
	}
}

void calculateFrequencies(HashTable *frequencyTable, char *myString){

	FILE *myFile = fopen(myString, "rb");
	unsigned char myByte;
	while(1){
		myByte = fgetc(myFile);
		if(feof(myFile)) break;
		putHashTable(frequencyTable, myByte, byteHash, frequencyItem);
	}
	fclose(myFile);
}

void buildHuffmanTree(BinaryTree *huff, HashTable *frequencyTable){
	Heap nodesHeap;
	initHeap(&nodesHeap, nodesHeapInit);

	int i;
	for(i = 0; i < MAX_HASH_SIZE; ++i){
		if(getHashTable(frequencyTable, i, byteHash)){
			enqueueHeap(&nodesHeap,
				createNode(createCharFreq((char)i,
					getHashTable(frequencyTable, (char)i, byteHash)), NULL, NULL),
						nodesHeapCmp, nodesHeapSwap, nodesHeapReturn);
		}
	}

	while(nodesHeap.size != 1){
		Node *a = dequeueHeap(&nodesHeap, nodesHeapCmp, nodesHeapSwap);
		Node *b = dequeueHeap(&nodesHeap, nodesHeapCmp, nodesHeapSwap);

		long long aFrequency = ((CharFreq *)a->data)->frequency;
		long long bFrequency = ((CharFreq *)b->data)->frequency;

		enqueueHeap(&nodesHeap, createNode(createCharFreq('*', aFrequency+bFrequency),
						a, b), nodesHeapCmp, nodesHeapSwap, nodesHeapReturn);
	}

	huff->root = dequeueHeap(&nodesHeap, nodesHeapCmp, nodesHeapSwap);
	DestroyHeap(&nodesHeap, nodesHeapDestroy);
}

void myHashKeycodes(HashTable *ht, Node *root, Keycode myKeycode){
	if(root){
		if(isEmpty(root->left) && isEmpty(root->right)){
			int key = (int) ( ((CharFreq *)root->data)->key );

			Keycode *newKeyCode = (Keycode *) malloc(sizeof(Keycode));
			newKeyCode->bitsSize = myKeycode.bitsSize;
			int i;
			for(i = 0; i < 64; ++i){
				newKeyCode->bitsCode[i] = myKeycode.bitsCode[i];
			}	
			ht->table[key] = newKeyCode;
		}
		else{
			unsigned int curr = myKeycode.bitsSize>>6;
			unsigned int n = myKeycode.bitsSize;

			myKeycode.bitsSize++;

			myHashKeycodes(ht, root->left, myKeycode);
			myKeycode.bitsCode[curr] = setbit(myKeycode.bitsCode[curr],((7)-(n%8)));
			myHashKeycodes(ht, root->right, myKeycode);

		}
	}
}

void hashKeycodes(HashTable *ht, Node *root){
	Keycode myKeycode;
	myKeycode.bitsSize = 0;
	int i;
	for(i = 0; i < 64; ++i){
		myKeycode.bitsCode[i] = 0;
	}

	myHashKeycodes(ht, root, myKeycode);
}

void printHuffmanNodes(FILE *fileCompressed, Node *root, int *numNodes){
	if(!isEmpty(root)){
		CharFreq *myCharFreq = (CharFreq *)root->data;
		if((isEmpty(root->left) && isEmpty(root->right)) && (myCharFreq->key == '*' || myCharFreq->key == '\\')){
				fputc('\\', fileCompressed);
				fputc(myCharFreq->key, fileCompressed);
		}
		else{
			fputc(myCharFreq->key, fileCompressed);
		}
		(*numNodes)++;
		printHuffmanNodes(fileCompressed, root->left, numNodes);
		printHuffmanNodes(fileCompressed, root->right, numNodes);
	}
}

void brushTheBits(HashTable *codificationTable, BinaryTree *HuffmanTree, char *myString){
	
	int numNodes = 0;
	FILE *filetoCompress = fopen(myString, "rb");
	FILE *fileCompressed = fopen("compressed", "wb");

	myByte toBrush;
	toBrush.byte = 0;
	toBrush.counter = 0;
	int i;
	unsigned char currByte;
	unsigned int bitsSize;
	unsigned char *bitsCode; 

	fputc(toBrush.byte, fileCompressed);
	fputc(toBrush.byte, fileCompressed);

	printHuffmanNodes(fileCompressed, HuffmanTree->root, &numNodes);

	while(1){
		currByte = fgetc(filetoCompress);
		if(feof(filetoCompress)) break;

		Keycode *myKeycode = (Keycode *)(codificationTable->table[byteHash(currByte)]);
		bitsSize = myKeycode->bitsSize;
		bitsCode = myKeycode->bitsCode;
		for(i = 0; i < bitsSize; ++i){
			if(isbitset(bitsCode[(i>>6)],7-modp2(i,8))){
				toBrush.byte = setbit(toBrush.byte,7-modp2(toBrush.counter,8));
			}
			toBrush.counter++;
			if(!(modp2(toBrush.counter,8))){
				fputc(toBrush.byte, fileCompressed);
				toBrush.byte = 0;
			}
		}

	}
	int bitThrash = 8 - modp2(toBrush.counter,8);
	if(bitThrash != 8) fputc(toBrush.byte, fileCompressed);
	fclose(filetoCompress);
	fclose(fileCompressed);

	int Header = (bitThrash<<13) | numNodes;
	toBrush.byte = 0;
	toBrush.counter = 0;

	fileCompressed = fopen("compressed", "r+b");
	for(i = 15; i >= 0; --i){
		if(Header & (1<<i)){
			toBrush.byte = setbit(toBrush.byte,(7 - modp2(toBrush.counter,8)));
		}
		toBrush.counter++;
		if(!(modp2(toBrush.counter,8))){
			fputc(toBrush.byte, fileCompressed);
			toBrush.byte = 0;
		}
	}
	fclose(fileCompressed);
}

void makeCodification(){

	char myString[100];
	scanf("%[^\n]s", myString);;

//-----------------------------------------------------------------//
	//###########################################################
	//CALCULATE THE BYTE'S FREQUENCY WITH A HASHTABLE
	//###########################################################
	HashTable frequencyTable;
	initHashTable(&frequencyTable, frequencyInit);
	calculateFrequencies(&frequencyTable, myString);
	//###########################################################
//-----------------------------------------------------------------//
	//###########################################################
	//BUILD THE HUFFMAN TREE WITH THE FREQUENCY HASH TABLE
	//###########################################################
	BinaryTree HuffmanTree;
	buildHuffmanTree(&HuffmanTree, &frequencyTable);
	//###########################################################
//-----------------------------------------------------------------//	
	//###########################################################
	//DESTROYING FREQUENCY HASH TABLE I WON'T USE ANYMORE
	//###########################################################
	destroyHashTable(&frequencyTable);
	//###########################################################
//-----------------------------------------------------------------//
	//###########################################################
	//HASH TABLE WITH THE KEYS CODIFICATION FOR COMPRESS THE FILE
	//###########################################################
	HashTable codificationTable;
	initHashTable(&codificationTable, codificationInit);
	hashKeycodes(&codificationTable, HuffmanTree.root);
	//###########################################################
//-----------------------------------------------------------------//
	//###########################################################
	//TIME TO BRUSH BITS
	//###########################################################
	brushTheBits(&codificationTable, &HuffmanTree, myString);
	//###########################################################
//-----------------------------------------------------------------//
	//###########################################################
	//DESTROYING REMAINING OBJECTS
	//###########################################################
	postorderBinaryTree(HuffmanTree.root, destroyNode);
	//###########################################################
//-----------------------------------------------------------------//
}