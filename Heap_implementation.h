#ifndef _HEAP
#define _HEAP

/***********************************************************************
****************************** HEAPS ***********************************
Implementation of HEAP data structres. It will be used just to build the
huffman tree, so it will store internal nodes of huffman.
************************************************************************/

#define MAX_HEAP_SIZE 257

typedef struct HEAP{
	GENERIC *dataArray;
	int heapSize;
	int insertionIndexCounter;
}HEAP;

/***********************************************************************
Creates a pointer to HEAP type and allocates memmory dinamically for an
array with pointers to NODE type.
************************************************************************/
GENERIC HEAP_Create(){
	HEAP *newHeap = (HEAP *) malloc(sizeof(HEAP));
	newHeap->dataArray = (NODE **) calloc(MAX_HEAP_SIZE, sizeof(NODE *));
	newHeap->heapSize = 0;
	newHeap->insertionIndexCounter = 0;
	return newHeap;
}

/***********************************************************************
As a part of the contract, this function return the left child index.
************************************************************************/
int HEAP_GetLeft(int parentIndex){
	int leftChildIndex = parentIndex << 1;
	return leftChildIndex;
}

/***********************************************************************
Simply returns the right child index. Self-explaned.
************************************************************************/
int HEAP_GetRight(int parentIndex){
	int rightChildIndex = (parentIndex << 1) + 1;
	return rightChildIndex;
}

/***********************************************************************
Again, self-explaned function.
************************************************************************/
int HEAP_GetParent(int childIndex){
	int parentIndex = childIndex >> 1;
	return parentIndex;
}

/***********************************************************************
Return the size of the heap.
************************************************************************/
int HEAP_GetSize(HEAP *myHeap){
	return myHeap->heapSize;
}

/***********************************************************************
Returns an GENERIC data of the heap given his index.
************************************************************************/
GENERIC HEAP_GetData(HEAP *myHeap, int dataIndex){
	return myHeap->dataArray[dataIndex];
}

/***********************************************************************
Simply swap two elements of the heap. Using GENERIC type, this function
is more generic to swap any type of data that heap can store.
************************************************************************/
GENERIC HEAP_SwapElement(HEAP *myHeap, int firstIndex, int secondIndex){
	GENERIC auxiliarVariable = myHeap->dataArray[firstIndex];
	myHeap->dataArray[firstIndex] = myHeap->dataArray[secondIndex];
	myHeap->dataArray[secondIndex] = auxiliarVariable;
}


/***********************************************************************
Compare two elementes of type NODE and return 1 (true) if the first node
has priority, 0 (false) otherwise. In other words: Nodes with lower
frequency must be at the top of the heap. When the two nodes have the
same frequency, the node with byteKey equals to '*' must have prioriy.
When the two nodes have the same frequency and the same byteKey, then
the node with insertion order greater must have priority.
************************************************************************/
GENERIC HEAP_CompareElement(GENERIC firstData, GENERIC secondData){
	HUFFMANDATA *firstHuffmanData = (HUFFMANDATA *) ((NODE *) firstData)->nodeData;
	HUFFMANDATA *secondHuffmanData = (HUFFMANDATA *) ((NODE *) secondData)->nodeData;

	unsigned char starByte = '*';

	if(firstHuffmanData->frequencyValue < secondHuffmanData->frequencyValue){
		return 1;
	}
	else if(firstHuffmanData->frequencyValue == secondHuffmanData->frequencyValue){
		if(firstHuffmanData->byteKey != starByte && firstHuffmanData->byteKey != starByte){
			if(firstHuffmanData->insertionIndex > secondHuffmanData->insertionIndex) return 1;
			else return 0;
		}
		else if(firstHuffmanData->byteKey == starByte && secondHuffmanData->byteKey == starByte){
			if(firstHuffmanData->insertionIndex > secondHuffmanData->insertionIndex) return 1;
			else return 0;
		}
		else if(firstHuffmanData->byteKey == starByte && secondHuffmanData->byteKey != starByte) return 1;
		else return 0;
	}
	else return 0;

}

/***********************************************************************
Heapift function based in compare function explained before.
************************************************************************/
GENERIC HEAP_Heapify(HEAP *myHeap, int heapifyIndex){
	int swapCandidate = heapifyIndex;
	int leftChildIndex = HEAP_GetLeft(heapifyIndex);
	int rightChildIndex = HEAP_GetRight(heapifyIndex);

	int myHeapSize = HEAP_GetSize(myHeap);

	GENERIC heapifyData = HEAP_GetData(myHeap, heapifyIndex);
	GENERIC leftChildData = HEAP_GetData(myHeap, leftChildIndex);
	GENERIC rightChildData = HEAP_GetData(myHeap, rightChildIndex);

	if(leftChildIndex <= myHeapSize && HEAP_CompareElement(leftChildData, heapifyData)){
		swapCandidate = leftChildIndex;
		heapifyData = leftChildData;
	}

	if(rightChildIndex <= myHeapSize && HEAP_CompareElement(rightChildData, heapifyData)){
		swapCandidate = rightChildIndex;
		heapifyData = rightChildData;
	}
	if(swapCandidate != heapifyIndex){
		HEAP_SwapElement(myHeap, swapCandidate, heapifyIndex);
		HEAP_Heapify(myHeap, swapCandidate);
	}
}

/***********************************************************************
Basic enqueue functions based in compare functions explained before.
************************************************************************/
GENERIC HEAP_Enqueue(HEAP *myHeap, GENERIC dataToEnqueue){
	myHeap->heapSize++;
	myHeap->insertionIndexCounter++;
	myHeap->dataArray[HEAP_GetSize(myHeap)] = dataToEnqueue;

	int parentIndex = HEAP_GetParent(HEAP_GetSize(myHeap));
	int childIndex = HEAP_GetSize(myHeap);

	while(parentIndex >= 1 && HEAP_CompareElement(HEAP_GetData(myHeap, childIndex), HEAP_GetData(myHeap, parentIndex))){
		HEAP_SwapElement(myHeap, parentIndex, childIndex);
		
		childIndex = parentIndex;
		parentIndex = HEAP_GetParent(childIndex);
	}
}

/***********************************************************************
Dequeues an element with higher priority then calls the heapify function.
************************************************************************/
GENERIC HEAP_Dequeue(HEAP *myHeap){
	GENERIC dequeuedElement = myHeap->dataArray[1];
	HEAP_SwapElement(myHeap, 1, HEAP_GetSize(myHeap));
	myHeap->heapSize--;
	HEAP_Heapify(myHeap, 1);
	return dequeuedElement;
}

#endif