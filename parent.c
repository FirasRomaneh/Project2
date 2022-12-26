#include "local.h"

pid_t pid;
pid_t pids[10];
pid_t pids_teller[4];
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
    char ticket_array[4][2] = {"B", "T", "R", "I"};
    printf("\033[0;36m");
    printf("start Forking............\n"); 
    fflush(stdout);
    for(int i=0; i<10; i++) {       // forking people
        pid = fork();
        if(pid == -1)  //error
            exit(-1);
        else if(pid == 0) { //people
            srand(time(0)*i*(unsigned) getpid());
            int num = (rand() % (4)) + 1;
            if(i%2 == 0){
                execl("./people", "MALE", ticket_array[num-1], (char *) NULL);
            } else {
                execl("./people", "FEMALE", ticket_array[num-1], (char *) NULL);
            }
            perror("exec failure ");
            exit(-2);
        } else {  //parent
            pids[i] = pid;
            sleep(1);
        }
    }
    char teller[4][2] = {"1", "2", "3", "4"};
    for(int i = 0; i < 4; i++){
         pid = fork();
        if(pid == -1)  //error
            exit(-1);
        else if(pid == 0) { //people
            execl("./teller", "FEMALE", teller[i], (char *) NULL);
            perror("exec failure ");
            exit(-2);
        } else {  //parent
            pids_teller[i] = pid;
        }
    }
    
    
    
    while(1);
}

void kill_all(int sig) {
    for(int i=0; i<10; i++) {
    	kill(pids[i], SIGKILL);
    }
    for(int i=0; i<4; i++) {
    	kill(pids_teller[i], SIGKILL);
    }
    kill(male_worker_pid, SIGKILL);
    kill(female_worker_pid, SIGKILL);
    kill(male_secofficer_pid, SIGKILL);
    kill(female_secofficer_pid, SIGKILL);
    exit(0);
}