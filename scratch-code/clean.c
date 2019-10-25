// #include <stdio.h>
// #include <stdlib.h>
// #include <pthread.h>
//
// int *integer_array;
// void *sorter(void *params); /* thread that performs basic sorting algorithms */
// void *merger(void *params); /* thread that performs merging of results */
//
// /* structure for passing data to threads */
// typedef struct {
//   int starting_index;
//   int ending_index;
// } parameters;
//
// int main(int argc, const char *argv[]) {
//   // clock_t begin = clock();
//   FILE *input_file = fopen(argv[1], "r");
//   int capacity=1;
//   integer_array =(int*)malloc(sizeof(int)*capacity);
//   int count = 0;
//   int data = 0;
//   char ch;
//
//   if (argc != 2) {
//     /* Assume that argv[0] is the program name */
//     printf("usage: %s, <file_name>", argv[0]);
//     return 0;
//   }
//
//   if (input_file == NULL) {
//     printf("can't open file");
//   }
//   else {
//     while(fscanf(input_file,"%d%c", &data, &ch) != EOF) {
//       if(capacity==count) {
//         integer_array=(int*)realloc(integer_array, sizeof(int)*(++capacity));
//       }
//       integer_array[count++] = data;
//     }
//   }
//
//   // printf("\n\n input_integer_array: \n");
//   // for (int i = 0; i < capacity; i++)
//   // {
//   //   printf("%d ", integer_array[i]);
//   // }
//
//   /* create worker threads */
//   pthread_t sorter_one_id, sorter_two_id;
//   pthread_attr_t attr;
//
//   // pthread_attr_init(&attr);
//
//   parameters *info = (parameters *) malloc(sizeof(parameters));
//
//
//   info->starting_index = 0;
//   info->ending_index = count/2;
//   pthread_create(&sorter_one_id, NULL, sorter, info);
//   pthread_join(sorter_one_id, NULL);
//
//
//   info->starting_index = count/2;
//   info->ending_index = count;
//   pthread_create(&sorter_two_id, NULL, sorter, info);
//   pthread_join(sorter_two_id, NULL);
//
//   int i = 0; int j = count/2; int k = 0;
//   int *sorted_integer_array =(int*)malloc(sizeof(int)*capacity);
//
//   while ((i < (count/2)) && (j < count))
//     {
//         // Check if current element of first
//         // array is smaller than current element
//         // of second array. If yes, store first
//         // array element and increment first array
//         // index. Otherwise do same with second array
//         if (integer_array[i] < integer_array[j])
//             sorted_integer_array[k++] = integer_array[i++];
//         else
//             sorted_integer_array[k++] = integer_array[j++];
//     }
//     // Store remaining elements of first array
//   while (i < count/2)
//       sorted_integer_array[k++] = integer_array[i++];
//
//   // Store remaining elements of second array
//   while (j < count)
//       sorted_integer_array[k++] = integer_array[j++];
//
//   // int aux[count];
//   // merge_sort(0, count - 1, integer_array, aux);
//   //
//
//   // printf("\n\n integer_array: \n");
//   // for (int i = 0; i < capacity; i++)
//   // {
//   //   printf("%d ", integer_array[i]);
//   // }
//
//   printf("\n\n sorted_integer_array: \n");
//
//   for (int i = 0; i < capacity; i++)
//   {
//     printf("%d ", sorted_integer_array[i]);
//   }
//
//   printf("\n\n");
//
//   /*  create the first sorting thread
//       create the second sorting thread
//       now wait for the 2 sorting threads to finish
//       establish the merge thread
//       wait for the merge thread to finish
//       output the sorted array */
//   // printf("Hello World\n");
//   free(integer_array);
//   free(sorted_integer_array);
//   return 0;
// }
//
// void *sorter(void *info)
// {
//    int i, j;
//    // printf("starting_index:%d ", ((parameters *) info)->starting_index);
//    // printf("ending_index:%d ", ((parameters *) info)->ending_index);
//    for (i = ((parameters *) info)->starting_index; i < ((parameters *) info)->ending_index; i++) {
//      // printf("\ni:%d\n", integer_array[i]);
//        // Last i elements are already in place
//        for (j = ((parameters *) info)->starting_index; j < ((parameters *) info)->ending_index-1; j++) {
//            // printf("j:%d\n", integer_array[j]);
//            if (integer_array[j] > integer_array[j+1]) {
//              // printf("j:%d\n", integer_array[j]);
//              int temp = integer_array[j];
//              integer_array[j] = integer_array[j+1];
//              integer_array[j+1] = temp;
//            }
//        }
//     }
//     pthread_exit(0);
// }

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
  // clock_t begin = clock();
  // NUM_THREADS = 2; // atoi(argv[2]);
  FILE *input_file = fopen(argv[1], "r");
  int capacity=1;
  integer_array =(int*)malloc(sizeof(int)*capacity);
  int count = 0;
  int data = 0;
  char ch;

  if (argc != 2) {
    /* Assume that argv[0] is the program name */
    printf("usage: %s, <file_name>\n", argv[0]);
    return 0;
  }

  if (input_file == NULL) {
    printf("can't open file");
  }
  else {
    while(fscanf(input_file,"%d%c", &data, &ch) != EOF) {
      if(capacity==count) {
        integer_array=(int*)realloc(integer_array, sizeof(int)*(++capacity));
      }
      integer_array[count++] = data;
    }
  }

  // printf("\n\n input_integer_array: \n");
  // for (int i = 0; i < capacity; i++)
  // {
  //   printf("%d ", integer_array[i]);
  // }

  /* create worker threads */

  // pthread_attr_t attr;

  // pthread_attr_init(&attr);

  parameters *info = (parameters *) malloc(sizeof(parameters));
  int threads = 0;
  for(threads = 0; threads < NUM_THREADS; threads++) {
    pthread_t sorter_id;
    info->starting_index = threads*count/NUM_THREADS;
    info->ending_index = (threads+1)*count/NUM_THREADS;
    pthread_create(&sorter_id, NULL, sorter, info);
    pthread_join(sorter_id, NULL);
  }

  printf("\n\n input_integer_array: \n");
  // for (int i = 0; i < capacity; i++)
  // {
  //   printf("%d ", integer_array[i]);
  // }
  // pthread_t sorter_one, sorter_two, sorter_three, sorter_four, sorter_five, sorter_six;
  // // pthread_attr_t attr;
  //
  // // pthread_attr_init(&attr);
  //
  // parameters *info = (parameters *) malloc(sizeof(parameters));
  // int threads = 0;
  // for(threads = 0; threads < N)
  // info->starting_index = 0;
  // info->ending_index = count/6;
  // pthread_create(&sorter_one, NULL, sorter, info);
  // pthread_join(sorter_one, NULL);
  //
  // info->starting_index = count/6;
  // info->ending_index = 2*count/6;
  // pthread_create(&sorter_two, NULL, sorter, info);
  // pthread_join(sorter_two, NULL);
  //
  // info->starting_index = 2*count/6;
  // info->ending_index = 3*count/6;
  // pthread_create(&sorter_three, NULL, sorter, info);
  // pthread_join(sorter_three, NULL);
  //
  // info->starting_index = 3*count/6;
  // info->ending_index = 4*count/6;
  // pthread_create(&sorter_four, NULL, sorter, info);
  // pthread_join(sorter_four, NULL);
  //
  // info->starting_index = 4*count/6;
  // info->ending_index = 5*count/6;
  // pthread_create(&sorter_five, NULL, sorter, info);
  // pthread_join(sorter_five, NULL);
  //
  // info->starting_index = 5*count/6;
  // info->ending_index = count;
  // pthread_create(&sorter_six, NULL, sorter, info);
  // pthread_join(sorter_six, NULL);




  sorted_integer_array =(int*)malloc(sizeof(int)*capacity);

  pthread_t merger_id;
  info->starting_index = 0;
  info->ending_index = count;
  pthread_create(&merger_id, NULL, merger, info);
  pthread_join(merger_id, NULL);

  // int aux[count];
  // merge_sort(0, count - 1, integer_array, aux);
  //

  // printf("\n\n integer_array: \n");
  // for (int i = 0; i < capacity; i++)
  // {
  //   printf("%d ", integer_array[i]);
  // }

  printf("\n\n sorted_integer_array: \n");

  for (int i = 0; i < capacity; i++)
  {
    printf("%d ", sorted_integer_array[i]);
  }

  printf("\n\n");

  /*  create the first sorting thread
      create the second sorting thread
      now wait for the 2 sorting threads to finish
      establish the merge thread
      wait for the merge thread to finish
      output the sorted array */
  // printf("Hello World\n");
  free(integer_array);
  free(sorted_integer_array);
  return 0;
}

