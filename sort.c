#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <sys/timeb.h>
#include <string.h>

typedef struct sarr {
	int len;
	int used;
	int *v;
} sarr;

sarr *create_array(void) {
	sarr *arr;

	arr = malloc(sizeof(sarr));	
	if (arr == NULL) return NULL;
	arr->len = 0;
	arr->used = 0;
	arr->v = NULL;
	return arr;
}

void free_array(sarr *arr) {
	if (arr == NULL)
		return;
	if (arr->v) free(arr->v);
	free(arr);
}

int expand_array_space(sarr *arr) {
	int *v = arr->v;

	if (v == NULL) {
		v = malloc(sizeof(int)*16);
		if (v == NULL) return -1;
		arr->len = 16;
	} else {
		v = realloc(v, sizeof(int)*arr->len*2);
		if (v == NULL) return -1;
		arr->len *= 2;
	}
	arr->v = v;	
	return 0;	
}


int append_number(sarr *arr, int num) {
	if (arr->len-arr->used == 0 && expand_array_space(arr) == -1) {
		fprintf(stderr, "expand array space failed");
		return -1;
	}
	arr->v[arr->used++] = num;
	return 0;	
}

sarr *load_data(char *filename) {
	struct timeb t;
	sarr *arr = NULL;
	FILE *fp = NULL;
	char buf[20];
	
	ftime(&t);
	printf("%s, begin time=%ld, ms=%d\n", __func__, t.time, t.millitm);
	arr = create_array();
	if (arr == NULL) {
		fprintf(stderr, "malloc memory failed!");
		goto err;
	} 

	fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "open sort file failed");
		goto err;
	}
	while (fgets(buf, sizeof(buf), fp) != NULL) {
		int num = atoi(buf);
		append_number(arr, num);	
	}
	ftime(&t);
	printf("%s, end time=%ld, ms=%d\n", __func__, t.time, t.millitm);

	return arr;

err:
	if (arr) free_array(arr);
	if (fp) fclose(fp);
	return NULL;

}

int bubble_sort(int *arr, int size);
void quick_sort(int *arr, int size);
void insterion_sort(int *arr, int size);
void shell_sort(int *arr, int size);
void merge_sort(int *arr, int size);
void heap_sort(int *arr, int size);

int main(int argc, char **argv) {
	sarr *arr;
	int i;

	((void)(argc));
	((void)(argv));
	arr = load_data("sort.data");
	if (arr == NULL)
		exit(1);

	for (i = 0; i < arr->used; i++)
		printf("%d ", arr->v[i]);
	printf("\n");

    heap_sort(arr->v, arr->used);
	//merge_sort(arr->v, arr->used);
	//shell_sort(arr->v, arr->used);
	//insterion_sort(arr->v, arr->used);
	//quick_sort(arr->v, arr->used);
	//bubble_sort(arr->v, arr->used);
	
	for (i = 0; i < arr->used; i++)
		printf("%d ", arr->v[i]);
	printf("\n");
	return 0;
}

static void swap(int *a, int *b) {
	if (a != b) {
		*a ^= *b;	
		*b ^= *a;	
		*a ^= *b;	
	}
}

int bubble_sort(int *arr, int size) {
	int i, j;

	size--;
	for (i = 0; i < size; i++) {
		for (j = 0; j < size-1; j++) {
			if (arr[j] > arr[j+1]) {
				swap(&arr[j], &arr[j+1]);
			}
		}
	}	
	return 1;
}

void quick_sort(int *arr,  int size) {
	int k, i, j, right = 1;

	if (size < 2)
		return;

	for (i = 0, j = size-1, k = arr[i]; i != j;) {
		if (right) {
			if (arr[j] < k) {
				arr[i++] = arr[j];
				right = 0;
			} else {
				j--;	
			}
		} else {
			if (arr[i] > k) {
				arr[j--] = arr[i];
				right = 1;
			} else {
				i++;	
			}
		}
	}
	arr[i] = k;
	quick_sort(arr, i);
	quick_sort(arr+i+1, size-i-1);
}


void insterion_sort(int *arr, int size) {
	int i, j;

	for (i = 1; i < size; i++) {
		for (j = i; j > 0 && arr[j] < arr[j-1]; j--)
			swap(&arr[j], &arr[j-1]);
	}	
}

void shell_sort(int *arr, int size) {
	int i, j, gap;

	for (gap = size/2; gap > 0; gap/=2) {
		for (i = gap; i < size; i++) {
			for (j = i-gap; j >= 0 && arr[j] > arr[j+gap]; j -= gap)
				swap(&arr[j], &arr[j+gap]);
		}
	}	
}

void merge_data(int *arr, int size, int mid, int *tmp) {
	int i, j, k = 0;

	for (i = 0, j = mid; k < size; k++) {
		if (j == size || (i < mid && arr[j] > arr[i])) {
			tmp[k] = arr[i++];
		} else {
			tmp[k] = arr[j++];
		}
	}
	memcpy(arr, tmp, sizeof(int)*size);
}

void _merge_sort(int *arr, int size, int *tmp) {
	int mid;

	if (size < 2)
		return;

	mid = size/2;
	merge_sort(arr, mid);
	merge_sort(arr+mid, size-mid);
	merge_data(arr, size, mid, tmp);		
}

void merge_sort(int *arr, int size) {
	int *tmp = malloc(sizeof(void*)*size);
	_merge_sort(arr, size, tmp);
	free(tmp);
}

void _heap_sort(int *arr, int size, int i) {
    int j;

    for (; (j = ((2*i)+1)) < size; i = j) {
        if (j < size-1 && arr[j+1] > arr[j])
            j++;
        if (arr[i] < arr[j])
           swap(&arr[i], &arr[j]); 
        else
            break;
    } 
}

void heap_sort(int *arr, int size) {
    int i;

    for (i = size/2-1; i >= 0; i--)
        _heap_sort(arr, size, i);
    
    for (i = size-1; i > 0; i--) {
        swap(&arr[0], &arr[i]);
        _heap_sort(arr, i, 0);
    }
}

