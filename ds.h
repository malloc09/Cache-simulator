#ifndef __MAL__QUEUE__
#define __MAL__QUEUE__

#include "cache.h"


extern mal_memp queue,heap;
extern int rear,front,heapend,Qnum;
extern int MAXQ,MAXH;
void initQ();
void desQ();
void initH();
void desH();
int is_fullQ();
int is_fullH();
int is_existQ(mal_mem data);
int is_existH(mal_mem data);
int enque(mal_mem data);
int deque();
int enheap(mal_mem data);
int deheap();
void heapify(int i);

#endif
