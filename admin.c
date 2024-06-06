#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdbool.h>
#include "common.h"

int main()
{
    int shmid=shmget(ftok("admin.c",1234), sizeof(1234), IPC_CREAT | 0666);
    int *shmPtr=shmat(shmid,NULL,0);
    *shmPtr=0;
    char choice='N';
    while(1){
    printf("Do you want to close the hotel? Enter Y for Yes and N for No.\n");
    scanf(" %c", &choice); // Add a space before %c
    if(choice=='Y'){
    *shmPtr=1;
    sleep(5);
    if(*shmPtr==1)
        break;
    else
    {
        printf("Customers are there\n");
    }
    }
}

    
    return 0;
}
