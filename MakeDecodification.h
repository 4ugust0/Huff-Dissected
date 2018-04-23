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

unsigned char WriteDecompressedFile(FILE *CompressedFile, FILE *DecompressedFile, BinaryTree *HuffmanTree, Node *AuxiliarNode){
	unsigned char ByteToBrush;
	unsigned char NextByteToBrush;

	int CurrentBit = 7;

	ByteToBrush = fgetc(CompressedFile);
	NextByteToBrush = fgetc(CompressedFile);
	while(1){
		if(AuxiliarNode->left == NULL){
			fputc(AuxiliarNode->data, DecompressedFile);
			AuxiliarNode = HuffmanTree->root;
		}
		else if(IsBitSet(ByteToBrush, CurrentBit)){
			AuxiliarNode = AuxiliarNode->right;
			CurrentBit--;
			if(CurrentBit == -1){
				CurrentBit = 7;
				ByteToBrush = NextByteToBrush;
				NextByteToBrush = fgetc(CompressedFile);
				if(feof(CompressedFile)) break;
			}
		}
		else{
			AuxiliarNode = AuxiliarNode->left;
			CurrentBit--;
			if(CurrentBit == -1){
				CurrentBit = 7;
				ByteToBrush = NextByteToBrush;
				NextByteToBrush = fgetc(CompressedFile);
				if(feof(CompressedFile)) break;
			}
		}
	}
	return ByteToBrush;
}

void FilterThrash(FILE *CompressedFile, FILE *DecompressedFile, BinaryTree *HuffmanTree, Node *AuxiliarNode,
							unsigned char HeaderByte0, unsigned char ByteToBrush){
	int CurrentBit;
	int HeaderInt0 = HeaderByte0;
	for(CurrentBit = 7; CurrentBit >= HeaderInt0-1; --CurrentBit){
		if(AuxiliarNode->left == NULL){
			fputc(AuxiliarNode->data, DecompressedFile);
			AuxiliarNode = HuffmanTree->root;
		}	
		else if(IsBitSet(ByteToBrush, CurrentBit)) AuxiliarNode = AuxiliarNode->right;
		else AuxiliarNode = AuxiliarNode->left;
	}
}

void Decodification(){
	char CompressedPath[100];
	char DecompressedPath[100];
	scanf("%[^\n]s", CompressedPath);
	scanf(" %[^\n]s", DecompressedPath);

	FILE *CompressedFile = fopen(CompressedPath, "rb");
	FILE *DecompressedFile = fopen(DecompressedPath, "wb");

	unsigned char HeaderByte0 = fgetc(CompressedFile);
	unsigned char HeaderByte1 = fgetc(CompressedFile);
	HeaderByte0 = (HeaderByte0>>5);

	BinaryTree *HuffmanTree = RebuildHuffmanTree(CompressedFile);

	Node *AuxiliarNode = HuffmanTree->root;
	unsigned char ByteToBrush;

	ByteToBrush = WriteDecompressedFile(CompressedFile, DecompressedFile, HuffmanTree, AuxiliarNode);
	FilterThrash(CompressedFile, DecompressedFile, HuffmanTree, AuxiliarNode, HeaderByte0, ByteToBrush);

	fclose(CompressedFile);
	fclose(DecompressedFile);
	return;
}