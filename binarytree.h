#define isEmpty(a) (((a==NULL)))

typedef struct node{
	Generic data;
	struct node *left;
	struct node *right;
}Node;

typedef struct BinaryTree{
	Node *root;
}BinaryTree;

void initBinaryTree(BinaryTree *bt, Generic (*initfunc)()){
	initfunc(bt);
}

void preorderBinaryTree(Node *root, Generic (*func)()){
	if(!isEmpty(root)){
		func(root);
		preorderBinaryTree(root->left, func);
		preorderBinaryTree(root->right, func);
	}
}

void inorderBinaryTree(Node *root, Generic (*func)()){
	if(!isEmpty(root)){
		inorderBinaryTree(root->left, func);
		func(root);
		inorderBinaryTree(root->right, func);
	}
}

void postorderBinaryTree(Node *root, Generic (*func)()){
	if(!isEmpty(root)){
		postorderBinaryTree(root->left, func);
		postorderBinaryTree(root->right, func);
		func(root);
	}
}

Node *createNode(Generic data, Node *left, Node *right){
	Node *newNode = (Node *) malloc(sizeof(Node));
	newNode->data = data;
	newNode->left = left;
	newNode->right = right;
	return newNode;
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