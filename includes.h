#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define MAX_HEAP_SIZE 512
#define MAX_HASH_SIZE 256

#define Generic void*
#define getSize(a) (( a->size ))
#define getParent(a) (( a>>1 ))
#define getData(a,b) (( a->arr[b] ))
#define getLeft(a) ((a<<1))
#define getRight(a) (((a<<1) + 1))
#define modp2(a,b) (((a&((b-1)))))
#define isEmpty(a) (((a==NULL)))


#include "hashtable.h"
#include "heap.h"
#include "binarytree.h"

#include "basicstructures.h"

#include "structuresfunctions.h"
#include "codification.h"
#include "decodification.h"
