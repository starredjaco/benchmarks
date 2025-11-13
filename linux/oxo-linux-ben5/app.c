#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *username;
    char *email;
    void (*notify)(const char*);
} UserProfile;

void send_email_notification(const char *msg) {
    printf("Email notification: %s\n", msg);
}

void send_sms_notification(const char *msg) {
    printf("SMS notification: %s\n", msg);
}

UserProfile* create_profile(const char *username, const char *email) {
    UserProfile *profile = malloc(sizeof(UserProfile));
    if (!profile) return NULL;
    
    profile->username = malloc(64);
    profile->email = malloc(64);
    
    if (!profile->username || !profile->email) {
        free(profile->username);
        free(profile->email);
        free(profile);
        return NULL;
    }
    
    strcpy(profile->username, username);
    strcpy(profile->email, email);
    profile->notify = send_email_notification;
    
    return profile;
}

void update_profile(UserProfile *profile, const char *field, const char *value) {
    if (strcmp(field, "username") == 0) {
        strcpy(profile->username, value);
    } else if (strcmp(field, "email") == 0) {
        strcpy(profile->email, value);
    }
}

void trigger_notification(UserProfile *profile, const char *message) {
    if (profile->notify) {
        profile->notify(message);
    }
}

int main() {
    printf("User Profile Manager\n");
    printf("===================\n\n");
    
    printf("Enter username: ");
    char username[128];
    if (!fgets(username, sizeof(username), stdin)) return 1;
    username[strcspn(username, "\n")] = 0;
    
    printf("Enter email: ");
    char email[128];
    if (!fgets(email, sizeof(email), stdin)) return 1;
    email[strcspn(email, "\n")] = 0;
    
    UserProfile *profile = create_profile(username, email);
    if (!profile) {
        printf("Failed to create profile\n");
        return 1;
    }
    
    printf("Profile created successfully\n\n");
    
    while (1) {
        printf("Options:\n");
        printf("  1 - View profile\n");
        printf("  2 - Update field\n");
        printf("  3 - Send notification\n");
        printf("  4 - Exit\n");
        printf("Choice: ");
        
        int choice;
        if (scanf("%d", &choice) != 1) break;
        getchar();
        
        if (choice == 1) {
            printf("Username: %s\n", profile->username);
            printf("Email: %s\n", profile->email);
        } else if (choice == 2) {
            printf("Field (username/email): ");
            char field[32];
            if (!fgets(field, sizeof(field), stdin)) continue;
            field[strcspn(field, "\n")] = 0;
            
            printf("New value: ");
            char value[256];
            if (!fgets(value, sizeof(value), stdin)) continue;
            value[strcspn(value, "\n")] = 0;
            
            update_profile(profile, field, value);
            printf("Profile updated\n");
        } else if (choice == 3) {
            printf("Message: ");
            char msg[128];
            if (!fgets(msg, sizeof(msg), stdin)) continue;
            msg[strcspn(msg, "\n")] = 0;
            
            trigger_notification(profile, msg);
        } else if (choice == 4) {
            break;
        }
        printf("\n");
    }
    
    free(profile->username);
    free(profile->email);
    free(profile);
    
    return 0;
}
