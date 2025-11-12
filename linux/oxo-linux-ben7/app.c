#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char data[48];
    struct Node *next;
    void (*process)(struct Node*);
} Node;

typedef struct {
    Node *head;
    int count;
} LinkedList;

void process_node(Node *node) {
    printf("Processing: %s\n", node->data);
}

LinkedList* list_create(void) {
    LinkedList *list = malloc(sizeof(LinkedList));
    if (!list) return NULL;
    
    list->head = NULL;
    list->count = 0;
    return list;
}

Node* list_append(LinkedList *list, const char *data) {
    Node *node = malloc(sizeof(Node));
    if (!node) return NULL;
    
    strcpy(node->data, data);
    node->next = NULL;
    node->process = process_node;
    
    if (!list->head) {
        list->head = node;
    } else {
        Node *current = list->head;
        while (current->next) {
            current = current->next;
        }
        current->next = node;
    }
    
    list->count++;
    return node;
}

void list_update(LinkedList *list, int index, const char *data) {
    Node *current = list->head;
    for (int i = 0; i < index && current; i++) {
        current = current->next;
    }
    
    if (current) {
        strcpy(current->data, data);
    }
}

void list_process_all(LinkedList *list) {
    Node *current = list->head;
    while (current) {
        if (current->process) {
            current->process(current);
        }
        current = current->next;
    }
}

int main() {
    printf("Linked List Processor\n");
    printf("====================\n\n");
    
    LinkedList *list = list_create();
    
    while (1) {
        printf("Commands:\n");
        printf("  1 - Add node\n");
        printf("  2 - Update node\n");
        printf("  3 - Process all\n");
        printf("  4 - Display list\n");
        printf("  5 - Exit\n");
        printf("Choice: ");
        
        int choice;
        if (scanf("%d", &choice) != 1) break;
        getchar();
        
        if (choice == 1) {
            printf("Data: ");
            char data[256];
            if (!fgets(data, sizeof(data), stdin)) continue;
            data[strcspn(data, "\n")] = 0;
            
            if (list_append(list, data)) {
                printf("Node added\n");
            }
        } else if (choice == 2) {
            printf("Node index: ");
            int idx;
            scanf("%d", &idx);
            getchar();
            
            printf("New data: ");
            char data[256];
            if (!fgets(data, sizeof(data), stdin)) continue;
            data[strcspn(data, "\n")] = 0;
            
            list_update(list, idx, data);
            printf("Node updated\n");
        } else if (choice == 3) {
            list_process_all(list);
        } else if (choice == 4) {
            Node *current = list->head;
            int i = 0;
            while (current) {
                printf("[%d] %s\n", i++, current->data);
                current = current->next;
            }
        } else if (choice == 5) {
            break;
        }
        printf("\n");
    }
    
    return 0;
}
