//heap part
Heap *CreateHuffmanNodeHeap(){
	Heap *newHeap = (Heap *) malloc(MAX_HEAP_SIZE * sizeof(Heap));
	newHeap->arr = (Node **) malloc(MAX_HEAP_SIZE * sizeof(Node *));
	newHeap->size = 0;
	return newHeap;
}

unsigned int GetSizeHeap(Heap *heap){
	return heap->size;
}

unsigned int GetParentHeap(unsigned int myChildren){
	return myChildren>>1;
}

unsigned int GetLeftChildren(unsigned int myParent){
	return (myParent<<1);
}

unsigned int GetRightChildren(unsigned int myParent){
	return ((myParent<<1) + 1);
}

Node *GetDataHuffmanNodeHeap(Heap *HuffmanNodeHeap, unsigned int myIndex){
	return HuffmanNodeHeap->arr[myIndex];
}

unsigned int HuffmanNodeHeapCmp(Heap *HuffmanNodeHeap, unsigned int myChildren, unsigned int myParent){
	HuffmanData *myChildrenData = (HuffmanData *)((GetDataHuffmanNodeHeap(HuffmanNodeHeap, myChildren))->data);
	HuffmanData *myParentData = (HuffmanData *)((GetDataHuffmanNodeHeap(HuffmanNodeHeap, myParent))->data);

	if(myChildrenData->frequency < myParentData->frequency) return 1;
	if(myChildrenData->frequency == myParentData->frequency){
		if(myChildrenData->key == '*' && myParentData->key != '*') return 1;
	}
	return 0;

}

void HuffmanNodeHeapSwap(Heap *HuffmanNodeHeap, unsigned int iIndex, unsigned int jIndex){
	Node *temp = HuffmanNodeHeap->arr[iIndex];
	HuffmanNodeHeap->arr[iIndex] = HuffmanNodeHeap->arr[jIndex];
	HuffmanNodeHeap->arr[jIndex] = temp;
}

void EnqueueHuffmanNodeHeap(Heap *HuffmanNodeHeap, Node *newNode){
	HuffmanNodeHeap->arr[++HuffmanNodeHeap->size] = newNode;

	unsigned int myChildren = GetSizeHeap(HuffmanNodeHeap);
	unsigned int myParent = GetParentHeap(myChildren);

	while(myParent >= 1 && HuffmanNodeHeapCmp(HuffmanNodeHeap, myChildren, myParent)){
		HuffmanNodeHeapSwap(HuffmanNodeHeap, myChildren, myParent);
		myChildren = myParent;
		myParent = GetParentHeap(myChildren);
	}

}

void MinHeapifyHuffmanNodeHeap(Heap *HuffmanNodeHeap, unsigned int myIndex){
	unsigned int mySwapCandidate = myIndex;
	unsigned int myLeftChildren = GetLeftChildren(myIndex);
	unsigned int myRightChildren = GetRightChildren(myIndex);

	if(myLeftChildren <= GetSizeHeap(HuffmanNodeHeap) && HuffmanNodeHeapCmp(HuffmanNodeHeap, myLeftChildren, mySwapCandidate)){
		mySwapCandidate = myLeftChildren;
	}
	if(myRightChildren <= GetSizeHeap(HuffmanNodeHeap) && HuffmanNodeHeapCmp(HuffmanNodeHeap, myRightChildren, mySwapCandidate)){
		mySwapCandidate = myRightChildren;
	}
	if(mySwapCandidate != myIndex){
		HuffmanNodeHeapSwap(HuffmanNodeHeap, myIndex, mySwapCandidate);
		MinHeapifyHuffmanNodeHeap(HuffmanNodeHeap, mySwapCandidate);
	}
}

Node *DequeueHuffmanNodeHeap(Heap *HuffmanNodeHeap){
	Node *myDequeued = HuffmanNodeHeap->arr[1];

	HuffmanNodeHeapSwap(HuffmanNodeHeap, 1, GetSizeHeap(HuffmanNodeHeap));
	HuffmanNodeHeap->size--;

	MinHeapifyHuffmanNodeHeap(HuffmanNodeHeap, 1);
	return myDequeued;
}

