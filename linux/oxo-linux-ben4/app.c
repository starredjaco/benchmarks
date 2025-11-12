#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ChunkHeader {
    size_t size;
    struct ChunkHeader *next;
    char data[0];
} ChunkHeader;

typedef struct {
    ChunkHeader *free_list;
    size_t total_allocated;
} MemoryPool;

void pool_init(MemoryPool *pool) {
    pool->free_list = NULL;
    pool->total_allocated = 0;
}

void* pool_allocate(MemoryPool *pool, size_t size) {
    ChunkHeader *chunk = malloc(sizeof(ChunkHeader) + size);
    if (!chunk) return NULL;
    
    chunk->size = size;
    chunk->next = NULL;
    pool->total_allocated += size;
    
    return chunk->data;
}

void pool_deallocate(MemoryPool *pool, void *ptr) {
    if (!ptr) return;
    
    ChunkHeader *chunk = (ChunkHeader*)((char*)ptr - sizeof(ChunkHeader));
    pool->total_allocated -= chunk->size;
    
    chunk->next = pool->free_list;
    pool->free_list = chunk;
}

void pool_stats(MemoryPool *pool) {
    printf("Pool Statistics:\n");
    printf("  Total allocated: %zu bytes\n", pool->total_allocated);
    
    int free_count = 0;
    ChunkHeader *current = pool->free_list;
    while (current) {
        free_count++;
        current = current->next;
    }
    printf("  Free chunks: %d\n", free_count);
}

int main() {
    MemoryPool pool;
    pool_init(&pool);
    
    void *buffers[10] = {0};
    int buffer_count = 0;
    
    printf("Memory Pool Manager\n");
    printf("==================\n\n");
    
    while (1) {
        printf("Commands:\n");
        printf("  1 - Allocate buffer\n");
        printf("  2 - Free buffer\n");
        printf("  3 - Write to buffer\n");
        printf("  4 - Pool stats\n");
        printf("  5 - Exit\n");
        printf("Choice: ");
        
        int choice;
        if (scanf("%d", &choice) != 1) break;
        
        if (choice == 1) {
            if (buffer_count >= 10) {
                printf("Maximum buffers reached\n");
                continue;
            }
            printf("Size: ");
            size_t size;
            scanf("%zu", &size);
            
            buffers[buffer_count] = pool_allocate(&pool, size);
            printf("Allocated buffer %d (%zu bytes)\n", buffer_count, size);
            buffer_count++;
        } else if (choice == 2) {
            printf("Buffer index: ");
            int idx;
            scanf("%d", &idx);
            
            if (idx >= 0 && idx < buffer_count && buffers[idx]) {
                pool_deallocate(&pool, buffers[idx]);
                buffers[idx] = NULL;
                printf("Buffer %d freed\n", idx);
            }
        } else if (choice == 3) {
            printf("Buffer index: ");
            int idx;
            scanf("%d", &idx);
            
            if (idx >= 0 && idx < buffer_count && buffers[idx]) {
                printf("Data: ");
                getchar();
                char data[256];
                if (fgets(data, sizeof(data), stdin)) {
                    data[strcspn(data, "\n")] = 0;
                    strcpy(buffers[idx], data);
                    printf("Data written\n");
                }
            }
        } else if (choice == 4) {
            pool_stats(&pool);
        } else if (choice == 5) {
            break;
        }
        printf("\n");
    }
    
    return 0;
}
