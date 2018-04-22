typedef struct heap{
	Generic *arr;
	int size;
}Heap;

void initHeap(Heap *h, Generic (*init)()){
	init(h);
}

void enqueueHeap(Heap *h, Generic key, Generic (*cmpfunc)(),
					Generic (*swapfunc)(), Generic (*retfunc)()){
	h->size++;
	h->arr[getSize(h)] = retfunc(key);

	int children = getSize(h);
	int parent = getParent(children);

	while(parent >= 1 && cmpfunc(getData(h,children), getData(h,parent))){
		swapfunc(h, children, parent);
		children = getParent(children);
		parent = getParent(parent);
	}

}

void heapifyHeap(Heap *h, int i, Generic (*cmpfunc)(), Generic (*swapfunc)()){
	int swap_candidate = i;
	int left = getLeft(i);
	int right = getRight(i);

	if(left <= getSize(h) && cmpfunc(getData(h, left), getData(h, swap_candidate))){
		swap_candidate = left;
	}
	if(right <= getSize(h) && cmpfunc(getData(h, right), getData(h, swap_candidate))){
		swap_candidate = right;
	}
	if(swap_candidate != i){
		swapfunc(h, i, swap_candidate);
		heapifyHeap;
	}
}

Generic dequeueHeap(Heap *h, Generic (*cmpfunc)(), Generic (*swapfunc)()){
	Generic dequeued = getData(h,1);
	swapfunc(h, 1, getSize(h));
	h->size--;
	heapifyHeap(h, 1, cmpfunc, swapfunc);
	return dequeued;
}

void DestroyHeap(Heap *h, Generic (*destroyfunc)()){
	destroyfunc(h);
}