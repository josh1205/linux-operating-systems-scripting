// Main docs used to solve problem
// https://www.gnu.org/software/libc/manual/html_node/Semaphores.html
// https://docs.oracle.com/cd/E26502_01/html/E35303/tlib-1.html


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>

#define M 7
#define N 10

sem_t semaphore;

void enter(void) {
	sem_wait(&semaphore);
}

void leave(void) {
	sem_post(&semaphore);
}

void doCriticalWork(pthread_t id) {
	int threadCount;
	sem_getvalue(&semaphore, &threadCount);
	printf("Thread ID: %li\n", (unsigned long int) id);
	printf("Number of threads currently in M section: %d\n\n", threadCount);

}

void * doWork(void* arg) {
	while(1) {
		//printf("entering while loop\n");
		enter();
		doCriticalWork(pthread_self());
		sleep(5);
		leave();
		//printf("leaving while loop\n");	
	}
	pthread_exit(NULL);

}


int main (int argc, char * argv[]) {
	pthread_t thread[N];
	sem_init(&semaphore, 0, M);
	for(int i = 0; i < N; i++) {
		//printf("its me: %d\n", i);
		int *a = malloc(sizeof(int));
		*a = i;
		if (pthread_create(&thread[i], NULL, doWork, a) != 0) {
			perror("Failed to create thread");
		}
		//printf("im leaving: %d\n", i);
	}
	
	for (int i = 0; i < N; i++) {
		//printf("here i am again: %d\n", i);
		if(pthread_join(thread[i], NULL) != 0) {
			perror("Failed to join thread");
		}
		//printf("im leaving again: %d\n", i);
	}
	sem_destroy(&semaphore);
	return 0;
	
	
}	
