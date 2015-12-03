#include <stdio.h>
#include <time.h>
#include "cache.h"

mal_mem MEM[584];
int pos;
int T;

int mem_access(int no, char mode){
	if(pos==584){
		printf("MEM FULL\n");
		pos++;
	}
	struct timespec req = {0, 100};
	nanosleep(&req, NULL);
	int i;
	char tag=0;
	for(i=0; i<584; i++){
		if(no == MEM[i].page_num){
			tag=1;
			MEM[i].dirty = 1;
			MEM[i].used = T++;
			break;
		}
	}
	
	if(!tag)	stor_access(no);
	if(pos < 584)	MEM[pos++].page_num = no;
	return 0;
}

int stor_access(int no){
	struct timespec req = {0, 10000000};
	nanosleep(&req, NULL);
	printf("%dACC\n", pos);
	return 0;
}

int main(){
	time_t S, T;
	double dur;
	char mode;
	int pageno;
	int garbage;
	
	FILE *fp = fopen("input.txt", "r");
	S=clock();
	while(!feof(fp)){
		fscanf(fp, "%c %d %c %d\n", &mode, &pageno, (char*)&garbage, &garbage);
		mem_access(pageno, mode);
	}
	T=clock();

	dur = (double)(T-S)/(CLOCKS_PER_SEC);

	printf("%lf\n", dur);
}
