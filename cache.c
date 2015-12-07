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

	Time+=MEMACC;
	int exist=is_existQ(data);
if(mode=='R'){//read
		rcount++;
		if(exist != -1) //hit
			queue[exist].used=Time;
		else{//miss
			miss++;
			if(is_fullQ()){ //if full deque
				deque();
			}
			Time+=STOACC; //read page
			stoacc++;
			data.used = Time;
			enque(data);
		}
	}
	else{//write
		wcount++;
		if(exist != -1){ //hit
			queue[exist].used=Time;
		}
		else{ //miss
			miss++;
			if(is_fullQ()){ //if full deque	
				 deque();
			}
			Time+=STOACC; //read page
			stoacc++;
			data.used = Time;
		}
	}

	return 0;
}


int mem_access_LRU_WT(int no, char mode){
	mal_mem data;
	data.page_num=no;
	if(mode=='W') data.dirty=1;
	else data.dirty=0;

	Time+=MEMACC;
	int exist=is_existH(data);
	if(mode=='R'){ //read
		rcount++;
		if(exist!=-1){ //hit
			heap[exist].used=Time; 
			heapify(exist); //heap rebuilding
		}
		else{ //miss
			miss++;
			if(is_fullH()) deheap(); //if full deheap
			Time+=STOACC; //read storage
			stoacc++;
			data.used=Time;
			enheap(data); 
		}
	}
	else{//write
		wcount++;
		if(exist!=-1){//hit
			heap[exist].used=Time;
			heapify(exist);
			Time+=STOACC; //write stroage
			stoacc++;
		
		}
		else{//miss
			miss++;
			Time+=STOACC; //write storage
			stoacc++;
		}}
		return 0;
}

