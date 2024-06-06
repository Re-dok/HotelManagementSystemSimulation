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

#define BUFFER_SIZE 25
#define MAX_MENU_ITEMS 10
#define READ_END 0
#define WRITE_END 1

typedef struct {
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
    bool red;
    // int m;
} TableData;

int main() {
    key_t key;
    int shmid;
    int y = 0;
    TableData *shared_table;

    printf("Enter Table number: ");
    scanf("%d", &y);

    // Generate a key for shared memory segment
    if ((key = ftok(SHARED_MEM_KEY_FILE, y*10)) == -1) {
        perror("ftok");
        exit(1);
    }

    // Create shared memory segment for table data
    if ((shmid = shmget(key, sizeof(TableData), IPC_CREAT | 0666)) == -1) {
        perror("shmget");
        exit(1);
    }

    // Attach shared memory segment for table data
    if ((shared_table = (TableData *)shmat(shmid, NULL, 0)) == (TableData *)(-1)) {
        perror("shmat");
        exit(1);
    }

    // shared_table->table_id = y;
shared_table->terminater=true;
while(true){
    shared_table->orderValidty=false;
    // shared_table->m = 0;
//----
    shared_table->stopper=true;
    // shared_table->red=false;
    //Geting number of custormers at the table
    printf("Enter Number of Customers at Table (maximum no. of customers can be %d): ", MAX_MENU_ITEMS);
    scanf("%d", &shared_table->number_of_customers);

    if(shared_table->number_of_customers==-1){
        shared_table->terminater=false;
        shared_table->red=true;
        break;
    }

    // shared_table->customer_count = shared_table->number_of_customers;
    // Reading menu prices
    FILE* ptr;
    ptr = fopen("menu.txt", "r");
    if (ptr == NULL) {
        printf("File can't be opened\n");
        exit(1);
    }
    int num = 0;
    bool flag = true;
    int ptr2 = 0;
    int pp = 0;
    shared_table->menuSize = 0;
    printf("Content of menu.txt:\n");
    char ch; // Declare the variable ch
    do {
        ch = fgetc(ptr); // Assign value to ch
        if (ch >= '0' && ch <= '9') {
            int dig = ch - '0';
            num = num * 10 + dig;
            flag = true;
        }
        if (ch == ' ' && flag == true) {
            if (pp % 2 == 1) {
                shared_table->menuSize++;
                shared_table->Prices[ptr2] = num;
                ptr2++;
            }
            pp++;
            num = 0;
            flag = false;
        }
        putchar(ch);
    } while (ch != EOF);
    fclose(ptr);
    printf("\n");
    // int dummy=0;
    
           int m;
while(shared_table->orderValidty==false){
    shared_table->orderValidty=false;
    // Initialize orders array to zero
    shared_table->tableWorking=true;
    shared_table->waiterWorking=true;
    for (int i = 0; i < MAX_MENU_ITEMS; i++) {
        shared_table->orders[i] = 0;
    }

    // Update shared memory with orders
    shared_table->customer_count = false;
    for (int i = 0; i < shared_table->number_of_customers; i++) {
        int fd[2];
        if (pipe(fd) == -1) {
            fprintf(stderr, "PIPE failed");
            return 1;
        }
        pid_t pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            return 1;
        } else if (pid == 0) { // Child process
            close(fd[READ_END]);
            int ptr = 0;
            // int y = i + 1;
            printf("Give order for customer %d (enter -1 to finish): ", i+1);
            int k;
            do {
                scanf("%d", &k);
                write(fd[WRITE_END], &k, sizeof(int));
            } while (k != -1 && ptr < MAX_MENU_ITEMS);
            close(fd[WRITE_END]);
            m--;
            if(m==0)
            {
              shared_table->customer_count = true;
                m = (int)shared_table->number_of_customers;
            }
            return 0;
        }
        close(fd[WRITE_END]);
        int read_msg;
        while (read(fd[READ_END], &read_msg, sizeof(int)) > 0) {
            // printf("In read_msg:%d\n", read_msg);
            if (read_msg > 0 && read_msg <= shared_table->menuSize) {                    
                shared_table->orders[read_msg]++;
            }
            else if(read_msg>shared_table->menuSize){
                shared_table->orders[0]=1;
            }
        }
        close(fd[READ_END]);
        wait(NULL);
    }


    shared_table->tableWorking=false;
    shared_table->waiterWorking=true;
    shared_table->red=true;
    while(shared_table->waiterWorking==true)
        sleep(1);
    if(shared_table->orderValidty==false){
        printf("Invalid order given,please re-enter\n");
        continue;   
    }
    
}
    shared_table->stopper=false;
    printf("Total bill is : %d \n\n", shared_table->bill);

}
    shmdt(shared_table);

   if (shmctl(shmid, IPC_RMID, 0) == -1) {
        perror("Error in shmctl\n");
        return 1;
    }

    return 0;
}