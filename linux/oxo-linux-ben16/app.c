#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { TYPE_INT, TYPE_STRING, TYPE_FLOAT } DataType;

typedef struct {
    DataType type;
    union {
        int int_val;
        char *str_val;
        float float_val;
    } data;
} Value;

typedef struct {
    char key[32];
    Value value;
} ConfigItem;

typedef struct {
    ConfigItem items[20];
    int count;
} Config;

void init_config(Config *cfg) {
    cfg->count = 0;
    memset(cfg->items, 0, sizeof(cfg->items));
}

void set_int_value(Config *cfg, const char *key, int val) {
    ConfigItem *item = &cfg->items[cfg->count++];
    strcpy(item->key, key);
    item->value.type = TYPE_INT;
    item->value.data.int_val = val;
}

void set_string_value(Config *cfg, const char *key, const char *val) {
    ConfigItem *item = &cfg->items[cfg->count++];
    strcpy(item->key, key);
    item->value.type = TYPE_STRING;
    item->value.data.str_val = malloc(strlen(val) + 1);
    strcpy(item->value.data.str_val, val);
}

void set_float_value(Config *cfg, const char *key, float val) {
    ConfigItem *item = &cfg->items[cfg->count++];
    strcpy(item->key, key);
    item->value.type = TYPE_FLOAT;
    item->value.data.float_val = val;
}

Value* get_value(Config *cfg, const char *key) {
    for (int i = 0; i < cfg->count; i++) {
        if (strcmp(cfg->items[i].key, key) == 0) {
            return &cfg->items[i].value;
        }
    }
    return NULL;
}

void print_value(Value *val) {
    switch (val->type) {
        case TYPE_INT:
            printf("%d\n", val->data.int_val);
            break;
        case TYPE_STRING:
            printf("%s\n", val->data.str_val);
            break;
        case TYPE_FLOAT:
            printf("%.2f\n", val->data.float_val);
            break;
    }
}

int main() {
    Config cfg;
    init_config(&cfg);
    
    printf("Configuration Manager\n");
    printf("====================\n\n");
    
    while (1) {
        printf("Commands:\n");
        printf("  1 - Set integer\n");
        printf("  2 - Set string\n");
        printf("  3 - Set float\n");
        printf("  4 - Get value\n");
        printf("  5 - List all\n");
        printf("  6 - Exit\n");
        printf("Choice: ");
        
        int choice;
        if (scanf("%d", &choice) != 1) break;
        getchar();
        
        if (choice == 1) {
            printf("Key: ");
            char key[64];
            if (!fgets(key, sizeof(key), stdin)) continue;
            key[strcspn(key, "\n")] = 0;
            
            printf("Value: ");
            int val;
            scanf("%d", &val);
            
            set_int_value(&cfg, key, val);
        } else if (choice == 2) {
            printf("Key: ");
            char key[64];
            if (!fgets(key, sizeof(key), stdin)) continue;
            key[strcspn(key, "\n")] = 0;
            
            printf("Value: ");
            char val[128];
            if (!fgets(val, sizeof(val), stdin)) continue;
            val[strcspn(val, "\n")] = 0;
            
            set_string_value(&cfg, key, val);
        } else if (choice == 3) {
            printf("Key: ");
            char key[64];
            if (!fgets(key, sizeof(key), stdin)) continue;
            key[strcspn(key, "\n")] = 0;
            
            printf("Value: ");
            float val;
            scanf("%f", &val);
            
            set_float_value(&cfg, key, val);
        } else if (choice == 4) {
            printf("Key: ");
            char key[64];
            if (!fgets(key, sizeof(key), stdin)) continue;
            key[strcspn(key, "\n")] = 0;
            
            Value *val = get_value(&cfg, key);
            if (val) {
                printf("Type: %d, Value: ", val->type);
                print_value(val);
            } else {
                printf("Key not found\n");
            }
        } else if (choice == 5) {
            for (int i = 0; i < cfg.count; i++) {
                printf("%s = ", cfg.items[i].key);
                print_value(&cfg.items[i].value);
            }
        } else if (choice == 6) {
            break;
        }
        printf("\n");
    }
    
    return 0;
}
