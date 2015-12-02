#include <stdio.h>

#define INCL 0
#define EXCL 1

struct input{
	char A[2];
	int B[2];
};

int main(){
	FILE *fp = fopen("input.txt", "r");
	int i=0;
	int count=0;
	int t1, t2;
	char tt1, tt2;
	struct input a[60000];
	int tag=0;

	fscanf(fp, "%c %d %c %d\n", &a[0].A[0], &a[0].B[0], &a[0].A[1], &a[0].B[1]);
	count++;
	while(!feof(fp)){
		fscanf(fp, "%c %d %c %d\n", &tt1, &t1, &tt2, &t2);
		for(i=0; i<count; i++){
			#if INCL
			if(t1 == a[i].B[0] && tt1 == a[i].A[0])
			#endif
			#if EXCL
			if(t1 == a[i].B[0])
			#endif
			{
				tag=1;
				break;
			}
			else{
				tag=0;
			}
		}
		if(tag == 0){
			a[count].A[0] = tt1;
			a[count].B[0] = t1;
			a[count].A[1] = tt2;
			a[count].B[1] = t2;
			count++;
		}
	}

	for(i=0; i<count; i++){
		printf("%c %d %c %d\n", a[i].A[0], a[i].B[0], a[i].A[1], a[i].B[1]);
	}
	printf("foot print : %d cache size : %d\n", count,count/2);
}
