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
    char gender[10];
	int pid; // to know to which client to return the file data
} person;

int main(int argc, char* args[]){
    char* gender = args[0];
    int m;
    if(strcmp(gender, "MALE") == 0){
        m = msgget(5000,0666|IPC_CREAT); 
        if(m == -1){
            perror("msgget initialization");
		    exit(1);
        }
    } else {
        m = msgget(7000,0666|IPC_CREAT);
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
        msgsnd(m, &person, sizeof(person)-4 ,0);// send pointer to p on the queue
        sleep(4);
    }
    printf("%d yuutyuio\n", m);
}