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
    int m;
    key_t key = 8000;
    key_t key1 = 10000;
    if(strcmp(gender, "MALE") == 0){
        m = msgget(key,0666|IPC_CREAT); 
        if(m == -1){
            perror("msgget initialization");
		    exit(1);
        }
    } else {
        m = msgget(key1,0666|IPC_CREAT);
        if(m == -1){
            perror("msgget initialization");
		    exit(1);
        }
    }
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
    printf("%d yuutyuio\n", m);
}