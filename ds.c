#include <stdio.h>
#include <stdlib.h>
#include "ds.h"


int rear=0,front=0; //for q
int heapend=1; //for heap first empty space
mal_memp queue;
mal_memp	heap;


/*init Q & heap*/
void initDS(){
queue=(mal_memp)malloc(sizeof(mal_mem)*MAXQ);
heap=(mal_memp)malloc(sizeof(mal_mem)*MAXH);
}

/*enque data*/
int enque(mal_mem data){
	rear=(rear+1)%MAXQ;
	if(front+1==rear)
		return 0;
	queue[rear]=data;
	return 1;
}


/*deque data*/
mal_mem deque(){
	mal_mem res;
	if(front==rear)
		return ;
	front=(front+1)%MAXQ;
	res=queue[front];
	return res;
}

/* heap rebuilding after insertion*/
void heapup(){
	int parent,child=heapend-1;
	mal_mem temp;
	parent=child/2;
	while(parent>0){
		if(heap[child].used<heap[parent].used){
			temp=heap[child];
			heap[child]=heap[parent];
			heap[parent]=temp;
			child=parent;
			parent=child/2;
		}
		else break;
	}
}

/*heap rebuilding after deletion*/
void heapdown(){
	int parent=1,lchild,rchild;
	mal_mem temp;
	while(parent<heapend){
		lchild=(parent)*2;
		rchild=(parent)*2+1;
		
		if(heap[lchild].used<heap[parent].used && lchild<heapend){
			temp=heap[lchild];
			heap[lchild]=heap[parent];
			heap[parent]=temp;
			lchild=parent;
		}
		else if(heap[rchild].used<heap[parent].used && rchild<heapend){
			temp=heap[rchild];
			heap[rchild]=heap[parent];
			heap[parent]=temp;
			rchild=parent;
		}
		else break;
	}

}


/*insert data to heap*/
int enheap(mal_mem data){
	if(heapend==MAXH)
		return 0;
	heap[heapend++]=data;
	heapup();
	return 1;
}

/*delete data from heap*/
mal_mem deheap(){
	mal_mem res;
	if(heapend==1)
		return ;
	res= heap[1];
	heap[1]=heap[--heapend];
	heapdown();
	return res;
}
