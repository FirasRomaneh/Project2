#include "local.h"

int main(int argc, char* args[]){
    char* gender = args[0];
    int m;
	sem_t *m1;
    if(strcmp(gender, "MALE") == 0){
        m = msgget(5000,0666|IPC_CREAT); //create new queue
        if ((m1 = sem_open("mysemaphore1", O_CREAT, 0644, 1)) == SEM_FAILED) {
            perror("semaphore initialization");
            exit(1);
        }
    } else {
        m = msgget(7000,0666|IPC_CREAT); //create new queue
        if ((m1 = sem_open("mysemaphore2", O_CREAT, 0644, 1)) == SEM_FAILED) {
            perror("semaphore initialization");
            exit(1);
        }
    }
    sem_init(m1, 1, 3);
    sleep(8);
    while (1){
        msgrcv(m, &person, sizeof(person)-4, 1, 0);
        person.type = person.pid;
        sleep(2);
        msgsnd(m, &person, sizeof(person)-4 ,0);// send pointer to p on the queue
    }
}