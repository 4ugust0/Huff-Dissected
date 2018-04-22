
unsigned int getThrash(unsigned char b0){
	return (b0>>5);
}

unsigned int getSizeHuffmanTree(unsigned char b0, unsigned char b1){
	b0 = b0<<3;
	b0 = b0>>3;
	unsigned int aux0 = b0;
	unsigned int aux1 = b1;
	return ((aux0<<8) | aux1);

}

Node *rebuildHuffmanTree(Node *root, unsigned char byte, FILE *myFile){
	Node *newNode = (Node *) malloc(sizeof(Node));
	newNode->left = newNode->right = NULL;
	newNode->data = byte;
	if(byte == '*'){
		byte = fgetc(myFile);
		newNode->left = rebuildHuffmanTree(newNode->left, byte, myFile);
		byte = fgetc(myFile);
		newNode->right = rebuildHuffmanTree(newNode->right, byte, myFile);
	}
	else if(byte == '\\'){
		byte = fgetc(myFile);
		newNode->data = byte;
	}
	return newNode;
}

void nodeprint(Node *root){
	printf("%c", root->data);
}

void makeDecodification(){
	char myString[100];
	scanf("%[^\n]s", myString);

	FILE *filetoDecompress = fopen(myString, "rb");
	FILE *fileDecompressed = fopen("decompressed", "wb");

	unsigned char byte0, byte1;
	byte0 = fgetc(filetoDecompress);
	byte1 = fgetc(filetoDecompress);

	unsigned int bitThrash = getThrash(byte0);
	unsigned int sizeHuffmanTree = getSizeHuffmanTree(byte0, byte1);

	BinaryTree HuffmanTree;
	byte0 = fgetc(filetoDecompress);
	HuffmanTree.root = rebuildHuffmanTree(HuffmanTree.root, byte0, filetoDecompress);

	Node *aux = HuffmanTree.root;

	byte0 = fgetc(filetoDecompress);
	byte1 = fgetc(filetoDecompress);
	unsigned int counter = 0;
	while(1){
		if(isEmpty(aux->left)){
			fputc(aux->data, fileDecompressed);
			aux = HuffmanTree.root;
		}
		else if(isbitset(byte0, 7 - counter%8)){
			counter++;
			aux = aux->right;
			if(counter%8 == 0){
				byte0 = byte1;
				byte1 = fgetc(filetoDecompress);
				if(feof(filetoDecompress)) break;
			}
		}
		else{
			counter++;
			aux = aux->left;
			if(counter%8 == 0){
				byte0 = byte1;
				byte1 = fgetc(filetoDecompress);
				if(feof(filetoDecompress)) break;
			}
		}
	}

	unsigned int i;
	for(i=0; i <= 8 - bitThrash; ++i){
		if(isEmpty(aux->left)){
			fputc(aux->data, fileDecompressed);
		}
		else if(isbitset(byte0, 7 - counter%8)){
			counter++;
			aux = aux->right;
		}
		else{
			counter++;
			aux = aux->left;
		}
	}

	fclose(filetoDecompress);
	fclose(fileDecompressed);
}