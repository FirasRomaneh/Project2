#include "local.h"

void signal_catcher(int);

int main(int argc, char* args[]){
    person.pid = getpid();
    char* gender = args[0];
    strcpy(person.ticket, args[1]);
    strcpy(person.gender, gender);
    int mid1;
    sem_t *m1;
    char str_ticket[9];
    int shmID = shmget(1000, sizeof(int)*9, 0666 | IPC_CREAT);
    int* s;
    sem_t *sem1, *sem2;
    int index_Area1, index_Area2;
    if(strcmp(gender, "MALE") == 0){
        index_Area1 = 1;
        index_Area2 = 3;
	    mid1 = msgget(5000,0666|IPC_CREAT); //create new queue
	    if ((m1 = sem_open("mysemaphore1", O_CREAT, 0644, 1)) == SEM_FAILED) {
		    perror("semaphore initialization");
		    exit(1);
	    }
        if ((sem1 = sem_open("mysemaphore3", O_CREAT, 0644, 1)) == SEM_FAILED) {
		    perror("semaphore initialization");
		    exit(1);
	    }
        if ((sem2 = sem_open("mysemaphore5", O_CREAT, 0644, 1)) == SEM_FAILED) {
		    perror("semaphore initialization");
		    exit(1);
	    }
    } else {
        index_Area1 = 2;
        index_Area2 = 4;
        mid1 = msgget(7000,0666|IPC_CREAT); //create new queue
        if ((m1 = sem_open("mysemaphore2", O_CREAT, 0644, 1)) == SEM_FAILED) {
		    perror("semaphore initialization");
		    exit(1);
	    }
        if ((sem1 = sem_open("mysemaphore4", O_CREAT, 0644, 1)) == SEM_FAILED) {
		    perror("semaphore initialization");
		    exit(1);
	    }
        if ((sem2 = sem_open("mysemaphore5", O_CREAT, 0644, 1)) == SEM_FAILED) {
		    perror("semaphore initialization");
		    exit(1);
	    }
    }
    int mid2 = msgget(9000,0666|IPC_CREAT); //create new queue
    sem_wait(sem1);
    s = (int *)shmat(shmID, NULL, 0);
    s[index_Area1]++;
    shmdt(s);
    sem_post(sem1);
    person.type = 1;
	msgsnd(mid1, &person, sizeof(person)-4, 0);// send pointer to p on the queue
    sleep(2);
    srand(time(0)*person.pid);
    int P = (rand() % (10)) + 1;
    if(P > 9){
        sem_wait(sem2);
        s = (int *)shmat(shmID, NULL, 0);
        s[11]++;
        shmdt(s);
        sem_post(sem2);
        sem_wait(sem1);
        s = (int *)shmat(shmID, NULL, 0);
        s[index_Area1]--;
        shmdt(s);
        sem_post(sem1);
        exit(0);
    } 
    msgrcv(mid1, &person, sizeof(person)-4, getpid(), 0);
    sem_wait(m1);
    sem_wait(sem1);
    s = (int *)shmat(shmID, NULL, 0);
    s[index_Area1]--;
    s[index_Area2]++;
    shmdt(s);
    sem_post(sem1);
    person.pid = getpid();
    person.type = 2;
    msgsnd(mid1, &person, sizeof(person)-4 ,0);// send pointer to p on the queue
    msgrcv(mid1, &person, sizeof(person)-4, getpid(), 0);
    sem_post(m1);
    sem_wait(sem1);
    s = (int *)shmat(shmID, NULL, 0);
    s[index_Area2]--;
    shmdt(s);
    sem_post(sem1);
    sem_wait(sem2);
    s = (int *)shmat(shmID, NULL, 0);
    s[5]++;
    shmdt(s);
    sem_post(sem2);
    sprintf(str_ticket, "%d", getpid());
    strcat(person.ticket, str_ticket);
    person.served = 0;
    person.pid = getpid();
    int type_ticket = 0;
    if(person.ticket[0] == 'B'){
        type_ticket = 1;
        sem_wait(sem2);
        s = (int *)shmat(shmID, NULL, 0);
        s[6]++;
        shmdt(s);
        sem_post(sem2);
    } else if(person.ticket[0] == 'T'){
        type_ticket = 2;
        sem_wait(sem2);
        s = (int *)shmat(shmID, NULL, 0);
        s[7]++;
        shmdt(s);
        sem_post(sem2);
    } else if(person.ticket[0] == 'R'){
        type_ticket = 3;
        sem_wait(sem2);
        s = (int *)shmat(shmID, NULL, 0);
        s[8]++;
        shmdt(s);
        sem_post(sem2);
    } else if(person.ticket[0] == 'I'){
        type_ticket = 4;
        sem_wait(sem2);
        s = (int *)shmat(shmID, NULL, 0);
        s[9]++;
        shmdt(s);
        sem_post(sem2);
    }
    person.pid = getpid();
    person.type = type_ticket;
    msgsnd(mid2, &person, sizeof(person)-4 ,0);// send pointer to p on the queue
    sleep(2);
    srand(time(0)*person.pid);
    P = (rand() % (10)) + 1;
    if(P > 9){
        sem_wait(sem2);
        s = (int *)shmat(shmID, NULL, 0);
        s[11]++;
        s[5]--;
        if(type_ticket == 1){
            s[6]--;
        } else if(type_ticket == 2){
            s[7]--;
        } else if(type_ticket == 3){
            s[8]--;
        } else if(type_ticket == 4){
            s[9]--;
        }
        shmdt(s);
        sem_post(sem2);
        exit(0);
    } 
    msgrcv(mid2, &person, sizeof(person)-4, getpid(), 0);
    if(person.served == 1){
        sem_wait(sem2);
        s = (int *)shmat(shmID, NULL, 0);
        s[12]++;
        s[5]--;
        if(type_ticket == 1){
            s[6]--;
        } else if(type_ticket == 2){
            s[7]--;
        } else if(type_ticket == 3){
            s[8]--;
        } else if(type_ticket == 4){
            s[9]--;
        }
        shmdt(s);
        sem_post(sem2);
    } else {
        sem_wait(sem2);
        s = (int *)shmat(shmID, NULL, 0);
        s[10]++;
        s[5]--;
        if(type_ticket == 1){
            s[6]--;
        } else if(type_ticket == 2){
            s[7]--;
        } else if(type_ticket == 3){
            s[8]--;
        } else if(type_ticket == 4){
            s[9]--;
        }
        shmdt(s);
        sem_post(sem2);
    }
    exit(0);
}