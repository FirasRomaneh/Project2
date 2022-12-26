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
        sleep(4);
        person.type = person.pid;
        msgsnd(m, &person, sizeof(person)-4 ,0);// send pointer to p on the queue
    }
}