#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char command[32];
    char args[96];
    void (*execute)(const char*);
} CommandEntry;

void exec_ls(const char *args) {
    char cmd[128];
    sprintf(cmd, "ls %s", args);
    system(cmd);
}

void exec_cat(const char *args) {
    char cmd[128];
    sprintf(cmd, "cat %s", args);
    system(cmd);
}

void exec_echo(const char *args) {
    printf("%s\n", args);
}

int parse_command_line(const char *input, CommandEntry *entry) {
    char buffer[128];
    strcpy(buffer, input);
    
    char *space = strchr(buffer, ' ');
    if (space) {
        *space = '\0';
        strcpy(entry->command, buffer);
        strcpy(entry->args, space + 1);
    } else {
        strcpy(entry->command, buffer);
        entry->args[0] = '\0';
    }
    
    if (strcmp(entry->command, "ls") == 0) {
        entry->execute = exec_ls;
    } else if (strcmp(entry->command, "cat") == 0) {
        entry->execute = exec_cat;
    } else if (strcmp(entry->command, "echo") == 0) {
        entry->execute = exec_echo;
    } else {
        return -1;
    }
    
    return 0;
}

void execute_command(CommandEntry *entry) {
    if (entry->execute) {
        entry->execute(entry->args);
    }
}

int main() {
    printf("Simple Shell\n");
    printf("===========\n\n");
    printf("Available commands: ls, cat, echo, exit\n\n");
    
    while (1) {
        printf("$ ");
        char input[256];
        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "exit") == 0) {
            break;
        }
        
        CommandEntry entry;
        memset(&entry, 0, sizeof(entry));
        
        if (parse_command_line(input, &entry) == 0) {
            execute_command(&entry);
        } else {
            printf("Unknown command\n");
        }
    }
    
    return 0;
}
