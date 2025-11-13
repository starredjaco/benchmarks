#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *buffer;
    unsigned int size;
    unsigned int used;
} DynamicBuffer;

DynamicBuffer* buffer_create(unsigned int initial_size) {
    DynamicBuffer *buf = malloc(sizeof(DynamicBuffer));
    if (!buf) return NULL;
    
    buf->buffer = malloc(initial_size);
    if (!buf->buffer) {
        free(buf);
        return NULL;
    }
    
    buf->size = initial_size;
    buf->used = 0;
    return buf;
}

int buffer_append(DynamicBuffer *buf, const char *data, unsigned int length) {
    unsigned int new_used = buf->used + length;
    
    if (new_used > buf->size) {
        unsigned int new_size = buf->size * 2 + length;
        char *new_buffer = realloc(buf->buffer, new_size);
        if (!new_buffer) {
            return -1;
        }
        
        buf->buffer = new_buffer;
        buf->size = new_size;
    }
    
    memcpy(buf->buffer + buf->used, data, length);
    buf->used = new_used;
    return 0;
}

void buffer_print(DynamicBuffer *buf) {
    printf("Buffer contents (%u/%u bytes):\n", buf->used, buf->size);
    for (unsigned int i = 0; i < buf->used && i < 100; i++) {
        putchar(buf->buffer[i]);
    }
    if (buf->used > 100) {
        printf("... (%u more bytes)", buf->used - 100);
    }
    printf("\n");
}

int main() {
    printf("Dynamic Buffer Manager\n");
    printf("=====================\n\n");
    
    DynamicBuffer *buf = buffer_create(64);
    if (!buf) {
        printf("Failed to create buffer\n");
        return 1;
    }
    
    while (1) {
        printf("Commands:\n");
        printf("  1 - Append data\n");
        printf("  2 - View buffer\n");
        printf("  3 - Buffer info\n");
        printf("  4 - Exit\n");
        printf("Choice: ");
        
        int choice;
        if (scanf("%d", &choice) != 1) break;
        getchar();
        
        if (choice == 1) {
            printf("Data length: ");
            unsigned int len;
            scanf("%u", &len);
            getchar();
            
            printf("Data: ");
            char *data = malloc(len + 1);
            if (!data) continue;
            
            if (fgets(data, len + 1, stdin)) {
                if (buffer_append(buf, data, len) == 0) {
                    printf("Data appended\n");
                } else {
                    printf("Append failed\n");
                }
            }
            free(data);
        } else if (choice == 2) {
            buffer_print(buf);
        } else if (choice == 3) {
            printf("Size: %u bytes\n", buf->size);
            printf("Used: %u bytes\n", buf->used);
            printf("Free: %u bytes\n", buf->size - buf->used);
        } else if (choice == 4) {
            break;
        }
        printf("\n");
    }
    
    free(buf->buffer);
    free(buf);
    return 0;
}
