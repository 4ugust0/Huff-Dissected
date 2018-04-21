void codification(){
	HashTable frequencyTable;
	initHashTable(&frequencyTable, frequencyInit);

	char string[100];
	printf("digite o caminho do arquivo: ");
	scanf("%[^\n]s", string);;
	
	FILE *myfile = fopen(string, "rb");

	

	fclose(myfile);
	destroyHashTable(&frequencyTable);
}