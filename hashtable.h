#define MAX_HASH_SIZE 256

typedef struct hashtable{
	Generic *table;
}HashTable;

void initHashTable(HashTable *ht, Generic (*init)()){
	init(ht);
}

void putHashTable(HashTable *ht, Generic key, Generic (*hashfunc)(), Generic (*itemfunc)()){
	ht->table[(int)hashfunc(key)] = itemfunc(ht, key, hashfunc);
}

Generic getHashTable(HashTable *ht, Generic key, Generic (*hashfunc)()){
	return ht->table[(int)hashfunc(key)];
}

void destroyHashTable(HashTable *ht){
	free(ht->table);
}

