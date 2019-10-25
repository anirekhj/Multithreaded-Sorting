// Sum of n natural number using parallel processing

#include <pthread.h>
#include <stdio.h>

#include <stdlib.h>

int sum; // this data is shared by the threads
void *runner(void *param) //threads call this function
{
  int i, upper = atoi(param);
  sum = 0;
  for (i =1; i<= upper; i++) {
    sum +=i;
  }
  pthread_exit(0);
}

int main(int argc, char *argv[])
{
  pthread_t tid; // the thread identifier
  pthread_attr_t attr; // set of thread attributes

  // set the default attributes of the thread
  pthread_attr_init(&attr);
  // create the thread
  pthread_create(&tid, &attr, runner, argv[1]); // a param is passed to thread

  // wait for the thread to exit

  // After creating the summation thread, the parent thread will wait for
  // it to terminate by calling pthread_join()
  pthread_join(tid,NULL);

  printf("sum = %d\n", sum);
}