Node *MergeHuffmanNode(Node *firstNode, Node *secondNode){
	Node *newNode = (Node *) malloc(sizeof(Node));
	newNode->data = (HuffmanData *) malloc(sizeof(HuffmanData));
	((HuffmanData *)newNode->data)->key = '*';
	((HuffmanData *)newNode->data)->frequency = ((HuffmanData *)firstNode->data)->frequency + ((HuffmanData *)secondNode->data)->frequency;
	newNode->left = firstNode;
	newNode->right = secondNode;
	return newNode;
}

Heap *BuildHuffmanRoot(Heap *HuffmanNodeHeap){
	while(GetSizeHeap(HuffmanNodeHeap) > 1){
		Node *firstDequeued = DequeueHuffmanNodeHeap(HuffmanNodeHeap);
		Node *secondDequeued = DequeueHuffmanNodeHeap(HuffmanNodeHeap);
		Node *mergedNode = MergeHuffmanNode(firstDequeued, secondDequeued);
		EnqueueHuffmanNodeHeap(HuffmanNodeHeap, mergedNode);
	}
	return HuffmanNodeHeap;
}

//end heap part

Node *CreateNewHuffmanTreeNode(HashTable *FrequencyHashTable, unsigned char myKey){
	Node *newNode = (Node *) malloc(sizeof(Node *));
	newNode->data = (HuffmanData *) malloc(sizeof(HuffmanData));
	((HuffmanData *)newNode->data)->key = myKey;
	((HuffmanData *)newNode->data)->frequency = GetFrequencyHashTable(FrequencyHashTable, myKey);
	newNode->left = newNode->right = NULL;
	return newNode;
}

BinaryTree *BuildHuffmanTree(HashTable *FrequencyHashTable){
	BinaryTree *HuffmanTree = (BinaryTree *) malloc(sizeof(BinaryTree));
	HuffmanTree->root = NULL;

	Heap *HuffmanNodeHeap = CreateHuffmanNodeHeap();
	unsigned int myKey;
	for(myKey = 0; myKey < MAX_HASH_TABLE_SIZE; ++myKey){
		if(hasFrequency(FrequencyHashTable, myKey)){
			Node *newNode = CreateNewHuffmanTreeNode(FrequencyHashTable, myKey);
			EnqueueHuffmanNodeHeap(HuffmanNodeHeap, newNode);
		}
	}

	HuffmanNodeHeap = BuildHuffmanRoot(HuffmanNodeHeap);
	HuffmanTree->root = DequeueHuffmanNodeHeap(HuffmanNodeHeap);

	return HuffmanTree;
}

unsigned int CodificationHash(unsigned char myKey){
	return (unsigned int) myKey;
}

void GenerateByteCodes(Node *root, HashTable *CodificationHashTable, ByteCode *myByteCode){

	if(root->left == NULL && root->right == NULL){
		unsigned char myKey = ((HuffmanData *)root->data)->key;

		ByteCode *newByteCode = (ByteCode *) malloc(sizeof(ByteCode));

		newByteCode->codesize = myByteCode->codesize;
		newByteCode->codestring = (unsigned char *) malloc(myByteCode->codesize * sizeof(unsigned char));
		strcpy(newByteCode->codestring, myByteCode->codestring);

		CodificationHashTable->table[CodificationHash(myKey)] = newByteCode;
		return;
	}

	myByteCode->codesize++;
	myByteCode->codestring = realloc(myByteCode->codestring, (myByteCode->codesize + 1) * sizeof(unsigned char));

	myByteCode->codestring[myByteCode->codesize - 1] = '0';
	myByteCode->codestring[myByteCode->codesize] = '\0';
	GenerateByteCodes(root->left, CodificationHashTable, myByteCode);

	myByteCode->codestring[myByteCode->codesize - 1] = '1';
	myByteCode->codestring[myByteCode->codesize] = '\0';
	GenerateByteCodes(root->right, CodificationHashTable, myByteCode);
	myByteCode->codesize--;
}

HashTable *BuildCodificationHashTable(BinaryTree *HuffmanTree){
	HashTable *newHashTable = (HashTable *) malloc(sizeof(HashTable));
	newHashTable->table = (ByteCode **) malloc( MAX_HASH_TABLE_SIZE * sizeof(ByteCode*));

	ByteCode *myByteCode = (ByteCode *) malloc(sizeof(ByteCode));
	myByteCode->codesize = 0;
	myByteCode->codestring = realloc(myByteCode->codestring, (myByteCode->codesize + 1) * sizeof(unsigned char));
	myByteCode->codestring[myByteCode->codesize] = '\0';

	GenerateByteCodes(HuffmanTree->root, newHashTable, myByteCode);
	return newHashTable;
}

