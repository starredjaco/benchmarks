#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *data;
    size_t size;
    void (*cleanup)(void*);
} Resource;

typedef struct {
    Resource *resources[10];
    int count;
} ResourceManager;

void default_cleanup(void* ptr) {
    printf("Cleaning up resource\n");
    free(ptr);
}

Resource* create_resource(const char* initial_data) {
    Resource *res = malloc(sizeof(Resource));
    if (!res) return NULL;
    
    res->size = strlen(initial_data) + 1;
    res->data = malloc(res->size);
    if (!res->data) {
        free(res);
        return NULL;
    }
    
    strcpy(res->data, initial_data);
    res->cleanup = default_cleanup;
    return res;
}

void release_resource(Resource *res) {
    if (res) {
        if (res->cleanup && res->data) {
            res->cleanup(res->data);
        }
        free(res);
    }
}

int add_resource(ResourceManager *mgr, Resource *res) {
    if (mgr->count >= 10) {
        return -1;
    }
    mgr->resources[mgr->count++] = res;
    return 0;
}

void process_cleanup_request(ResourceManager *mgr, int index) {
    if (index < 0 || index >= mgr->count) {
        printf("Invalid resource index\n");
        return;
    }
    
    Resource *res = mgr->resources[index];
    release_resource(res);
    printf("Resource %d released\n", index);
}

int main() {
    ResourceManager mgr = {0};
    
    printf("Resource Management System\n");
    printf("==========================\n\n");
    
    Resource *res1 = create_resource("Configuration data");
    Resource *res2 = create_resource("User preferences");
    Resource *res3 = create_resource("Cache data");
    
    add_resource(&mgr, res1);
    add_resource(&mgr, res2);
    add_resource(&mgr, res3);
    
    printf("Created %d resources\n\n", mgr.count);
    
    while (1) {
        printf("Commands:\n");
        printf("  1 - List resources\n");
        printf("  2 - Release resource\n");
        printf("  3 - Use resource\n");
        printf("  4 - Exit\n");
        printf("Choice: ");
        
        int choice;
        if (scanf("%d", &choice) != 1) {
            break;
        }
        
        if (choice == 1) {
            for (int i = 0; i < mgr.count; i++) {
                if (mgr.resources[i]) {
                    printf("  [%d] %s\n", i, mgr.resources[i]->data);
                }
            }
        } else if (choice == 2) {
            printf("Resource index: ");
            int idx;
            scanf("%d", &idx);
            process_cleanup_request(&mgr, idx);
        } else if (choice == 3) {
            printf("Resource index: ");
            int idx;
            scanf("%d", &idx);
            
            if (idx >= 0 && idx < mgr.count && mgr.resources[idx]) {
                printf("Resource data: %s\n", mgr.resources[idx]->data);
                if (mgr.resources[idx]->cleanup) {
                    printf("Cleanup handler: available\n");
                }
            }
        } else if (choice == 4) {
            break;
        }
        printf("\n");
    }
    
    printf("\nCleaning up remaining resources...\n");
    for (int i = 0; i < mgr.count; i++) {
        if (mgr.resources[i]) {
            release_resource(mgr.resources[i]);
        }
    }
    
    return 0;
}
