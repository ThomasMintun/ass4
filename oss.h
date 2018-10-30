#ifndef OSS_H
#define OSS_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <string.h>

//shared mem keys, probs not secure in a header
#define CLOCK_SHMKEY 696969
#define PCB_SHMKEY 969696

typedef struct PCB_t {
    pid_t pidHolder;
    float totalCPUTime;
    float timeInSystem;
    float timeLastBurst;
    int priotiyValue;
    int bitVector;
} PCB_t;//name PCB_t was down here

typedef struct systemClock_t{
    unsigned int seconds;
    unsigned int nanoseconds;
} systemClock_t;//name systemClock_t was down here

// globals for accessing pointers to shared memory
int sysClockshmid;//holds the shared memory segment id
int PCBshmid;
systemClock_t *sysClockshmPtr;//points to the data structure
PCB_t *PCBshmPtr;//points to the data structure

//allocates shared mem
void sharedMemoryConfig(){

    //shared mem for sysClock
    sysClockshmid = shmget(CLOCK_SHMKEY, sizeof(systemClock_t), IPC_CREAT|0777);
    if(sysClockshmid < 0){
        perror("sysClock shmget error in master \n");
        exit(errno);
    }
    sysClockshmPtr = shmat(sysClockshmid, NULL, 0);
    if(sysClockshmPtr < 0){
        perror("sysClock shmat error in oss\n");
        exit(errno);
    }

    // shared mem for PCB
    PCBshmid = shmget(PCB_SHMKEY, sizeof(PCB_t), IPC_CREAT|0777);
    if(PCBshmid < 0){
        perror("PCB shmget error in master \n");
        exit(errno);
    }
    PCBshmPtr = shmat(PCBshmid, NULL, 0);
    if(PCBshmPtr < 0){
        perror("PCB shmat error in oss\n");
        exit(errno);
    }

    //tests for shared mem
//    sysClockshmPtr->nanoseconds = 3;
//    printf("%d\n", sysClockshmPtr->nanoseconds);

//    sysClockshmPtr->nanoseconds = 0;
//    sysClockshmPtr->seconds = 0;
}

#endif