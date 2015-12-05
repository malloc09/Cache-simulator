#include <stdio.h>
#include <stdlib.h>
#include "ds.h"


int rear=0,front=0; //for q
int Qnum=0;
int heapend; //for heap first empty space
mal_memp queue;
mal_memp	heap;

void printH(){
	int i;
	for(i=1; i<heapend; i++){
		printf("%d %d\n", heap[i].page_num, heap[i].used);
	}
}

void initQ(){
	queue=(mal_memp)malloc(sizeof(mal_mem)*MAXQ);
	rear=0;
	front=0;
	Qnum=0;
}
void desQ(){
	if(queue!=NULL){
		free(queue);
		queue=NULL;
	}
}
int is_fullQ(){
 if(Qnum==MAXQ){
	 return 1;
 }
 else
	 return 0;
}


int is_existQ(mal_mem data){
	int pageno=data.page_num;
	int i;

	for(i=front;i<front+Qnum;i++){	/* front부터 rear까지 검색으로 바꿈 */
		if(pageno==queue[i%MAXQ].page_num)
			return i%MAXQ;
	}
	return -1;
}


/*enque data*/
int enque(mal_mem data){
	if(!is_fullQ()){
	queue[rear]=data;	/* rear에 데이터를 넣고 증가시키는걸로 바꿈 */
	rear=(rear+1)%MAXQ;
	Qnum++;
	return 1;
	}
	else
		return 0;
}


/*deque data*/
int deque(){
	if(Qnum==0)	/* Q의 멤버수가 0일 때로 바꿈 */
		return -1;
	front=(front+1)%MAXQ;
	Qnum--;
	return (front-1)%MAXQ;
}
void initH(){
	heap=(mal_memp)malloc(sizeof(mal_mem)*(MAXH+1));
	heapend=1;
}
void desH(){
	if(heap!=NULL){
		free(heap);
		heap=NULL;
	}
}
int is_existH(mal_mem data){
	int pageno=data.page_num;
	int i;
	for(i=1;i<heapend;i++){
		if(pageno==heap[i].page_num)
			return i;
	}
	return -1;
}


int is_fullH(){
	if(heapend-1==MAXH){
		return 1;
	}
	else
		return 0;
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
	while(parent<heapend/2){
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
	if(!is_fullH()){
	heap[heapend++]=data;
	heapup();
	return 1;
	}
	else
		return 0;
}

/*delete data from heap*/
int deheap(){
	if(heapend==1)
		return 0;
	heap[1]=heap[--heapend];
	heapdown();
	return 1;
}
