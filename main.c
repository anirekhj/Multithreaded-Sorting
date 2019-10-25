#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 2

int *integer_array;
int *sorted_integer_array;

void *sorter(void *params); /* thread that performs basic sorting algorithms */
void *merger(void *params); /* thread that performs merging of results */

/* structure for passing data to threads */
typedef struct {
  int starting_index;
  int ending_index;
} parameters;

int main(int argc, const char *argv[]) {
  // Open the input file
  FILE *txt_file = fopen(argv[1], "r");
  int capacity = 1;
  int count = 0, data = 0;
  char ch;
  integer_array = (int*) malloc(sizeof(int) * capacity);

  // Check the number of args
  if (argc != 2) {
    printf("Usage: %s, <file_name>\n");
    return 0;
  }

  if (txt_file == NULL) {
    printf("Error: can't open the file");
  }
  else {
    // read till EOF and store in integer array. Realloc the size dynamically.
    while (fscanf(txt_file, "%d%c", &data, &ch) != EOF) {
      if (capacity == count) {
        integer_array = (int*) realloc(integer_array, sizeof(int)*(++capacity));
      }
      integer_array[count++] = data;
    }
  }

  // create and execute the sorting threads
  parameters *info = (parameters*) malloc(sizeof(parameters));
  int thread = 0;
  for (thread = 0; thread < NUM_THREADS; thread++) {
    pthread_t sorter_id;
    info->starting_index = thread * count / NUM_THREADS;
    info->ending_index = (thread + 1) * count / NUM_THREADS;
    pthread_create(&sorter_id, NULL, sorter, info);
    pthread_join(sorter_id, NULL);
  }

  sorted_integer_array = (int*) malloc(sizeof(int) * capacity);

  // create and execute the merging thread
  pthread_t merger_id;
  info->starting_index = 0;
  info->ending_index = count;
  pthread_create(&merger_id, NULL, merger, info);
  pthread_join(merger_id, NULL);

  // Create the ouput file and write sorted list to it
  txt_file = fopen("SortedIntegerList.txt", "w");
  int i = 0;
  for (i = 0; i < capacity; i++) {
    if (i != 0) {
      fprintf(txt_file, ",");
    }
    fprintf(txt_file, "%d", sorted_integer_array[i]);
  }

  fclose(txt_file);
  free(integer_array);
  free(sorted_integer_array);
  return 0;
}

// Bubble sort
void *sorter(void *info) {
  int i, j;
  int start = ((parameters *) info)->starting_index;
  int end = ((parameters *) info)->ending_index;

  for (i = start; i < end; i++) {
    for (j = start; j < end - 1; j++) {
      if (integer_array[j] > integer_array[j+1]) {
        int temp = integer_array[j];
        integer_array[j] = integer_array[j+1];
        integer_array[j+1] = temp;
      }
    }
  }
  pthread_exit(0);
}

// Simple merge function that compares the first elements of the sorted
// sub-arrays and then stores the smaller in a new array and increments
// accordingly. Stores the leftovers from both subarrays at the end.
void *merger(void *info) {
  int i = 0, k = 0;
  int end = ((parameters *) info)->ending_index;
  int j = end/2;

  while ((i < end/2) && (j < end)) {
    if (integer_array[i] < integer_array[j]) {
      sorted_integer_array[k++] = integer_array[i++];
    }
    else {
      sorted_integer_array[k++] = integer_array[j++];
    }
  }

  while (i < end/2) {
    sorted_integer_array[k++] = integer_array[i++];
  }

  while (j < end) {
    sorted_integer_array[k++] = integer_array[j++];
  }
}
