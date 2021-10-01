// simpletest.c : Defines the entry point for the console application.
//

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#include "uthread.h"


/////////////////////////////////////////////
//
// CCISEL 
// 2007-2019
//
// UThread Library First Test
//
// Jorge Martins, 2019
//
////////////////////////////////////////////

#define DEBUG

#define MAX_THREADS 10


/////////////////////////////////////////////////////////////
//                                                         //
// Test 1: N threads, each one printing its number M times //
//                                                         //
/////////////////////////////////////////////////////////////

uint64_t test1_count;

void test1_thread (void * argument) {
	uint8_t c;
	uint64_t index;
	c = (uint8_t) (uint64_t) argument;	
	 
	for (index = 0; index < 100; ++index) {
	    putchar(c); fflush(stdout);
		 
	    if ((rand() % 4) == 0) {
			usleep(100000); 
		    ut_yield();
	    }	 
    }
	++test1_count;
}

void test1 ()  {
	uint64_t index;

	test1_count = 0; 

	printf("\n :: Test 1 - BEGIN :: \n\n");

	for (index = 0; index < MAX_THREADS; ++index) {
		ut_create(test1_thread, (void *) ('0' + index), 1);
	}   

	ut_run();

	assert(test1_count == MAX_THREADS);

	printf("\n\n :: Test 1 - END :: \n");
}


// Test to run in single step

void func1(void * arg) {

	printf("Start func1\n");
	
	ut_deactivate();
 
	printf("End func1\n");

}

void func2(void * arg) {
	uthread_t * argt1 = (uthread_t *) arg;
	printf("func2\n");
	ut_activate(argt1);
}

void func3(void * arg) {
	printf("thread: %s\n", (char *) arg);
}

void test2() {
	printf("\n :: Test 2 - BEGIN :: \n\n");
	
	uthread_t *t1 = ut_create(func1, NULL, 1);
  	uthread_t *t2 = ut_create(func2, t1, 1);

  	printf("T1 Status : %d\n", ut_status(t1));
  	printf("T2 Status : %d\n", ut_status(t2));

  	ut_run();

 	printf("T1 Status : %d\n", ut_status(t1));
  	printf("T2 Status : %d\n", ut_status(t2));

  	printf("\n\n :: Test 2 - END :: \n");
}

void test3() {
	printf("\n :: Test 3 - BEGIN :: \n\n");
	
	ut_create(func3, "ut1", 2);
	ut_create(func3, "ut2", 3);
	ut_create(func3, "ut3", 1);
	
	ut_run();
	
	printf("\n\n :: Test 3 - END :: \n");
}

int main () {
	ut_init();
 
	test3();	
	 
	ut_end();
	return 0;
}


