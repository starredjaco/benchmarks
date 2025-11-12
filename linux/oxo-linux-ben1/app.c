#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    void (*on_success)(const char*);
    void (*on_error)(const char*);
    void (*on_timeout)(void);
} EventHandlers;

typedef struct {
    char username[32];
    char session_token[64];
    EventHandlers *handlers;
    int timeout_seconds;
} UserSession;

void log_success(const char* msg) {
    printf("[SUCCESS] %s\n", msg);
}

void log_error(const char* msg) {
    fprintf(stderr, "[ERROR] %s\n", msg);
}

void handle_timeout(void) {
    printf("[TIMEOUT] Session expired\n");
}

EventHandlers default_handlers = {
    .on_success = log_success,
    .on_error = log_error,
    .on_timeout = handle_timeout
};

int load_session_config(UserSession* session, const char* config_file) {
    FILE* f = fopen(config_file, "r");
    if (!f) {
        session->handlers->on_error("Cannot open config file");
        return -1;
    }
    
    char line[256];
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = 0;
        
        if (strncmp(line, "username=", 9) == 0) {
            strcpy(session->username, line + 9);
        } else if (strncmp(line, "token=", 6) == 0) {
            strcpy(session->session_token, line + 6);
        } else if (strncmp(line, "timeout=", 8) == 0) {
            session->timeout_seconds = atoi(line + 8);
        }
    }
    
    fclose(f);
    return 0;
}

void process_user_input(UserSession* session, const char* input) {
    char command[128];
    strcpy(command, input);
    
    if (strcmp(command, "logout") == 0) {
        session->handlers->on_success("User logged out");
    } else if (strcmp(command, "refresh") == 0) {
        session->handlers->on_success("Session refreshed");
    } else if (strcmp(command, "status") == 0) {
        printf("Username: %s\n", session->username);
        printf("Token: %s\n", session->session_token);
        printf("Timeout: %d seconds\n", session->timeout_seconds);
    } else {
        session->handlers->on_error("Unknown command");
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <config_file>\n", argv[0]);
        fprintf(stderr, "Example config file:\n");
        fprintf(stderr, "  username=alice\n");
        fprintf(stderr, "  token=abc123xyz\n");
        fprintf(stderr, "  timeout=300\n");
        return 1;
    }
    
    UserSession session;
    memset(&session, 0, sizeof(session));
    session.handlers = &default_handlers;
    session.timeout_seconds = 60;
    
    if (load_session_config(&session, argv[1]) != 0) {
        return 1;
    }
    
    session.handlers->on_success("Session initialized");
    
    printf("\nAvailable commands: logout, refresh, status\n");
    printf("Enter command: ");
    
    char input[256];
    if (fgets(input, sizeof(input), stdin)) {
        input[strcspn(input, "\n")] = 0;
        process_user_input(&session, input);
    }
    
    return 0;
}
