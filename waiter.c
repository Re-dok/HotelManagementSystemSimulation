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

#define MAX_MENU_ITEMS 10
#define MAX_CUSTOMERS 5

typedef struct {
    int number_of_customers;
    int bill;
    int menuSize;
    int Prices[10];
    int orders[10];
    bool orderValidty;
    bool waiterWorking;
    bool tableWorking;
    bool customer_count;
    bool terminater;
    bool stopper;
    bool red;
} TableData;

typedef struct{
    int bill;
    int number_of_customers;
} billData;

int main() {
    key_t key;
    int shmid;
    TableData *shared_table;
    billData *shared_table2;

    printf("Enter Waiter ID: ");
    int waiter_id;
    scanf("%d", &waiter_id);
    // printf("hi1");
    // Generate a key for shared memory segment
    if ((key = ftok(SHARED_MEM_KEY_FILE, waiter_id*10)) == -1) {
        perror("ftok");
        exit(1);
    }

    // Create shared memory segment for table data
    if ((shmid = shmget(key, sizeof(TableData), 0666)) == -1) {
        perror("shmget");
        exit(1);
    }

    // Attach shared memory segment for table data
    if ((shared_table = (TableData *)shmat(shmid, NULL, 0)) == (TableData *) -1) {
        perror("shmat");
        exit(1);
    }
    
    //temp Change to take repeat order
   key_t key_hm;
   int shmid_hm;
   
if ((key_hm = ftok(SHARED_MEM_KEY_FILE_2, waiter_id*10)) == -1) {
        perror("ftok");
        exit(1);
    }

    // Create shared memory segment for table data
    if ((shmid_hm = shmget(key_hm, sizeof(billData), IPC_CREAT | 0666)) == -1) {
        perror("shmget");
        exit(1);
    }

    // Attach shared memory segment for table data
    if ((shared_table2 = (billData *)shmat(shmid_hm, NULL, 0)) == (billData *) -1) {
        perror("shmat");
        exit(1);
    }

//----
    shared_table->stopper=true;
    
    while(shared_table->terminater){

    shared_table2->number_of_customers=1;
    // sleep(10);
    while(shared_table->red==false)
    {
        sleep(1);
    }
    shared_table->red=false;
    shared_table->stopper=true;
    while(shared_table->orderValidty==false){
    if(shared_table->orders[0]!=0)
    {
        
    shared_table->waiterWorking=false;
    while(shared_table->tableWorking==true){ 
        // printf("something"); 
        sleep(1);
        }
    }
    else
    {   shared_table->waiterWorking=false;
        shared_table->orderValidty=true;
      if(shared_table->customer_count == true) 
      {
        break;
      }
    }

    }
    shared_table->orderValidty=true;
    shared_table->waiterWorking=false;
    //fa
//   sleep(10);
    // printf("Total tables: %d\n", MAX_TABLES);

    // Read and display orders for each table
    shared_table->bill=0;
    for(int i=1;i<10;i++)
    {
        int p=(shared_table->orders[i]);
        shared_table->bill+=(shared_table->Prices[i-1])*p;
    }
    if(shared_table->number_of_customers!=-1)
    {
            printf("The bill for table %d is %d\n",waiter_id,shared_table->bill);
            shared_table2->bill=shared_table->bill;
    }
    // sleep(10);
    // shared_table2->bill=0;
    }
    shared_table2->number_of_customers=-1;
   shmdt(shared_table);
   shmdt(shared_table2);
    
    
    return 0;   
}
//waiter.c