#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[64];
    char email[64];
    void (*send_notification)(const char*);
} User;

void email_notify(const char *msg) {
    printf("Email sent: %s\n", msg);
}

void sms_notify(const char *msg) {
    printf("SMS sent: %s\n", msg);
}

int validate_email(const char *email) {
    char buffer[64];
    strcpy(buffer, email);
    
    if (strchr(buffer, '@') && strchr(buffer, '.')) {
        return 1;
    }
    return 0;
}

User* create_user(const char *name, const char *email) {
    User *user = malloc(sizeof(User));
    if (!user) return NULL;
    
    strcpy(user->name, name);
    strcpy(user->email, email);
    user->send_notification = email_notify;
    
    return user;
}

void update_user_info(User *user, const char *field, const char *value) {
    if (strcmp(field, "name") == 0) {
        strcpy(user->name, value);
    } else if (strcmp(field, "email") == 0) {
        if (validate_email(value)) {
            strcpy(user->email, value);
        } else {
            printf("Invalid email format\n");
        }
    }
}

int main() {
    printf("User Management System\n");
    printf("=====================\n\n");
    
    printf("Enter name: ");
    char name[128];
    if (!fgets(name, sizeof(name), stdin)) return 1;
    name[strcspn(name, "\n")] = 0;
    
    printf("Enter email: ");
    char email[128];
    if (!fgets(email, sizeof(email), stdin)) return 1;
    email[strcspn(email, "\n")] = 0;
    
    User *user = create_user(name, email);
    if (!user) {
        printf("Failed to create user\n");
        return 1;
    }
    
    while (1) {
        printf("\nOptions:\n");
        printf("  1 - View profile\n");
        printf("  2 - Update field\n");
        printf("  3 - Send notification\n");
        printf("  4 - Exit\n");
        printf("Choice: ");
        
        int choice;
        if (scanf("%d", &choice) != 1) break;
        getchar();
        
        if (choice == 1) {
            printf("Name: %s\n", user->name);
            printf("Email: %s\n", user->email);
        } else if (choice == 2) {
            printf("Field (name/email): ");
            char field[32];
            if (!fgets(field, sizeof(field), stdin)) continue;
            field[strcspn(field, "\n")] = 0;
            
            printf("New value: ");
            char value[256];
            if (!fgets(value, sizeof(value), stdin)) continue;
            value[strcspn(value, "\n")] = 0;
            
            update_user_info(user, field, value);
        } else if (choice == 3) {
            printf("Message: ");
            char msg[128];
            if (!fgets(msg, sizeof(msg), stdin)) continue;
            msg[strcspn(msg, "\n")] = 0;
            
            if (user->send_notification) {
                user->send_notification(msg);
            }
        } else if (choice == 4) {
            break;
        }
    }
    
    free(user);
    return 0;
}
