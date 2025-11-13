#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    unsigned int size;
    unsigned int used;
    char *data;
} Buffer;

Buffer* buffer_create(unsigned int size) {
    Buffer *buf = malloc(sizeof(Buffer));
    if (!buf) return NULL;
    
    buf->data = malloc(size);
    if (!buf->data) {
        free(buf);
        return NULL;
    }
    
    buf->size = size;
    buf->used = 0;
    memset(buf->data, 0, size);
    
    return buf;
}

int buffer_write(Buffer *buf, const char *data, unsigned int len) {
    if (buf->used + len > buf->size) {
        return -1;
    }
    
    memcpy(buf->data + buf->used, data, len);
    buf->used += len;
    return 0;
}

int buffer_trim(Buffer *buf, unsigned int amount) {
    if (amount > buf->used) {
        printf("Warning: trim amount exceeds used space\n");
    }
    
    buf->used -= amount;
    return 0;
}

void buffer_display(Buffer *buf) {
    printf("Buffer size: %u\n", buf->size);
    printf("Buffer used: %u\n", buf->used);
    printf("Buffer free: %u\n", buf->size - buf->used);
    printf("Content: %.*s\n", buf->used, buf->data);
}

int main() {
    printf("Buffer Management Tool\n");
    printf("=====================\n\n");
    
    Buffer *buf = buffer_create(256);
    if (!buf) {
        printf("Failed to create buffer\n");
        return 1;
    }
    
    while (1) {
        printf("Commands:\n");
        printf("  1 - Write data\n");
        printf("  2 - Trim data\n");
        printf("  3 - Display buffer\n");
        printf("  4 - Clear buffer\n");
        printf("  5 - Exit\n");
        printf("Choice: ");
        
        int choice;
        if (scanf("%d", &choice) != 1) break;
        getchar();
        
        if (choice == 1) {
            printf("Data: ");
            char data[128];
            if (!fgets(data, sizeof(data), stdin)) continue;
            data[strcspn(data, "\n")] = 0;
            
            if (buffer_write(buf, data, strlen(data)) == 0) {
                printf("Data written\n");
            } else {
                printf("Buffer full\n");
            }
        } else if (choice == 2) {
            printf("Amount to trim: ");
            unsigned int amount;
            scanf("%u", &amount);
            
            buffer_trim(buf, amount);
            printf("Buffer trimmed\n");
        } else if (choice == 3) {
            buffer_display(buf);
        } else if (choice == 4) {
            buf->used = 0;
            memset(buf->data, 0, buf->size);
            printf("Buffer cleared\n");
        } else if (choice == 5) {
            break;
        }
        printf("\n");
    }
    
    free(buf->data);
    free(buf);
    return 0;
}