void WriteHuffmanTreeInCompressedFile(FILE *CompressedFile, Node *root, unsigned int *HuffmanTreeSize){
	if(root){
		(*HuffmanTreeSize)++;
		if(root->left == NULL)
			if(((HuffmanData *)root->data)->key == '*' || ((HuffmanData *)root->data)->key == '\\')
				fputc('\\', CompressedFile);
		fputc(((HuffmanData *)root->data)->key, CompressedFile);
		WriteHuffmanTreeInCompressedFile(CompressedFile, root->left, HuffmanTreeSize);
		WriteHuffmanTreeInCompressedFile(CompressedFile, root->right, HuffmanTreeSize);
	}
}

unsigned int SetBit(unsigned int ToSet, unsigned int bitToSet){
	unsigned int mask = (1<<bitToSet);
	return (mask | ToSet);
}

unsigned int IsBitSet(unsigned int ToLook, unsigned int bitToLook){
	return ( ToLook & (1<<bitToLook));
}

void FixHeader(unsigned int bitTrash, unsigned int HuffmanTreeSize, char *CompressedPath){
	FILE *CompressedFile = fopen(CompressedPath, "r+b");
	int HeaderBit, toCount = 7;

	unsigned int ProtoHeader = (bitTrash<<13) | HuffmanTreeSize;
	unsigned char toBrush = 0;

	for(HeaderBit = 15; HeaderBit >= 0; HeaderBit--){
		if(IsBitSet(ProtoHeader, HeaderBit)) toBrush = SetBit(toBrush, toCount);
		toCount--;
		if(toCount == -1){
			toCount = 7;
			fputc(toBrush, CompressedFile);
			toBrush = 0;
		}
	}

	fclose(CompressedFile);
}

void BuildCompressedFile(HashTable *CodificationHashTable, BinaryTree *HuffmanTree, char *toCompressPath, char *CompressedPath){
	FILE *toCompressFile = fopen(toCompressPath, "rb");
	FILE *CompressedFile = fopen(CompressedPath, "wb");

	fputc(0, CompressedFile);
	fputc(0, CompressedFile);

	unsigned int i, HuffmanTreeSize = 0;
	WriteHuffmanTreeInCompressedFile(CompressedFile, HuffmanTree->root, &HuffmanTreeSize);

	unsigned char toRead, toBrush, bitTrash;
	int toCount;
	toRead = toBrush = bitTrash = 0;
	toCount = 7;

	while(1){
		toRead = fgetc(toCompressFile);
		if(feof(toCompressFile)) break;
		ByteCode *myByteCode = (ByteCode *)CodificationHashTable->table[CodificationHash(toRead)];

		for(i = 0; i < myByteCode->codesize; ++i){
			if(myByteCode->codestring[i] == '1'){
				toBrush = SetBit(toBrush, toCount);
			}
			toCount--;
			if(toCount == -1){
				toCount = 7;
				fputc((unsigned char)toBrush, CompressedFile);
				toBrush = 0;
			}
		}
	}

	if((toCount + 1) != 7){
		bitTrash = toCount + 1;
		fputc(toBrush, CompressedFile);
	}

	fclose(toCompressFile);
	fclose(CompressedFile);

	FixHeader(bitTrash, HuffmanTreeSize, CompressedPath);
}

void DebugarBits(Generic bits, size_t size){
	int i;
	for(i = 8*size - 1; i>=0; --i){
		if(IsBitSet(bits, i)) printf("%d", 1);
		else printf("%d", 0);
	}
	printf(" ");
}

void Codification(){
	char toCompressPath[100];
	char CompressedPath[100];

	scanf("%[^\n]s", toCompressPath);
	scanf(" %[^\n]s", CompressedPath);

	HashTable *FrequencyHashTable = BuildFrequencyHashTable(toCompressPath);

	BinaryTree *HuffmanTree = BuildHuffmanTree(FrequencyHashTable);

	HashTable *CodificationHashTable = BuildCodificationHashTable(HuffmanTree);

	BuildCompressedFile(CodificationHashTable, HuffmanTree, toCompressPath, CompressedPath);
	
	return;
}