#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RECORDS 100

typedef struct {
    int id;
    char name[64];
    float score;
} Record;

typedef struct {
    Record records[MAX_RECORDS];
    int count;
} RecordStore;

void init_store(RecordStore *store) {
    store->count = 0;
    memset(store->records, 0, sizeof(store->records));
}

int add_record(RecordStore *store, int id, const char *name, float score) {
    if (store->count >= MAX_RECORDS) {
        return -1;
    }
    
    Record *r = &store->records[store->count];
    r->id = id;
    strcpy(r->name, name);
    r->score = score;
    
    store->count++;
    return 0;
}

Record* get_record(RecordStore *store, int index) {
    if (index < 0 || index >= MAX_RECORDS) {
        return NULL;
    }
    return &store->records[index];
}

void search_records(RecordStore *store, const char *query) {
    printf("Search results for '%s':\n", query);
    for (int i = 0; i < store->count; i++) {
        if (strstr(store->records[i].name, query)) {
            printf("  [%d] ID:%d %s (%.2f)\n", i, store->records[i].id, 
                   store->records[i].name, store->records[i].score);
        }
    }
}

int main() {
    RecordStore store;
    init_store(&store);
    
    printf("Record Management System\n");
    printf("=======================\n\n");
    
    add_record(&store, 1, "Alice Johnson", 95.5);
    add_record(&store, 2, "Bob Smith", 87.3);
    add_record(&store, 3, "Carol White", 92.1);
    
    while (1) {
        printf("Commands:\n");
        printf("  1 - Add record\n");
        printf("  2 - View record\n");
        printf("  3 - Search records\n");
        printf("  4 - List all\n");
        printf("  5 - Exit\n");
        printf("Choice: ");
        
        int choice;
        if (scanf("%d", &choice) != 1) break;
        getchar();
        
        if (choice == 1) {
            printf("ID: ");
            int id;
            scanf("%d", &id);
            getchar();
            
            printf("Name: ");
            char name[128];
            if (!fgets(name, sizeof(name), stdin)) continue;
            name[strcspn(name, "\n")] = 0;
            
            printf("Score: ");
            float score;
            scanf("%f", &score);
            
            if (add_record(&store, id, name, score) == 0) {
                printf("Record added\n");
            }
        } else if (choice == 2) {
            printf("Index: ");
            int idx;
            scanf("%d", &idx);
            
            Record *r = get_record(&store, idx);
            if (r && r->id != 0) {
                printf("ID: %d\n", r->id);
                printf("Name: %s\n", r->name);
                printf("Score: %.2f\n", r->score);
            } else {
                printf("Record not found\n");
            }
        } else if (choice == 3) {
            printf("Query: ");
            char query[64];
            if (!fgets(query, sizeof(query), stdin)) continue;
            query[strcspn(query, "\n")] = 0;
            search_records(&store, query);
        } else if (choice == 4) {
            for (int i = 0; i < store.count; i++) {
                printf("[%d] %s\n", i, store.records[i].name);
            }
        } else if (choice == 5) {
            break;
        }
        printf("\n");
    }
    
    return 0;
}
