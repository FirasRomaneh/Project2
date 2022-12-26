#include "local.h"
void signal_catcher(int);
int main(int argc, char* args[]){
    srand(time(0)*person.pid);
    person.patience_time = (rand() % (121)) + 30;
	 alarm(10); // will send SIGALRM after 1 second
	if (sigset(SIGALRM, signal_catcher) == SIG_ERR ) {
		perror("SIGSET cannot set SIGALRM");
		exit(SIGINT);
	}
    char* gender = args[0];
    strcpy(person.ticket, args[1]);
    strcpy(person.gender, gender);
    int mid1;
    sem_t *m1;
    char str_ticket[9];
    if(strcmp(gender, "MALE") == 0){
	    mid1 = msgget(5000,0666|IPC_CREAT); //create new queue
	    if ((m1 = sem_open("mysemaphore1", O_CREAT, 0644, 1)) == SEM_FAILED) {
		    perror("semaphore initialization");
		    exit(1);
	    }
    } else {
        mid1 = msgget(7000,0666|IPC_CREAT); //create new queue
        if ((m1 = sem_open("mysemaphore2", O_CREAT, 0644, 1)) == SEM_FAILED) {
		    perror("semaphore initialization");
		    exit(1);
	    }
    }
    int mid2 = msgget(9000,0666|IPC_CREAT); //create new queue
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
    sprintf(str_ticket, "%d", getpid());
    strcat(person.ticket, str_ticket);
    person.served = 0;
    person.pid = getpid();
    int type_ticket = 0;
    printf("\033[0;39m");
    if(person.ticket[0] == 'B'){
        type_ticket = 1;
        printf("The person with pid = %d got a BIRTH CERTIFICATE ticket\n", getpid());
    } else if(person.ticket[0] == 'T'){
        type_ticket = 2;
        printf("The person with pid = %d got a TRAVEL DOCUMENT ticket\n", getpid());
    } else if(person.ticket[0] == 'R'){
        type_ticket = 3;
        printf("The person with pid = %d got a FAMILY REUNION ticket\n", getpid());
    } else if(person.ticket[0] == 'I'){
        type_ticket = 4;
        printf("The person with pid = %d got an ID-RELATED ticket\n", getpid());
    }
    fflush(stdout);
    person.type = type_ticket;
    msgsnd(mid2, &person, sizeof(person)-4 ,0);// send pointer to p on the queue
    msgrcv(mid2, &person, sizeof(person)-4, getpid(), 0);
    printf("\033[0;33m");
    if(person.served == 1){
        printf("A %s person with pid = %d was served\n", person.gender, getpid());
    } else {
        printf("A %s person with pid = %d was NOT served\n", person.gender, getpid());
    }
    fflush(stdout);
    while(1);
}


void signal_catcher(int the_sig) {
    person.patience_time -= 30;
    if(person.patience_time <= 0){
        printf("\033[0;38m");
        printf("A %s person with pid = %d was impatient and left unserved\n", person.gender, getpid());
        fflush(stdout);
        exit(0);
    }
	alarm(10);//set an alarm after another second to trigger this function again
}