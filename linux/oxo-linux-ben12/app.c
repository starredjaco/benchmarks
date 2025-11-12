#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
    int balance;
    char owner[64];
    int transaction_count;
} BankAccount;

BankAccount account = {1000, "Alice", 0};

void* deposit_thread(void *arg) {
    int amount = *(int*)arg;
    
    printf("[Deposit] Reading balance: %d\n", account.balance);
    sleep(1);
    
    int new_balance = account.balance + amount;
    printf("[Deposit] Updating balance to: %d\n", new_balance);
    
    account.balance = new_balance;
    account.transaction_count++;
    
    return NULL;
}

void* withdraw_thread(void *arg) {
    int amount = *(int*)arg;
    
    printf("[Withdraw] Reading balance: %d\n", account.balance);
    sleep(1);
    
    if (account.balance >= amount) {
        int new_balance = account.balance - amount;
        printf("[Withdraw] Updating balance to: %d\n", new_balance);
        account.balance = new_balance;
        account.transaction_count++;
    } else {
        printf("[Withdraw] Insufficient funds\n");
    }
    
    return NULL;
}

void process_transaction(int type, int amount) {
    pthread_t thread;
    int *amt = malloc(sizeof(int));
    *amt = amount;
    
    if (type == 1) {
        pthread_create(&thread, NULL, deposit_thread, amt);
    } else {
        pthread_create(&thread, NULL, withdraw_thread, amt);
    }
    
    pthread_detach(thread);
}

int main() {
    printf("Bank Account System\n");
    printf("==================\n\n");
    
    printf("Account owner: %s\n", account.owner);
    printf("Initial balance: $%d\n\n", account.balance);
    
    while (1) {
        printf("Commands:\n");
        printf("  1 - Deposit\n");
        printf("  2 - Withdraw\n");
        printf("  3 - Check balance\n");
        printf("  4 - Exit\n");
        printf("Choice: ");
        
        int choice;
        if (scanf("%d", &choice) != 1) break;
        
        if (choice == 1 || choice == 2) {
            printf("Amount: $");
            int amount;
            scanf("%d", &amount);
            
            process_transaction(choice, amount);
            printf("Transaction initiated...\n");
            sleep(2);
        } else if (choice == 3) {
            printf("Current balance: $%d\n", account.balance);
            printf("Transactions: %d\n", account.transaction_count);
        } else if (choice == 4) {
            break;
        }
        printf("\n");
    }
    
    return 0;
}
