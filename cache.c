#include <stdio.h>
#include <time.h>
#include "cache.h"
#include "ds.h"

#define filename "input.txt"

double Time;
int miss,total,stoacc;
int rcount,wcount;

int mem_access_FIFO_WT(int no, char mode){
	mal_mem data;
	data.page_num=no;
	data.used=Time;
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

int mem_access_FIFO_WB(int no, char mode){
 	int deQ;
	mal_mem data;
	data.page_num = no;
	Time += MEMACC;

	int exist = is_existQ(data);
	if(mode=='R'){
		rcount++;
		if(exist != -1)
			queue[exist].used=Time;
		else{
			miss++;
			if(is_fullQ())	deQ = deque();
			if(queue[deQ].dirty == 1){
				Time+=STOACC;
				stoacc++;
			}
			Time+=STOACC;
			stoacc++;
			data.used = Time;
			data.dirty = 0;
			enque(data);
		}
	}
	else{
		wcount++;
		if(exist != -1){
			queue[exist].used=Time;
			queue[exist].dirty=1;
		}
		else{
			miss++;
			if(is_fullQ()){	
				deQ = deque();
				if(queue[deQ].dirty == 1){
					Time+=STOACC;
					stoacc++;
				}
			}
			Time+=STOACC;
			stoacc++;
			data.used = Time;
			data.dirty = 1;
			enque(data);
		}
	}
	return 0;
}
int mem_access_LRU_WT(int no, char mode){
	//printH();
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
			heapify(exist);
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

int mem_access_LRU_WB(int no, char mode){
	int deH;
	mal_mem data;
	data.page_num=no;
	data.used=Time;

	Time+=MEMACC;
	int exist=is_existH(data);
	if(mode=='R'){
		rcount++;
		if(exist != -1)
			heap[exist].used=Time;
		else{
			miss++;
			if(is_fullH())	deH = deheap();
			if(heap[deH].dirty == 1){
				Time+=STOACC;
				stoacc++;
			}
			Time+=STOACC;
			stoacc++;
			data.used = Time;
			data.dirty = 0;
			enheap(data);
		}
	}
	else{
		wcount++;
		if(exist != -1){
			heap[exist].used=Time;
			heap[exist].dirty=1;
		}
		else{
			miss++;
			if(is_fullH()){
				deH = deheap();
				if(heap[deH].dirty == 1){
					Time+=STOACC;
					stoacc++;
				}
			}
			Time+=STOACC;
			stoacc++;
			data.used = Time;
			data.dirty = 1;
			enheap(data);
		}
	}
	return 0;
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
	FILE *fp = fopen(filename, "r");
	while(!feof(fp)){
		fscanf(fp, "%c %d %c %d\n", &mode, &pageno, (char*)&garbage, &garbage);
		total++;
		mem_access_FIFO_WT(pageno, mode);
		/*for(int i=front;i<front+Qnum;i++)
			printf("%d ",queue[i%MAXQ].page_num);
		printf("\n");*/

	}
	fclose(fp);
	printf("=====  WriteThrough FIFO  =====\n");
	printf("TOTAL %d WRITE %d READ %d\n",total,wcount,rcount);
	printf("STORAGE ACCESS : %d\n",stoacc);
	printf("MISS %d HIT RATIO %.2f\n",miss,(total-miss)/(double)total);
	printf("TIME : %.2lfms\n\n",Time/10000);
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
	int pageno;
	int garbage;
	char garbage1;
	initH();
	FILE *fp = fopen(filename, "r");
	while(!feof(fp)){
		fscanf(fp, "%c %d %c %d\n", &mode, &pageno, &garbage1, &garbage);
		total++;
		mem_access_LRU_WT(pageno, mode);
		////
		/*printf("%d\n", total);
		for(int i=1;i<heapend;i++)
			printf("[%d %.1lf]",heap[i].page_num, heap[i].used);
		printf("\n");
		*/
	}
	fclose(fp);
	printf("=====  WriteThrough LRU  =====\n");
	printf("TOTAL %d WRITE %d READ %d\n",total,wcount,rcount);
	printf("STORAGE ACCESS : %d\n",stoacc);
	printf("MISS %d HIT RATIO %.2f\n",miss,(total-miss)/(double)total);
	printf("TIME : %.2lfms\n\n",Time/10000);
	desH();
	return 0;
}

int SimWB_FIFO(){
	Time=0;
	miss=0;
	total=0;
	stoacc=0;
	wcount=rcount=0;
	char mode;
	int pageno;
	int garbage;
	initQ();
	FILE *fp = fopen(filename, "r");
	while(!feof(fp)){
		fscanf(fp, "%c %d %c %d\n", &mode, &pageno, (char*)&garbage, &garbage);
		total++;
		mem_access_FIFO_WB(pageno, mode);
		/*for(int i=front;i<front+Qnum;i++)
			printf("%d ",queue[i%MAXQ].page_num);
		printf("\n");*/

	}
	fclose(fp);
	printf("=====  WriteBack FIFO  =====\n");
	printf("TOTAL %d WRITE %d READ %d\n",total,wcount,rcount);
	printf("STORAGE ACCESS : %d\n",stoacc);
	printf("MISS %d HIT RATIO %.2f\n",miss,(total-miss)/(double)total);
	printf("TIME : %.2lfms\n\n",Time/10000);
	desQ();
	return 0;
}

int SimWB_LRU(){
	Time=0;
	miss=0;
	total=0;
	stoacc=0;
	wcount=rcount=0;
	char mode;
	int pageno;
	int garbage;
	char garbage1;
	initH();
	FILE *fp = fopen(filename, "r");
	while(!feof(fp)){
		fscanf(fp, "%c %d %c %d\n", &mode, &pageno, &garbage1, &garbage);
		total++;
		mem_access_LRU_WB(pageno, mode);
		/*for(int i=0;i<heapend;i++)
			printf("%d %.2f ",heap[i].page_num,heap[i].used);
		printf("\n");*/
	}
	fclose(fp);
	printf("=====  WriteBack LRU  =====\n");
	printf("TOTAL %d WRITE %d READ %d\n",total,wcount,rcount);
	printf("STORAGE ACCESS : %d\n",stoacc);
	printf("MISS %d HIT RATIO %.2f\n",miss,(total-miss)/(double)total);
	printf("TIME : %.2lfms\n\n",Time/10000);
	desH();
	return 0;
}
int main(){
	SimWT_FIFO();
	SimWT_LRU();
	SimWB_FIFO();
	SimWB_LRU();
	return 0;
}
