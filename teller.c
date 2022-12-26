#include "local.h"

int main(int argc, char* args[]){
    char* teller_type = args[0];
    int teller_type_int = atoi(teller_type);
    int m = msgget(9000, 0666|IPC_CREAT);
    while (1){
        msgrcv(m, &person, sizeof(person)-4, teller_type_int, 0);
        srand(time(0)*person.pid);
        int P = (rand() % (10)) + 1;
        if(P > 8){
            person.served = 0;
        } else {
            person.served = 1;
        }
        person.type = person.pid;
        sleep(4);
        msgsnd(m, &person, sizeof(person)-4 ,0);// send pointer to p on the queue
    }
}