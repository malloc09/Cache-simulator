#ifndef __MAL__QUEUE__
#define __MAL__QUEUE__

#include "cache.h"

#define MAXQ 2
#define MAXH 2 //not use 0

extern mal_memp queue,heap;
extern int rear,front,heapend,Qnum;

void printH();
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


#endif
