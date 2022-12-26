#include "local.h"

pid_t pid;
pid_t pids[100];
pid_t pids_teller[4];
pid_t male_worker_pid, female_worker_pid;
pid_t male_secofficer_pid, female_secofficer_pid;
void kill_all(int);
int mid1,mid2,mid3, shmID, max_num_of_people;
sem_t *sem1, *sem2, *sem3, *sem4, *sem5;

int main () {
    char word[30], value[10];
    int area2_max_capacity;
    int maxnumber_people_leave_unserved, maxnumber_people_leave_unhappy, maxnumber_people_leave_satisfied;
    int num_of_people_left_unserved, num_of_people_left_unhappy, num_of_people_left_satisfied;
    FILE* file = fopen("userdefined.txt","r");
    if(file == NULL) {
        printf("File NOT Found\n");
        exit(0);
    }
    char temp [30];
    fgets(temp, 250, file);
    strcpy(word, strtok(temp," "));
    max_num_of_people = atoi(strtok(NULL,"\0"));

    fgets(temp, 250, file);
    strcpy(word, strtok(temp," "));
    area2_max_capacity = atoi(strtok(NULL,"\0"));

    fgets(temp, 250, file);
    strcpy(word, strtok(temp," "));
    maxnumber_people_leave_unserved = atoi(strtok(NULL,"\0"));

    fgets(temp, 250, file);
    strcpy(word, strtok(temp," "));
    maxnumber_people_leave_unhappy = atoi(strtok(NULL,"\0"));

    fgets(temp, 250, file);
    strcpy(word, strtok(temp," "));
    maxnumber_people_leave_satisfied = atoi(strtok(NULL,"\0"));
    shmID = shmget(1000, sizeof(int)*9, 0666 | IPC_CREAT);
    mid1 = msgget(5000,0666|IPC_CREAT); //create new queue
    mid2 = msgget(7000,0666|IPC_CREAT); //create new queue
    mid3 = msgget(9000,0666|IPC_CREAT);
    int* s;
    if ((sem1 = sem_open("mysemaphore3", O_CREAT, 0644, 1)) == SEM_FAILED) {
		perror("semaphore initialization");
		exit(1);
	}
    if ((sem2 = sem_open("mysemaphore4", O_CREAT, 0644, 1)) == SEM_FAILED) {
		perror("semaphore initialization");
		exit(1);
	}
    if ((sem3 = sem_open("mysemaphore5", O_CREAT, 0644, 1)) == SEM_FAILED) {
		perror("semaphore initialization");
		exit(1);
	}
    if ((sem4 = sem_open("mysemaphore1", O_CREAT, 0644, 1)) == SEM_FAILED) {
		perror("semaphore initialization");
		exit(1);
	}
    if ((sem5 = sem_open("mysemaphore2", O_CREAT, 0644, 1)) == SEM_FAILED) {
		perror("semaphore initialization");
		exit(1);
	}
    sem_init(sem1, 1, 1);
    sem_init(sem2, 1, 1);
    sem_init(sem3, 1, 1);
    sem_init(sem4, 1, area2_max_capacity);
    sem_init(sem5, 1, area2_max_capacity);
    if(sigset(SIGINT, kill_all) == -1) {
        perror("sigset can not set SIGINT");
        exit(SIGINT);
    }
    pid = fork();
    if(pid == -1)  //error
        exit(-1);
    else if(pid == 0) { //w
        execl("./worker", "MALE", (char *) NULL);
        perror("worker exec failure ");
        exit(-2);
    }
    else {
        male_worker_pid = pid;
    }

    pid = fork();
    if(pid == -1)  //error
        exit(-1);
    else if(pid == 0) { //w
        execl("./worker", "FEMALE", (char *) NULL);
        perror("worker exec failure ");
        exit(-2);
    }
    else {
        female_worker_pid = pid;
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
        male_secofficer_pid = pid;
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
        female_secofficer_pid = pid;
    }
    char ticket_array[4][2] = {"B", "T", "R", "I"};
    printf("\033[0;36m");
    printf("Running............\n"); 
    fflush(stdout);
    for(int i=0; i<max_num_of_people; i++) {       // forking people
        pid = fork();
        if(pid == -1)  //error
            exit(-1);
        else if(pid == 0) { //people
            sleep(1);
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
            sem_wait(sem3);
            s = (int *)shmat(shmID, NULL, 0);
            s[0]++;
            shmdt(s);
            sem_post(sem3);
            pids[i] = pid;
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
    while(1){
        sem_wait(sem3);
        s = (int *)shmat(shmID, NULL, 0);
        system("clear") ;        
        printf("\033[0;36m");
        printf("Total number of people = %d\n", s[0]);
        printf("\033[1;33m");
        printf("Number of MALE people in Area 1 outside the OIM = %d\n", s[1]);
        printf("\033[0;32m");
        printf("Number of FEMALE people in Area 1 outside the OIM = %d\n", s[2]);
        printf("\033[0;31m");
        printf("Number of MALE people in Area 2 = %d\n", s[3]);
        printf("\033[0;34m");
        printf("Number of FEMALE people in Area 2 = %d\n", s[4]);
        printf("\033[0;35m");
        printf("Number of people in Inner Grouping Area = %d\n", s[5]);
        printf("\033[0;30m");
        num_of_people_left_unserved = s[6];
        printf("Number of people who were UNSERVED = %d\n", num_of_people_left_unserved);
        printf("\033[0;37m");
        num_of_people_left_unhappy = s[7];
        printf("Number of people who were UNHAPPY = %d\n", num_of_people_left_unhappy);
        printf("\033[0;32m");
        num_of_people_left_satisfied = s[8];
        printf("Number of people who were SATISFIED = %d\n", num_of_people_left_satisfied);
        fflush(stdout);
        shmdt(s);
        sem_post(sem3);
        printf("\033[0;30m");
        if(num_of_people_left_unserved > maxnumber_people_leave_unserved){
            printf("The program ends due to the number of people left unserved is above the given threshold\n");
            kill_all(SIGKILL);
        } 
        if(num_of_people_left_unhappy > maxnumber_people_leave_unhappy){
            printf("The program ends due to the number of people left unhappy is above the given threshold\n");
            kill_all(SIGKILL);
        } 
        if(num_of_people_left_satisfied > maxnumber_people_leave_satisfied){
            printf("The program ends due to the number of people left satisfied is above the given threshold\n");
            kill_all(SIGKILL);
        }
        fflush(stdout);
        sleep(2);
    }
}

void kill_all(int sig) {
    for(int i=0; i<max_num_of_people; i++) {
    	kill(pids[i], SIGKILL);
    }
    for(int i=0; i<4; i++) {
    	kill(pids_teller[i], SIGKILL);
    }
    kill(male_worker_pid, SIGKILL);
    kill(female_worker_pid, SIGKILL);
    kill(male_secofficer_pid, SIGKILL);
    kill(female_secofficer_pid, SIGKILL);
    msgctl(mid1, IPC_RMID, (struct msqid_ds *) 0);
    msgctl(mid2, IPC_RMID, (struct msqid_ds *) 0);
    msgctl(mid3, IPC_RMID, (struct msqid_ds *) 0);
    shmctl(shmID, IPC_RMID, (struct shmid_ds *) 0);
    semctl(sem1, 0, IPC_RMID, 0);
    semctl(sem2, 0, IPC_RMID, 0);
    semctl(sem3, 0, IPC_RMID, 0);
    semctl(sem4, 0, IPC_RMID, 0);
    semctl(sem5, 0, IPC_RMID, 0);
    exit(0);
}