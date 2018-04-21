//----- FOR HASH TABLE <-> KEEP TRACKING THE FREQUENCY OF BYTES OF ARCHIVE -----//

int myHash(unsigned char c){
	return (int)c;
}

int myFrequency(HashTable *ht, Generic key, Generic (*myHash)()){
	return ht->table[(int)myHash(key)] + 1;
}

void myInitHashTable(HashTable *ht){
	ht->table = (int *) malloc(MAX_HASH_SIZE * sizeof(int));
}
//-----------------------------------------------------------------------------//

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

//----- FOR HASH TABLE <-> COUNT FREQUENCIES ----------------------------------//
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