int mem_access_FIFO_WB(int no, char mode){
	int deQ;
	mal_mem data;
	data.page_num = no;
	Time += MEMACC;

	int exist = is_existQ(data);
	if(mode=='R'){//read
		rcount++;
		if(exist != -1) //hit
			queue[exist].used=Time;
		else{//miss
			miss++;
			if(is_fullQ()){ //if full deque
				deQ = deque();
				if(queue[deQ].dirty == 1){ //dirty -> storage update
					Time+=STOACC;
					stoacc++;
				}
			}
			Time+=STOACC; //read page
			stoacc++;
			data.used = Time;
			data.dirty = 0;
			enque(data);
		}
	}
	else{//write
		wcount++;
		if(exist != -1){ //hit
			queue[exist].used=Time;
			queue[exist].dirty=1; //make dirty
		}
		else{ //miss
			miss++;
			if(is_fullQ()){ //if full deque	
				deQ = deque();
				if(queue[deQ].dirty == 1){ //dirty-> storage update
					Time+=STOACC;
					stoacc++;
				}
			}
			Time+=STOACC; //read page
			stoacc++;
			data.used = Time;
			data.dirty = 1;
			enque(data);
		}
	}
	return 0;
}
int mem_access_LRU_WB(int no, char mode){
	int deH;
	mal_mem data;
	data.page_num=no;
	data.used=Time;

	Time+=MEMACC;
	int exist=is_existH(data);
	if(mode=='R'){ //read
		rcount++;
		if(exist != -1){//hit
			heap[exist].used=Time;
			heapify(exist);
		}
		else{//miss
			miss++;
			if(is_fullH())	{ //if full deheap
				deH = deheap();
				if(heap[deH].dirty == 1){//dirty->update
					Time+=STOACC;
					stoacc++;
				}
			}
			Time+=STOACC;//read storage
			stoacc++;
			data.used = Time;
			data.dirty = 0;
			enheap(data);
		}
	}
	else{//write
		wcount++;
		if(exist != -1){//hit
			heap[exist].used=Time;
			heap[exist].dirty=1; //make dirty
			heapify(exist);
		}
		else{//miss
			miss++;
			if(is_fullH()){//if full deheap
				deH = deheap();
				if(heap[deH].dirty == 1){//dirty->update
					Time+=STOACC;
					stoacc++;
				}
			}
			Time+=STOACC;//read storage
			stoacc++;
			data.used = Time;
			data.dirty = 1;
			enheap(data);
		}
	}
	return 0;
}
int mem_access_FIFO_WR(int no, char mode){
	mal_mem data;
	data.page_num=no;

	int exist=is_existQ(data);
	if(mode=='R'){ //read page
		Time+=MEMACC;
		rcount++;
		if(exist!=-1){ //hit
			queue[exist].used=Time;
		}
		else{
			miss++;
			if(is_fullQ()){
				deque();
			}
			Time+=STOACC;
			stoacc++;
			data.used=Time;
			enque(data);

		}
	}
	else{ //write
		wcount++;
		Time+=STOACC;
		stoacc++;
	}
	return 0;
}
int mem_access_LRU_WR(int no, char mode){
	mal_mem data;
	data.page_num=no;

	int exist=is_existH(data);
	if(mode=='R'){ //read page
		Time+=MEMACC;
		rcount++;
		if(exist!=-1){ //hit
			heap[exist].used=Time;
			heapify(exist);
		}
		else{
			if(is_fullH()){
				deheap();
			}
			miss++;
			Time+=STOACC;
			stoacc++;
			data.used=Time;
			enheap(data);

		}
	}
	else{ //write
		wcount++;
		Time+=STOACC;
		stoacc++;
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
	int i;
	FILE *fp = fopen(filename, "r");
	while(!feof(fp)){
		/*read line and access memory until end of file*/
		fscanf(fp, "%c %d %c %d\n", &mode, &pageno, (char*)&garbage, &garbage);
		total++;
		mem_access_FIFO_WT(pageno, mode);
	//	for(i=front;i<front+Qnum;i=(i+1)%MAXQ)
//		  printf("%d %.2f ",queue[i].page_num,queue[i].used);
//		 printf("\n");
	}
	fclose(fp);
	printf("=====  WriteThrough FIFO  =====\n");
	printf("TOTAL %d WRITE %d READ %d\n",total,wcount,rcount);
	printf("STORAGE ACCESS : %d\n",stoacc);
	printf("MISS %d HIT RATIO %.2f\n",miss,(total-miss)/(double)total);
	printf("TIME : %.2lfms\n\n",Time/1000000);
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
	int i;
	FILE *fp = fopen(filename, "r");
	while(!feof(fp)){
		/*read line and access memory until end of file*/
		fscanf(fp, "%c %d %c %d\n", &mode, &pageno, &garbage1, &garbage);
		total++;
		mem_access_LRU_WT(pageno, mode);
//		for(i=0;i<heapend;i++)
//		  printf("%d %.2f ",heap[i].page_num,heap[i].used);
//		  printf("\n");
	
		 
	}
	fclose(fp);
	printf("=====  WriteThrough LRU  =====\n");
	printf("TOTAL %d WRITE %d READ %d\n",total,wcount,rcount);
	printf("STORAGE ACCESS : %d\n",stoacc);
	printf("MISS %d HIT RATIO %.2f\n",miss,(total-miss)/(double)total);
	printf("TIME : %.2lfms\n\n",Time/1000000);
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
		/*read line and access memory until end of file*/
		fscanf(fp, "%c %d %c %d\n", &mode, &pageno, (char*)&garbage, &garbage);
		total++;
		mem_access_FIFO_WB(pageno, mode);
		/*for(int i=front;i<front+Qnum;i++)
		  printf("%d %d ",queue[front].page_num,queue[front].used);
		  printf("\n");*/

	}
	fclose(fp);
	printf("=====  WriteBack FIFO  =====\n");
	printf("TOTAL %d WRITE %d READ %d\n",total,wcount,rcount);
	printf("STORAGE ACCESS : %d\n",stoacc);
	printf("MISS %d HIT RATIO %.2f\n",miss,(total-miss)/(double)total);
	printf("TIME : %.2lfms\n\n",Time/1000000);
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
		/*read line and access memory until end of file*/
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
	printf("TIME : %.2lfms\n\n",Time/1000000);
	desH();
	return 0;
}
int SimWR_FIFO(){
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
		/*read line and access memory until end of file*/
		fscanf(fp, "%c %d %c %d\n", &mode, &pageno, (char*)&garbage, &garbage);
		total++;
		mem_access_FIFO_WR(pageno, mode);
	}
	fclose(fp);
	printf("=====  WriteAround FIFO  =====\n");
	printf("TOTAL %d WRITE %d READ %d\n",total,wcount,rcount);
	printf("STORAGE ACCESS : %d\n",stoacc);
		printf("MISS IN READ %d HIT RATIO IN READ %.2f\n",miss,(rcount-miss)/(double)rcount);
		printf("TIME : %.2lfms\n\n",Time/1000000);
	desQ();
	return 0;
}
int SimWR_LRU(){
	Time=0;
	miss=0;
	total=0;
	stoacc=0;
	wcount=rcount=0;
	char mode;
	int pageno;
	int garbage;
	initH();
	FILE *fp = fopen(filename, "r");
	while(!feof(fp)){
		/*read line and access memory until end of file*/
		fscanf(fp, "%c %d %c %d\n", &mode, &pageno, (char*)&garbage, &garbage);
		total++;
		mem_access_LRU_WR(pageno, mode);
	}
	fclose(fp);
	printf("=====  WriteAround LRU  =====\n");
	printf("TOTAL %d WRITE %d READ %d\n",total,wcount,rcount);
	printf("STORAGE ACCESS : %d\n",stoacc);
	printf("MISS IN READ %d HIT RATIO IN READ %.2f\n",miss,(rcount-miss)/(double)rcount);
	printf("TIME : %.2lfms\n\n",Time/1000000);
	desH();
	return 0;
}
int main(int argc,char* argv[]){
	int max;
	if(argc>1){
		max=atoi(argv[1]);
		if(max>0){
			MAXQ=max;
			MAXH=max;}
	}
	
	SimWT_FIFO();
	SimWT_LRU();
	SimWB_FIFO();
	SimWB_LRU();
	SimWR_FIFO();
	SimWR_LRU();
	return 0;
}
