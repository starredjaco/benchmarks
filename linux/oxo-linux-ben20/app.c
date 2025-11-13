#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Message {
    char content[64];
    struct Message *next;
    void (*display)(struct Message*);
} Message;

typedef struct {
    Message *head;
    Message *tail;
    int count;
} MessageQueue;

void display_message(Message *msg) {
    printf("Message: %s\n", msg->content);
}

void display_priority_message(Message *msg) {
    printf("[PRIORITY] %s\n", msg->content);
}

MessageQueue* queue_create(void) {
    MessageQueue *queue = malloc(sizeof(MessageQueue));
    if (!queue) return NULL;
    
    queue->head = NULL;
    queue->tail = NULL;
    queue->count = 0;
    return queue;
}

void queue_add(MessageQueue *queue, const char *content, int is_priority) {
    Message *msg = malloc(sizeof(Message));
    if (!msg) return;
    
    strcpy(msg->content, content);
    msg->next = NULL;
    msg->display = is_priority ? display_priority_message : display_message;
    
    if (!queue->head) {
        queue->head = msg;
        queue->tail = msg;
    } else {
        queue->tail->next = msg;
        queue->tail = msg;
    }
    
    queue->count++;
}

Message* queue_peek(MessageQueue *queue) {
    return queue->head;
}

void queue_remove(MessageQueue *queue) {
    if (!queue->head) return;
    
    Message *msg = queue->head;
    queue->head = msg->next;
    
    if (!queue->head) {
        queue->tail = NULL;
    }
    
    free(msg);
    queue->count--;
}

int main() {
    MessageQueue *queue = queue_create();
    
    printf("Message Queue System\n");
    printf("===================\n\n");
    
    while (1) {
        printf("Commands:\n");
        printf("  1 - Add message\n");
        printf("  2 - View next message\n");
        printf("  3 - Remove message\n");
        printf("  4 - Display message\n");
        printf("  5 - Queue info\n");
        printf("  6 - Exit\n");
        printf("Choice: ");
        
        int choice;
        if (scanf("%d", &choice) != 1) break;
        getchar();
        
        if (choice == 1) {
            printf("Content: ");
            char content[128];
            if (!fgets(content, sizeof(content), stdin)) continue;
            content[strcspn(content, "\n")] = 0;
            
            printf("Priority (1/0): ");
            int priority;
            scanf("%d", &priority);
            
            queue_add(queue, content, priority);
            printf("Message added\n");
        } else if (choice == 2) {
            Message *msg = queue_peek(queue);
            if (msg) {
                printf("Next: %s\n", msg->content);
            } else {
                printf("Queue empty\n");
            }
        } else if (choice == 3) {
            queue_remove(queue);
            printf("Message removed\n");
        } else if (choice == 4) {
            Message *msg = queue_peek(queue);
            if (msg && msg->display) {
                msg->display(msg);
            } else {
                printf("Queue empty\n");
            }
        } else if (choice == 5) {
            printf("Messages in queue: %d\n", queue->count);
        } else if (choice == 6) {
            break;
        }
        printf("\n");
    }
    
    return 0;
}
