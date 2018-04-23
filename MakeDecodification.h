Node *RebuildHuffmanTreeNode(unsigned char byte, FILE *CompressedFile){
	Node *newNode = (Node *) malloc(sizeof(Node));
	newNode->left = newNode->right = NULL;
	if(byte == '\\'){
		unsigned char bits = fgetc(CompressedFile);
		newNode->data = bits;
	}
	else if(byte == '*'){
		newNode->data = byte;
		byte = fgetc(CompressedFile);
		newNode->left = RebuildHuffmanTreeNode(byte, CompressedFile);
		byte = fgetc(CompressedFile);
		newNode->right = RebuildHuffmanTreeNode(byte, CompressedFile);
	}
	else{
		newNode->data = byte;
	}
	return newNode;
}

BinaryTree *RebuildHuffmanTree(FILE *CompressedFile){
	BinaryTree *HuffmanTree = (BinaryTree *) malloc(sizeof(BinaryTree));
	HuffmanTree->root = NULL;

	unsigned char byte;
	byte = fgetc(CompressedFile);
	HuffmanTree->root = RebuildHuffmanTreeNode(byte, CompressedFile);
}

void Decodification(){
	char CompressedPath[100];
	char DecompressedPath[100];
	scanf("%[^\n]s", CompressedPath);
	scanf(" %[^\n]s", DecompressedPath);

	FILE *CompressedFile = fopen(CompressedPath, "rb");
	FILE *DecompressedFile = fopen(DecompressedPath, "wb");

	unsigned char byte0 = fgetc(CompressedFile);
	unsigned char byte1 = fgetc(CompressedFile);

	byte0 = (byte0>>5);

	BinaryTree *HuffmanTree = RebuildHuffmanTree(CompressedFile);

	Node *aux = HuffmanTree->root;
	int counter = 7;
	unsigned char bits;
	unsigned char antibits;

	bits = fgetc(CompressedFile);
	antibits = fgetc(CompressedFile);
	while(1){
		if(aux->left == NULL){
			fputc(aux->data, DecompressedFile);
			aux = HuffmanTree->root;
		}
		else if(IsBitSet(bits, counter)){
			aux = aux->right;
			counter--;
			if(counter == -1){
				counter = 7;
				bits = antibits;
				antibits = fgetc(CompressedFile);
				if(feof(CompressedFile)) break;
			}
		}
		else{
			aux = aux->left;
			counter--;
			if(counter == -1){
				counter = 7;
				bits = antibits;
				antibits = fgetc(CompressedFile);
				if(feof(CompressedFile)) break;
			}
		}
	}

	int i;
	int o = byte0;
	for(i = 7; i >= o-1; --i){
		if(aux->left == NULL){
			fputc(aux->data, DecompressedFile);
			aux = HuffmanTree->root;
		}	
		else if(IsBitSet(bits, i)) aux = aux->right;
		else aux = aux->left;
	}

	printf("%d\n", o);

	fclose(CompressedFile);
	fclose(DecompressedFile);
	return;
}