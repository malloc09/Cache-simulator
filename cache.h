#ifndef __MAL__CACHE__
#define __MAL__CACHE__

typedef struct mal_mem{
	int page_num;
	int dirty;
	int used;
}mal_mem;

int mem_access(int no, char mode);
int stor_access(int no);

#endif
