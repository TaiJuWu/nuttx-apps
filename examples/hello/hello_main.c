/****************************************************************************
 * apps/examples/hello/hello_main.c
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <stdio.h>
#include <nuttx/spinlock.h>
#include <pthread.h>
#include <sched.h>

/****************************************************************************
 * Public Functions
 ****************************************************************************/
fair_spinlock_list_t test_fair_spinlock_list;
/****************************************************************************
 * hello_main
 ****************************************************************************/
void *thread_function(void *arg)
{
  #define ENTER_TIMES 5
    int thread_id = *((int *)arg);
    fair_spinlock_t lock;
    fair_spinlock_init(&lock);

    for(int i = 1; i <= ENTER_TIMES; ++i) {
      fair_spin_lock(&test_fair_spinlock_list, &lock);
      printf("[CPU%d]Thread %d is enter lock %d time.\n", sched_getcpu(),thread_id, i);
      // Do some work here...
      int counter = 0;
      while(counter++ < 100);
      printf("[CPU%d]Thread %d is exiting lock %d time.\n", sched_getcpu(),thread_id, i);
      fair_spin_unlock(&test_fair_spinlock_list, &lock);
    }
    
    pthread_exit(NULL);
}


int main(int argc, FAR char *argv[])
{
#define THREAD_NUM 20
    pthread_t thread[THREAD_NUM];
    int ret;
    
    fair_spinlock_list_init(&test_fair_spinlock_list);
    printf("Main thread is running.\n");

    // Create threads
    for(int i = 0; i < THREAD_NUM; ++i){
      ret = pthread_create(&thread[i], NULL, thread_function, (void *)(&i));
      if (ret != 0) {
          printf("Error creating thread 1: %d\n", ret);
          return 1;
      }
    }
    
    // Wait for threads to finish
    for(int i = 0; i < THREAD_NUM; ++i){
      pthread_join(thread[i], NULL);
    }
    

    printf("Main thread is exiting.\n");
  return 0;
}
