void FrequencyTableInit(HashTable *FrequencyHashTable){
	FrequencyHashTable->table = (unsigned long long *) NULL;
	FrequencyHashTable->table = (unsigned long long *) malloc(MAX_HASH_TABLE_SIZE * sizeof(unsigned long long));
}

unsigned int FrequencyHash(unsigned char myKey){
	return (unsigned int) myKey;
}

void PutFrequencyHashTable(HashTable *FrequencyHashTable, unsigned char myKey){
	FrequencyHashTable->table[FrequencyHash(myKey)]++;
}

unsigned int hasFrequency(HashTable *FrequencyHashTable, unsigned int i){
	return FrequencyHashTable->table[i];
}

unsigned long long int GetFrequencyHashTable(HashTable *FrequencyHashTable, unsigned char myKey){
	return FrequencyHashTable->table[FrequencyHash(myKey)];
}

HashTable *BuildFrequencyHashTable(char *myString){
	HashTable *NewHashTable = (HashTable *) malloc(sizeof(HashTable));
	NewHashTable->table = (unsigned long long) malloc(MAX_HASH_TABLE_SIZE * sizeof(unsigned long long));

	FILE *myFile = fopen(myString, "rb");

	unsigned char myByte;
	while(1){
		myByte = fgetc(myFile);
		if(feof(myFile)) break;
		PutFrequencyHashTable(NewHashTable, myByte);
	}

	fclose(myFile);
	return NewHashTable;
}

void FrequencyHashTableDestroy(HashTable *FrequencyHashTable){
	free(FrequencyHashTable->table);
	free(FrequencyHashTable);
}
