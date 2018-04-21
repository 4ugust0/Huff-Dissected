void calculateFrequencies(HashTable *frequencyTable){
	char myString[100];
	printf("digite o caminho do arquivo: ");
	scanf("%[^\n]s", myString);;
	
	FILE *myFile = fopen(myString, "rb");

	unsigned char myByte;
	while(1){
		myByte = fgetc(myFile);
		if(feof(myFile)) break;
		putHashTable(frequencyTable, myByte, frequencyHash, frequencyItem);
	}	

	fclose(myFile);
}

void buildHuffmanTree(BinaryTree *huff, HashTable *frequencyTable){
	Heap nodesHeap;
	initHeap(&nodesHeap, nodesHeapInit);

	int i;
	for(i = 0; i < MAX_HASH_SIZE; ++i){
		if(getHashTable(frequencyTable, i, frequencyHash)){
			enqueueHeap(&nodesHeap,
				createNode(createCharFreq((char)i,
					getHashTable(frequencyTable, (char)i, frequencyHash)), NULL, NULL),
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

	preorderBinaryTree(huff->root, printnodesHuffmanTree);

	DestroyHeap(&nodesHeap, nodesHeapDestroy);
}

void myHashKeycodes(HashTable *ht, Node *root, Keycode myKeycode){
	if(root){
		if(isEmpty(root->left) && isEmpty(root->right)){
			int key = (int) ( ((CharFreq *)root->data)->key );

			Keycode *newKeyCode = (Keycode *) malloc(sizeof(Keycode));
			newKeyCode->bitsSize = myKeycode.bitsSize;
			int i;
			for(i = 0; i < 16; ++i){
				newKeyCode->bitsCode[i] = myKeycode.bitsCode[i];
			}	
			ht->table[key] = newKeyCode;
		}
		else{
			int curr = myKeycode.bitsSize/32;
			int n = myKeycode.bitsSize;

			myKeycode.bitsSize++;

			myHashKeycodes(ht, root->left, myKeycode);
			myKeycode.bitsCode[curr] = setbit(myKeycode.bitsCode[curr],((31)-(n%32)));
			myHashKeycodes(ht, root->right, myKeycode);

		}
	}
}

void hashKeycodes(HashTable *ht, Node *root){
	Keycode myKeycode;
	myKeycode.bitsSize = 0;
	int i;
	for(i = 0; i < 16; ++i){
		myKeycode.bitsCode[i] = 0;
	}

	myHashKeycodes(ht, root, myKeycode);
}

void makeCodification(){
//-----------------------------------------------------------------//
	//###########################################################
	//CALCULATE THE BYTE'S FREQUENCY WITH A HASHTABLE
	//###########################################################
	HashTable frequencyTable;
	initHashTable(&frequencyTable, frequencyInit);
	calculateFrequencies(&frequencyTable);
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
	printf("\n");
	unsigned int x = 0;
	x = setbit(x,((31)-(0%32)));
	printf("%u\n", x);
	

	//###########################################################
//-----------------------------------------------------------------//
	//###########################################################
	//DESTROYING REMAINING OBJECTS
	//###########################################################
	postorderBinaryTree(HuffmanTree.root, destroyNode);
	//###########################################################
//-----------------------------------------------------------------//
}