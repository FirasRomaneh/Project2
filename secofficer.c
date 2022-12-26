#include "local.h"

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
        msgrcv(m, &person, sizeof(person)-4, 2, 0);
        person.type = person.pid;
        printf("\033[0;32m");
        printf("A %s person with PID = %d is body checking\n", person.gender, person.pid);
        fflush(stdout);
        msgsnd(m, &person, sizeof(person)-4 ,0);// send pointer to p on the queue
        sleep(4);
    }
}