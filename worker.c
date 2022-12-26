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
    sleep(8);
    while (1){
        msgrcv(m, &person, sizeof(person)-4, 1, 0);
        person.type = person.pid;
        char piddw[20];
        sprintf(piddw, "/proc/%d/stat", person.pid);
        FILE* f = fopen(piddw, "r");
        int unused;
        char comm[1000];
        char state;
        int ppid;
        fscanf(f, "%d %s %c %d", &unused, comm, &state, &ppid);
        if(state == 'Z'){
            continue;
        }
        fclose(f);
        sleep(2);
        msgsnd(m, &person, sizeof(person)-4 ,0);// send pointer to p on the queue
    }
}