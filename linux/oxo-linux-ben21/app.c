#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Plugin {
    char name[32];
    char version[16];
    void (*init)(void);
    void (*cleanup)(void);
    struct Plugin *next;
} Plugin;

typedef struct {
    Plugin *plugins;
    int count;
} PluginManager;

void default_init(void) {
    printf("Plugin initialized\n");
}

void default_cleanup(void) {
    printf("Plugin cleaned up\n");
}

PluginManager* manager_create(void) {
    PluginManager *mgr = malloc(sizeof(PluginManager));
    if (!mgr) return NULL;
    
    mgr->plugins = NULL;
    mgr->count = 0;
    return mgr;
}

Plugin* plugin_load(const char *name, const char *version) {
    Plugin *p = malloc(sizeof(Plugin));
    if (!p) return NULL;
    
    strcpy(p->name, name);
    strcpy(p->version, version);
    p->init = default_init;
    p->cleanup = default_cleanup;
    p->next = NULL;
    
    return p;
}

void plugin_register(PluginManager *mgr, Plugin *plugin) {
    if (!mgr->plugins) {
        mgr->plugins = plugin;
    } else {
        Plugin *last = mgr->plugins;
        while (last->next) {
            last = last->next;
        }
        last->next = plugin;
    }
    mgr->count++;
}

void plugin_unload(PluginManager *mgr, const char *name) {
    Plugin *prev = NULL;
    Plugin *curr = mgr->plugins;
    
    while (curr) {
        if (strcmp(curr->name, name) == 0) {
            if (curr->cleanup) {
                curr->cleanup();
            }
            
            if (prev) {
                prev->next = curr->next;
            } else {
                mgr->plugins = curr->next;
            }
            
            free(curr);
            mgr->count--;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

Plugin* plugin_find(PluginManager *mgr, const char *name) {
    Plugin *curr = mgr->plugins;
    while (curr) {
        if (strcmp(curr->name, name) == 0) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

int main() {
    PluginManager *mgr = manager_create();
    
    printf("Plugin Manager\n");
    printf("=============\n\n");
    
    while (1) {
        printf("Commands:\n");
        printf("  1 - Load plugin\n");
        printf("  2 - Unload plugin\n");
        printf("  3 - Initialize plugin\n");
        printf("  4 - List plugins\n");
        printf("  5 - Exit\n");
        printf("Choice: ");
        
        int choice;
        if (scanf("%d", &choice) != 1) break;
        getchar();
        
        if (choice == 1) {
            printf("Plugin name: ");
            char name[64];
            if (!fgets(name, sizeof(name), stdin)) continue;
            name[strcspn(name, "\n")] = 0;
            
            printf("Version: ");
            char version[32];
            if (!fgets(version, sizeof(version), stdin)) continue;
            version[strcspn(version, "\n")] = 0;
            
            Plugin *p = plugin_load(name, version);
            if (p) {
                plugin_register(mgr, p);
                printf("Plugin loaded\n");
            }
        } else if (choice == 2) {
            printf("Plugin name: ");
            char name[64];
            if (!fgets(name, sizeof(name), stdin)) continue;
            name[strcspn(name, "\n")] = 0;
            
            plugin_unload(mgr, name);
            printf("Plugin unloaded\n");
        } else if (choice == 3) {
            printf("Plugin name: ");
            char name[64];
            if (!fgets(name, sizeof(name), stdin)) continue;
            name[strcspn(name, "\n")] = 0;
            
            Plugin *p = plugin_find(mgr, name);
            if (p && p->init) {
                p->init();
            } else {
                printf("Plugin not found\n");
            }
        } else if (choice == 4) {
            Plugin *curr = mgr->plugins;
            int i = 0;
            while (curr) {
                printf("[%d] %s v%s\n", i++, curr->name, curr->version);
                curr = curr->next;
            }
        } else if (choice == 5) {
            break;
        }
        printf("\n");
    }
    
    return 0;
}
