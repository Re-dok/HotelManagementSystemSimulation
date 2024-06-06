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

#define EARNINGS_FILE "earnings.txt"

typedef struct {
    int table_id;
    // int table_id;
    int number_of_customers;
    int bill;
    int menuSize;
    int Prices[10]; // Change from int Prices[9] to int Prices[MAX_MENU_ITEMS]
    int orders[10]; // Max items per customer
    bool orderValidty;
    bool waiterWorking;
    bool tableWorking;
    bool customer_count;
    bool terminater;
    bool stopper;
} TableData;

typedef struct{
    int bill;
    int number_of_customers;
} billData;

int main() {
    int total_tables;
    printf("Enter the Total Number of Tables at the Hotel: ");
    scanf("%d", &total_tables);

    // Open earnings file
    FILE *earnings_fp = fopen(EARNINGS_FILE, "w");
    if (earnings_fp == NULL) {
        perror("Error opening earnings file");
        exit(EXIT_FAILURE);
    }

    int shm_id[10];
    billData *DM[10];

    // Attach to shared memory segments for all tables
    for (int i = 0; i < total_tables; ++i) {
        
        key_t key=ftok(SHARED_MEM_KEY_FILE_2, (i+1)*10);
        // printf("%d\n",i);
        shm_id[i] = shmget(key, sizeof(billData), IPC_CREAT | 0666);
        
        if (shm_id[i] == -1) {
            perror("Error creating shared memory for waiter data");
            exit(EXIT_FAILURE);
        }
        DM[i] = shmat(shm_id[i], NULL, 0);
        if (DM[i] == (void *)-1) {
            perror("Error attaching shared memory for waiter data");
            exit(EXIT_FAILURE);
        }
    }
    for(int i = 0; i < total_tables; ++i)
    {
        DM[i]->number_of_customers=-1;
    }

    float total_earnings = 0;
    // float 

    // Loop until termination signal received
    int shmid = shmget(ftok("admin.c",1234), sizeof(1234), IPC_CREAT | 0666);
    int *shmPtr = shmat(shmid, NULL, 0);
    bool ch=false;
    int totalCustomers=0;
    while(ch==false){
    
        // Collect earnings from all tables
        totalCustomers=0;
        for (int i = 0; i < total_tables; i++) {
            wait(NULL);
            if(DM[i]->bill!=0){
                total_earnings += DM[i]->bill;
                printf("Earning from Table %d: %d INR\n", i + 1, DM[i]->bill);
                fprintf(earnings_fp, "Earning from Table %d: %d INR\n", i + 1, DM[i]->bill);
                DM[i]->bill = 0; // Reset bill to avoid recounting
            }
            
        }
                for(int i = 0; i < total_tables; i++)
                {
                    if(DM[i]->number_of_customers!=-1)
                    {

                        totalCustomers++;
                    }
                }
        if(*shmPtr==1)
        {
                        printf("here2\n");

            if(totalCustomers!=0)
            *shmPtr=0;
            else
            ch=true;
        }
        
    }
    
	
    // Wait and collect until all tables are empty
    for (int i = 0; i < total_tables; i++) {
        if (DM[i]->bill != 0) {
            wait(NULL);
            total_earnings += DM[i]->bill;
            fprintf(earnings_fp, "Earning from Table %d: %d INR\n", i + 1, DM[i]->bill);
            DM[i]->bill = 0; // Reset bill to avoid recounting
        }
    }

    // Print total earnings
    fprintf(earnings_fp, "Total Earnings of Hotel: %f INR\n", total_earnings);
    fprintf(earnings_fp, "Total Wages of Waiters: %f INR\n", 0.4*total_earnings);
    fprintf(earnings_fp, "Total Profit: %f INR\n", 0.6*total_earnings);
    printf("Total Earnings of Hotel: %f INR\n", total_earnings);
    
    // Close earnings file
    fclose(earnings_fp);

    // Detach from shared memory and remove segments
    for (int i = 0; i < total_tables; ++i) {
        shmdt(DM[i]);
        shmctl(shm_id[i], IPC_RMID, NULL);
    }

    // Display termination message
    printf("Thank you for visiting the Hotel!\n");

    return 0;
}//hotelmanager.c
