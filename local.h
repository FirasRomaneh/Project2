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

struct people{
	long int type;
    char gender[10];
    char ticket[10];
    int served;
	int pid; // to know to which client to return the file data
} person;