#ifndef __MAL__CACHE__
#define __MAL__CACHE__

#define MEMACC 100
#define STOACC 10000000 


typedef struct mal_mem{
	int page_num;
	int dirty;
	double used;
}mal_mem;

typedef mal_mem* mal_memp;


int mem_access(int no, char mode);
int stor_access(int no);

#endif
