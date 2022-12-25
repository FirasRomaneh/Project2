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

pid_t pid;
pid_t pids[10];

int main () {
    pid = fork();
    if(pid == -1)  //error
        exit(-1);
    else if(pid == 0) { //w
        execl("./worker", "MALE", "1",(char *) NULL);
        perror("exec failure ");
        exit(-2);
    }

    pid = fork();
    if(pid == -1)  //error
        exit(-1);
    else if(pid == 0) { //w
        execl("./worker", "FEMALE", "1",(char *) NULL);
        perror("exec failure ");
        exit(-2);
    }
    sleep(1);
    pid = fork();
    if(pid == -1)  //error
        exit(-1);
    else if(pid == 0) { //w
        execl("./worker", "MALE", "2",(char *) NULL);
        perror("exec failure ");
        exit(-2);
    }

    pid = fork();
    if(pid == -1)  //error
        exit(-1);
    else if(pid == 0) { //w
        execl("./worker", "FEMALE", "2",(char *) NULL);
        perror("exec failure ");
        exit(-2);
    }

    printf("\033[0;36m");
    printf("start Forking............\n"); 
    fflush(stdout);
    for(int i=0; i<10; i++) {       // forking people
        pid = fork();
        if(pid == -1)  //error
            exit(-1);
        else if(pid == 0) { //people
            if(i%2 == 0){
                execl("./people", "MALE", (char *) NULL);
            } else {
                execl("./people", "FEMALE", (char *) NULL);
            }
            perror("exec failure ");
            exit(-2);
        } else {  //parent
            pids[i] = pid;
            sleep(1);
        }
    }
    while(1);
}



