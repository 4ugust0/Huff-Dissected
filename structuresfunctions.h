
//----- FOR HEAP <-> BASIC OPERATIONS WITH INTEGERS ---------------------------//
void myInitHeap(Heap *h){
	h->arr = (int *) calloc(MAX_HEAP_SIZE, sizeof(int));
	h->size = 0;
}

int cmpFunction(Generic a, Generic b){
	return ((int)a < (int)b);
}

void mySwap(Heap *h, int i, int j){
	Generic aux = h->arr[i];
	h->arr[i] = h->arr[j];
	h->arr[j] = aux;
}

int myF2(Generic key){
	return (int) key;
}

void destroyHeap(Heap *h){
	free(h->arr);
}
//-----------------------------------------------------------------------------//

//----- FOR HASH TABLE <-> KEEP TRACKING THE FREQUENCY OF BYTES OF ARCHIVE ----//
void frequencyInit(HashTable *ht){
	ht->table = (long long *) calloc(MAX_HASH_SIZE, sizeof(long long));
}

int frequencyHash(unsigned char c){
	return (int)c;
}

int frequencyItem(HashTable *ht, unsigned char c, Generic (*hashfunc)()){
	return ht->table[(int)hashfunc(c)] + 1;
}
//-----------------------------------------------------------------------------//
//----- FOR HEAPS <-> NODES HEAP TO MAKE HUFFMAN TREE--------------------------//
void nodesHeapInit(Heap *h){
	h->arr = (Node **) calloc(MAX_HASH_SIZE, sizeof(Node *));
	h->size = 0;
}

void nodesHeapDestroy(Heap *h){
	int i = 0;
	for (i = 0; i <= h->size; ++i) free(h->arr[i]);
	free(h->arr);
}

Generic nodesHeapReturn(Generic key){
	return key;
}

int nodesHeapCmp(Node *a, Node *b){
	CharFreq *aa = (CharFreq *) a->data;
	CharFreq *bb = (CharFreq *) b->data;

	if(aa->frequency < bb->frequency) return 1;
	else if(aa->frequency == bb->frequency && aa->key == '*') return 1;
	else return 0;
}

void nodesHeapSwap(Heap *h, int i, int j){
	Generic aux = h->arr[i];
	h->arr[i] = h->arr[j];
	h->arr[j] = aux;
}

//-----------------------------------------------------------------------------//

//----- FOR CHAR FREQ <-> CREATE A STRUCTURES WITH A CHAR AND HIS FREQUENCY ---//
CharFreq *createCharFreq(char key, long long frequency){
	CharFreq *newCharFreq = (CharFreq *) malloc(sizeof(CharFreq));
	newCharFreq->key = (char) key;
	newCharFreq->frequency = frequency;
	return newCharFreq;
}
//-----------------------------------------------------------------------------//

//----- FOR BINARY TREE <-> HUFFMAN TREE --------------------------------------//
void printnodesHuffmanTree(Node *root){
	printf("%c", ((CharFreq *)root->data)->key);
}

void btprint(Node *root){
	printf("%d ", (int)root->data);
}

void myBTinit(BinaryTree *bt){
	bt->root = NULL;
}

void destroyNode(Node *root){
	free(root->data);
	free(root);
}
//-----------------------------------------------------------------------------//

//----- FOR HASH TABLE WITH KEYCODES ------//
void codificationInit(HashTable *ht){
	ht->table = (Keycode **) calloc(MAX_HASH_SIZE, sizeof(Keycode*));
}
