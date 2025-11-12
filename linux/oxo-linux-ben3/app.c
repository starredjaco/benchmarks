#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char username[64];
    char log_format[128];
    int log_level;
    FILE *log_file;
} Logger;

void init_logger(Logger *logger, const char *username) {
    strcpy(logger->username, username);
    strcpy(logger->log_format, "[%s] %s: ");
    logger->log_level = 1;
    logger->log_file = stderr;
}

void log_message(Logger *logger, const char *message) {
    char timestamp[32];
    time_t now = time(NULL);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    fprintf(logger->log_file, logger->log_format, timestamp, logger->username);
    fprintf(logger->log_file, message);
    fprintf(logger->log_file, "\n");
}

void set_log_format(Logger *logger, const char *format) {
    strcpy(logger->log_format, format);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <username>\n", argv[0]);
        return 1;
    }
    
    Logger logger;
    init_logger(&logger, argv[1]);
    
    printf("Logging System\n");
    printf("=============\n\n");
    
    while (1) {
        printf("Options:\n");
        printf("  1 - Log message\n");
        printf("  2 - Change log format\n");
        printf("  3 - View current format\n");
        printf("  4 - Exit\n");
        printf("Choice: ");
        
        int choice;
        if (scanf("%d", &choice) != 1) break;
        getchar();
        
        if (choice == 1) {
            printf("Enter message: ");
            char msg[256];
            if (fgets(msg, sizeof(msg), stdin)) {
                msg[strcspn(msg, "\n")] = 0;
                log_message(&logger, msg);
            }
        } else if (choice == 2) {
            printf("Enter new format: ");
            char fmt[256];
            if (fgets(fmt, sizeof(fmt), stdin)) {
                fmt[strcspn(fmt, "\n")] = 0;
                set_log_format(&logger, fmt);
                printf("Format updated\n");
            }
        } else if (choice == 3) {
            printf("Current format: ");
            printf(logger.log_format);
            printf("\n");
        } else if (choice == 4) {
            break;
        }
        printf("\n");
    }
    
    return 0;
}
