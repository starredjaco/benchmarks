#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[64];
    int age;
    void (*greet)(const char*);
} Person;

void normal_greet(const char *name) {
    printf("Hello, %s!\n", name);
}

void premium_greet(const char *name) {
    printf("Welcome back, valued customer %s!\n", name);
}

Person* create_person(const char *name, int age, int is_premium) {
    Person *p = malloc(sizeof(Person));
    if (!p) return NULL;
    
    strcpy(p->name, name);
    p->age = age;
    p->greet = is_premium ? premium_greet : normal_greet;
    
    return p;
}

void delete_person(Person *p) {
    if (p) {
        free(p);
    }
}

void greet_person(Person *p) {
    if (p && p->greet) {
        p->greet(p->name);
    }
}

int main() {
    Person *people[10] = {0};
    int count = 0;
    
    printf("Person Manager\n");
    printf("=============\n\n");
    
    while (1) {
        printf("Commands:\n");
        printf("  1 - Create person\n");
        printf("  2 - Delete person\n");
        printf("  3 - Greet person\n");
        printf("  4 - List people\n");
        printf("  5 - Exit\n");
        printf("Choice: ");
        
        int choice;
        if (scanf("%d", &choice) != 1) break;
        getchar();
        
        if (choice == 1) {
            if (count >= 10) {
                printf("Maximum people reached\n");
                continue;
            }
            
            printf("Name: ");
            char name[128];
            if (!fgets(name, sizeof(name), stdin)) continue;
            name[strcspn(name, "\n")] = 0;
            
            printf("Age: ");
            int age;
            scanf("%d", &age);
            
            printf("Premium (1/0): ");
            int premium;
            scanf("%d", &premium);
            
            people[count] = create_person(name, age, premium);
            if (people[count]) {
                printf("Person created at index %d\n", count);
                count++;
            }
        } else if (choice == 2) {
            printf("Index: ");
            int idx;
            scanf("%d", &idx);
            
            if (idx >= 0 && idx < 10) {
                delete_person(people[idx]);
                printf("Person deleted\n");
            }
        } else if (choice == 3) {
            printf("Index: ");
            int idx;
            scanf("%d", &idx);
            
            if (idx >= 0 && idx < 10) {
                greet_person(people[idx]);
            }
        } else if (choice == 4) {
            for (int i = 0; i < count; i++) {
                if (people[i]) {
                    printf("[%d] %s\n", i, people[i]->name);
                }
            }
        } else if (choice == 5) {
            break;
        }
        printf("\n");
    }
    
    return 0;
}
