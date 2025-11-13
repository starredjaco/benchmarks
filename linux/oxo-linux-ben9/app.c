#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[64];
    int age;
    char *address;
} Person;

typedef struct {
    Person *people[20];
    int count;
} Database;

void init_database(Database *db) {
    db->count = 0;
    for (int i = 0; i < 20; i++) {
        db->people[i] = NULL;
    }
}

int add_person(Database *db, const char *name, int age, const char *address) {
    if (db->count >= 20) {
        return -1;
    }
    
    Person *p = malloc(sizeof(Person));
    if (!p) return -1;
    
    strcpy(p->name, name);
    p->age = age;
    
    if (address && strlen(address) > 0) {
        p->address = malloc(strlen(address) + 1);
        if (p->address) {
            strcpy(p->address, address);
        }
    } else {
        p->address = NULL;
    }
    
    db->people[db->count++] = p;
    return 0;
}

void remove_person(Database *db, int index) {
    if (index < 0 || index >= db->count) {
        return;
    }
    
    Person *p = db->people[index];
    if (p) {
        if (p->address) {
            free(p->address);
        }
        free(p);
        db->people[index] = NULL;
    }
}

void print_person(Person *p) {
    if (!p) {
        printf("Person not found\n");
        return;
    }
    
    printf("Name: %s\n", p->name);
    printf("Age: %d\n", p->age);
    printf("Address: %s\n", p->address ? p->address : "N/A");
}

int main() {
    Database db;
    init_database(&db);
    
    printf("Person Database\n");
    printf("==============\n\n");
    
    while (1) {
        printf("Commands:\n");
        printf("  1 - Add person\n");
        printf("  2 - Remove person\n");
        printf("  3 - View person\n");
        printf("  4 - List all\n");
        printf("  5 - Exit\n");
        printf("Choice: ");
        
        int choice;
        if (scanf("%d", &choice) != 1) break;
        getchar();
        
        if (choice == 1) {
            printf("Name: ");
            char name[128];
            if (!fgets(name, sizeof(name), stdin)) continue;
            name[strcspn(name, "\n")] = 0;
            
            printf("Age: ");
            int age;
            scanf("%d", &age);
            getchar();
            
            printf("Address: ");
            char address[256];
            if (!fgets(address, sizeof(address), stdin)) continue;
            address[strcspn(address, "\n")] = 0;
            
            if (add_person(&db, name, age, address) == 0) {
                printf("Person added\n");
            }
        } else if (choice == 2) {
            printf("Index: ");
            int idx;
            scanf("%d", &idx);
            remove_person(&db, idx);
            printf("Person removed\n");
        } else if (choice == 3) {
            printf("Index: ");
            int idx;
            scanf("%d", &idx);
            if (idx >= 0 && idx < 20) {
                print_person(db.people[idx]);
            }
        } else if (choice == 4) {
            for (int i = 0; i < 20; i++) {
                if (db.people[i]) {
                    printf("[%d] %s\n", i, db.people[i]->name);
                }
            }
        } else if (choice == 5) {
            break;
        }
        printf("\n");
    }
    
    return 0;
}
