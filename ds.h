#ifndef __MAL__QUEUE__
#define __MAL__QUEUE__

#include "cache.h"

#define MAXQ 584
#define MAXH 585 //not use 0


extern int rear,front;
extern mal_memp queue,heap;
extern int heapend;

void initDS();
int enque(mal_mem data);
int deque();
int enheap(mal_mem data);
int deheap();


#endif