void *sorter(void *info)
{
   int i, j;
   // printf("starting_index:%d ", ((parameters *) info)->starting_index);
   // printf("ending_index:%d ", ((parameters *) info)->ending_index);
   for (i = ((parameters *) info)->starting_index; i < ((parameters *) info)->ending_index; i++) {
     // printf("\ni:%d\n", integer_array[i]);
       // Last i elements are already in place
       for (j = ((parameters *) info)->starting_index; j < ((parameters *) info)->ending_index-1; j++) {
           // printf("j:%d\n", integer_array[j]);
           if (integer_array[j] > integer_array[j+1]) {
             // printf("j:%d\n", integer_array[j]);
             int temp = integer_array[j];
             integer_array[j] = integer_array[j+1];
             integer_array[j+1] = temp;
           }
       }
    }
    pthread_exit(0);
}

void *merger(void *info)
{
   int i = 0; int j = ((parameters *) info)->ending_index/2; int k = 0;
   // printf("starting_index:%d ", ((parameters *) info)->starting_index);
   // printf("ending_index:%d ", ((parameters *) info)->ending_index);
   while ((i < (((parameters *) info)->ending_index/2)) && (j < ((parameters *) info)->ending_index))
     {
         // Check if current element of first
         // array is smaller than current element
         // of second array. If yes, store first
         // array element and increment first array
         // index. Otherwise do same with second array
         if (integer_array[i] < integer_array[j])
             sorted_integer_array[k++] = integer_array[i++];
         else
             sorted_integer_array[k++] = integer_array[j++];
     }
     // Store remaining elements of first array
   while (i < ((parameters *) info)->ending_index/2)
       sorted_integer_array[k++] = integer_array[i++];

   // Store remaining elements of second array
   while (j < ((parameters *) info)->ending_index)
       sorted_integer_array[k++] = integer_array[j++];
   pthread_exit(0);
}
