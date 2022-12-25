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
    char* ttype = args[1];
    int tttype = atoi(ttype);
    int m;
	sem_t *m1;
    key_t key = 8000;
    key_t key1 = 10000;
    if(strcmp(gender, "MALE") == 0){
        m = msgget(key,0666|IPC_CREAT); //create new queue
        if ((m1 = sem_open("mysemaphore1", O_CREAT, 0644, 1)) == SEM_FAILED) {
            perror("semaphore initialization");
            exit(1);
        }
    } else {
        m = msgget(key1,0666|IPC_CREAT); //create new queue
        if ((m1 = sem_open("mysemaphore2", O_CREAT, 0644, 1)) == SEM_FAILED) {
            perror("semaphore initialization");
            exit(1);
        }
    }
    if(tttype == 1){
        sem_init(m1, 1, 3);
        sleep(5);
        while (1){
            msgrcv(m, &person, sizeof(person)-4, 1, 0);
            person.type = person.pid;
            sleep(2);
            msgsnd(m, &person, sizeof(person)-4 ,0);// send pointer to p on the queue
        }
    } else {
        sleep(15);
        while (1){
            printf("%d gfdg\n", m);
            msgrcv(m, &person, sizeof(person)-4, 2, 0);
            person.type = person.pid;
            printf("A %s person with PID = %d is body checking\n", person.gender, person.pid);
            fflush(stdout);
            sleep(5);
            msgsnd(m, &person, sizeof(person)-4 ,0);// send pointer to p on the queue
        }
    }
}