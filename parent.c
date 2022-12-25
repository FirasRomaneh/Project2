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
pid_t male_worker_pid, female_worker_pid;
pid_t male_secofficer_pid, female_secofficer_pid;
void kill_all(int);

int main () {
    if(sigset(SIGINT, kill_all) == -1) {
        perror("sigset can not set SIGINT");
        exit(SIGINT);
    }
    
    pid = fork();
    if(pid == -1)  //error
        exit(-1);
    else if(pid == 0) { //w
        execl("./worker", "MALE", (char *) NULL);
        perror("exec failure ");
        exit(-2);
    }
    else {
        male_worker_pid = getpid();
    }

    pid = fork();
    if(pid == -1)  //error
        exit(-1);
    else if(pid == 0) { //w
        execl("./worker", "FEMALE", (char *) NULL);
        perror("exec failure ");
        exit(-2);
    }
    else {
        female_worker_pid = getpid();
    }
    
    pid = fork();
    if(pid == -1)  //error
        exit(-1);
    else if(pid == 0) { //w
        execl("./secofficer", "MALE", (char *) NULL);
        perror("exec failure ");
        exit(-2);
    }
    else {
        male_secofficer_pid = getpid();
    }

    pid = fork();
    if(pid == -1)  //error
        exit(-1);
    else if(pid == 0) { //w
        execl("./secofficer", "FEMALE", (char *) NULL);
        perror("exec failure ");
        exit(-2);
    }
    else {
        female_secofficer_pid = getpid();
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

void kill_all(int sig) {
    for(int i=0; i<10; i++) {
    	kill(pids[i], SIGKILL);
    }
    kill(male_worker_pid, SIGKILL);
    kill(female_worker_pid, SIGKILL);
    kill(male_secofficer_pid, SIGKILL);
    kill(female_secofficer_pid, SIGKILL);
    exit(0);
}