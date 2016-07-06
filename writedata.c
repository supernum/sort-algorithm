#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/timeb.h>

#define NUMMAX 20

int main(int argc, char **argv)
{
	int i, num;

    if (argc == 2) 
        num = atoi(argv[1]);
    else
        num = NUMMAX;
	FILE *fp = fopen("sort.data", "a");
	if (fp == NULL) {
		fprintf(stderr, "file open failed.");
		return 1;
	}
    
	srand(time(NULL));
	for (i = 0; i < num; i++)
		fprintf(fp, "%d\n", rand()%num);
	fclose(fp);
	return 0;
}


