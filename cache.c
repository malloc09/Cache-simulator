#include <stdio.h>
#include <time.h>
#include "cache.h"
#include "ds.h"

int Time,miss,actual;


int mem_access_FIFO(int no, char mode){
	
	mal_mem data;
	data.page_num=no;
	if(mode=='W') data.dirty=1;
	else data.dirty=0;
	data.used=Time;  //init data

	Time+=MEMACC;
	int exist=is_existQ(data);
	if(mode=='R'){	//Read
		if(exist==-1) {
			miss++;
			Time+=HDDACC; //load to mem
			if(is_fullQ()) //if full deque
				deque();
			enque(data);
		}
		else{
			queue[exist].used=Time;
		}

	}
	else{ //Write
		Time+=HDDACC; //write storage
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
	printf("%d MEM!\n",no);
	if(mode=='R'){	//Read
		if(exist==-1) {
		miss++;
		Time+=HDDACC; //load to mem
		
		if(is_fullH()){ //if full deque
			deheap();
		}
		enheap(data);
		}
		else
			heap[exist].used=Time;

		
	}
	else{ //Write
		Time+=HDDACC; //write storage
	}
	return 0;
}
int SimWT_FIFO(){
	Time=0;
	miss=0;
	actual=0;
	char mode;
	int pageno;
	int garbage;
	initQ();
	FILE *fp = fopen("input.txt", "r");
	while(!feof(fp)){
		fscanf(fp, "%c %d %c %d\n", &mode, &pageno, (char*)&garbage, &garbage);
		actual++;
		mem_access_FIFO(pageno, mode);
	}
	fclose(fp);
	printf("total %d, miss %d, hit ratio %.2f\n",actual,miss,(actual-miss)/(double)actual);
	printf("time : %d",Time);
	desQ();
	return 0;
}
int SimWT_LRU(){
	Time=0;
	miss=0;
	actual=0;
	char mode;
	int pageno;
	int garbage;
	char garbage1;
	initH();
	FILE *fp = fopen("input.txt", "r");
	while(!feof(fp)){
		printf("AAA\n");
		fscanf(fp, "%c %d %c %d\n", &mode, &pageno, &garbage1, &garbage);
		printf("ASDF\n");
		actual++;
		
		printf("%c %d %c %d\n",mode,pageno,garbage1,garbage);
		mem_access_LRU(pageno, mode);
	}
	fclose(fp);
	printf("total %d, miss %d, hit ratio %.2f\n",actual,miss,(actual-miss)/(double)actual);
	printf("time : %d",Time);
	desH();
	return 0;
}
int main(){
//	SimWT_FIFO();
	SimWT_LRU();
	return 0;
}
