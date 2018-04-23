typedef struct node{
	Generic data;
	struct node *left;
	struct node *right;
}Node;

typedef struct binarytree{
	Node *root;
}BinaryTree;

typedef struct hashtable{
	Generic *table;
}HashTable;

typedef struct heap{
	Generic *arr;
	unsigned int size;
}Heap;

typedef struct huffmandata{
	unsigned char key;
	unsigned long long frequency;
}HuffmanData;

typedef struct bytecode{
	unsigned int codesize;
	unsigned char *codestring;
}ByteCode;
