#include <stdio.h>
#include <time.h>
#include "cache.h"
#include "ds.h"

int Time,miss,total,stoacc;
int rcount,wcount;

int mem_access_FIFO(int no, char mode){

	mal_mem data;
	data.page_num=no;
	if(mode=='W') data.dirty=1;
	else data.dirty=0;

	Time+=MEMACC;
	int exist=is_existQ(data);
	if(mode=='R'){
		rcount++;
		if(exist!=-1){
			queue[exist].used=Time;
		}
		else{
			miss++;
			if(is_fullQ()) deque();
			Time+=STOACC;
			stoacc++;
			data.used=Time;
			enque(data);
		}
	}
	else{
		wcount++;
		if(exist!=-1){
			queue[exist].used=Time;
			Time+=STOACC;
			stoacc++;
		}
		else{
			miss++;
			Time+=STOACC;
			stoacc++;
		}
	}
	return 0;
}
int mem_access_LRU(int no, char mode){

	mal_mem data;
	data.page_num=no;
	if(mode=='W') data.dirty=1;
	else data.dirty=0;
	data.used=Time;  //init data

	Time+=MEMACC;
	int exist=is_existH(data);
	if(mode=='R'){
		rcount++;
		if(exist!=-1){
			heap[exist].used=Time;
		}
		else{
			miss++;
			if(is_fullH()) deheap();
			Time+=STOACC;
			stoacc++;
			data.used=Time;
			enheap(data);
		}
	}
	else{
		wcount++;
		if(exist!=-1){
			heap[exist].used=Time;
			Time+=STOACC;
			stoacc++;
		}
		else{
			miss++;
			Time+=STOACC;
			stoacc++;
		}
	}	return 0;
}
int SimWT_FIFO(){
	Time=0;
	miss=0;
	total=0;
	stoacc=0;
	wcount=rcount=0;
	char mode;
	int pageno;
	int garbage;
	initQ();
	FILE *fp = fopen("input.txt", "r");
	while(!feof(fp)){
		fscanf(fp, "%c %d %c %d\n", &mode, &pageno, (char*)&garbage, &garbage);
		total++;
		mem_access_FIFO(pageno, mode);
	}
	fclose(fp);
	printf("=====  WriteThrough FIFO  =====\n");
	printf("TOTAL %d WRITE %d READ %d\n",total,wcount,rcount);
	printf("STORAGE ACCESS : %d\n",stoacc);
	printf("MISS %d HIT RATIO %.2f\n",miss,(total-miss)/(double)total);
	printf("TIME : %dms\n\n",Time/1000000);
	desQ();
	return 0;
}
int SimWT_LRU(){
	Time=0;
	miss=0;
	total=0;
	stoacc=0;
	wcount=rcount=0;
	char mode;
	int pageno,i;
	int garbage;
	char garbage1;
	initH();
	FILE *fp = fopen("input.txt", "r");
	while(!feof(fp)){
		fscanf(fp, "%c %d %c %d\n", &mode, &pageno, &garbage1, &garbage);
		total++;
		mem_access_LRU(pageno, mode);
	}
	fclose(fp);
	printf("=====  WriteThrough LRU  =====\n");
	printf("TOTAL %d WRITE %d READ %d\n",total,wcount,rcount);
	printf("STORAGE ACCESS : %d\n",stoacc);
	printf("MISS %d HIT RATIO %.2f\n",miss,(total-miss)/(double)total);
	printf("TIME : %dms\n\n",Time/1000000);
	desH();
	return 0;
}
int main(){
	SimWT_FIFO();
	SimWT_LRU();
	return 0;
}
