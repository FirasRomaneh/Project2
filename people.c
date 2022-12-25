#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/wait.h>  /* Needed for the wait function */
#include <unistd.h>    /* Needed for the fork function */
#include <string.h>    /* Needed for the strcat function */
#include <semaphore.h>
#include <stdint.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <fcntl.h>

struct people{
	long int type;
    char* gender;
	int pid; // to know to which client to return the file data
} person;

int main(int argc, char* args[]){
    char* gender = args[0];
    //int gg = atoi(g);
    person.gender = gender;
    int mid1, mid2;
    sem_t *m1;
    key_t key = 8000;
    key_t key1 = 10000;
    if(strcmp(gender, "MALE") == 0){
	    mid1 = msgget(key,0666|IPC_CREAT); //create new queue
        if(mid1 == -1){
            perror("msgget initialization");
		    exit(1);
        }
	    if ((m1 = sem_open("mysemaphore1", O_CREAT, 0644, 1)) == SEM_FAILED) {
		    perror("semaphore initialization");
		    exit(1);
	    }
    } else {
        mid1 = msgget(key1,0666|IPC_CREAT); //create new queue
        if(mid1 == -1){
            perror("msgget initialization");
		    exit(1);
        }
        if ((m1 = sem_open("mysemaphore2", O_CREAT, 0644, 1)) == SEM_FAILED) {
		    perror("semaphore initialization");
		    exit(1);
	    }
    }
    printf("\033[0;34m");
    printf("A %s person with PID = %d has entered Area 1\n", person.gender, getpid());
    person.pid = getpid();
    person.type = 1;
	msgsnd(mid1, &person, sizeof(person)-4, 0);// send pointer to p on the queue
    msgrcv(mid1, &person, sizeof(person)-4, getpid(), 0);
    sem_wait(m1);
    printf("\033[0;31m");
    printf("A %s person with PID = %d has entered Area 2\n", person.gender, getpid());
    fflush(stdout);
    person.pid = getpid();
    person.type = 2;
    msgsnd(mid1, &person, sizeof(person)-4 ,0);// send pointer to p on the queue
    msgrcv(mid1, &person, sizeof(person)-4, getpid(), 0);
    sem_post(m1);
    printf("\033[0;35m");
    printf("A %s person with PID = %d has entered the inner grouping area\n", person.gender, getpid());
    while(1);
}
