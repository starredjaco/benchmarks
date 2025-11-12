#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[32];
    char description[64];
    int priority;
    void (*handler)(void);
} Task;

typedef struct {
    Task *tasks;
    int count;
    int capacity;
} TaskQueue;

void default_handler(void) {
    printf("Task executed\n");
}

void high_priority_handler(void) {
    printf("High priority task executed\n");
}

TaskQueue* create_queue(int capacity) {
    TaskQueue *queue = malloc(sizeof(TaskQueue));
    if (!queue) return NULL;
    
    queue->tasks = malloc(sizeof(Task) * capacity);
    if (!queue->tasks) {
        free(queue);
        return NULL;
    }
    
    queue->count = 0;
    queue->capacity = capacity;
    return queue;
}

int add_task(TaskQueue *queue, const char *name, const char *desc, int priority) {
    if (queue->count >= queue->capacity) {
        return -1;
    }
    
    Task *task = &queue->tasks[queue->count];
    strcpy(task->name, name);
    strcpy(task->description, desc);
    task->priority = priority;
    task->handler = priority > 5 ? high_priority_handler : default_handler;
    
    queue->count++;
    return 0;
}

void execute_task(TaskQueue *queue, int index) {
    if (index < 0 || index >= queue->count) {
        printf("Invalid task index\n");
        return;
    }
    
    Task *task = &queue->tasks[index];
    printf("Executing: %s\n", task->name);
    if (task->handler) {
        task->handler();
    }
}

int main() {
    printf("Task Scheduler\n");
    printf("=============\n\n");
    
    TaskQueue *queue = create_queue(10);
    if (!queue) {
        printf("Failed to create queue\n");
        return 1;
    }
    
    while (1) {
        printf("Commands:\n");
        printf("  1 - Add task\n");
        printf("  2 - List tasks\n");
        printf("  3 - Execute task\n");
        printf("  4 - Exit\n");
        printf("Choice: ");
        
        int choice;
        if (scanf("%d", &choice) != 1) break;
        getchar();
        
        if (choice == 1) {
            printf("Task name: ");
            char name[128];
            if (!fgets(name, sizeof(name), stdin)) continue;
            name[strcspn(name, "\n")] = 0;
            
            printf("Description: ");
            char desc[256];
            if (!fgets(desc, sizeof(desc), stdin)) continue;
            desc[strcspn(desc, "\n")] = 0;
            
            printf("Priority (1-10): ");
            int priority;
            scanf("%d", &priority);
            
            if (add_task(queue, name, desc, priority) == 0) {
                printf("Task added\n");
            } else {
                printf("Queue full\n");
            }
        } else if (choice == 2) {
            for (int i = 0; i < queue->count; i++) {
                printf("[%d] %s (priority: %d)\n", i, queue->tasks[i].name, queue->tasks[i].priority);
            }
        } else if (choice == 3) {
            printf("Task index: ");
            int idx;
            scanf("%d", &idx);
            execute_task(queue, idx);
        } else if (choice == 4) {
            break;
        }
        printf("\n");
    }
    
    free(queue->tasks);
    free(queue);
    return 0;
}
