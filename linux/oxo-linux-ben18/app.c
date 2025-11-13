#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char username[64];
    char session_id[32];
    int permissions;
    int is_admin;
} Session;

typedef struct {
    Session *sessions[10];
    int count;
} SessionManager;

void init_manager(SessionManager *mgr) {
    mgr->count = 0;
    for (int i = 0; i < 10; i++) {
        mgr->sessions[i] = NULL;
    }
}

Session* create_session(const char *username) {
    Session *sess = malloc(sizeof(Session));
    if (!sess) return NULL;
    
    strcpy(sess->username, username);
    snprintf(sess->session_id, sizeof(sess->session_id), "SID%d", rand());
    
    return sess;
}

int add_session(SessionManager *mgr, Session *sess) {
    if (mgr->count >= 10) {
        return -1;
    }
    
    mgr->sessions[mgr->count++] = sess;
    return 0;
}

void set_permissions(Session *sess, int perms) {
    sess->permissions = perms;
    sess->is_admin = (perms & 0x1000) != 0;
}

void display_session(Session *sess) {
    if (!sess) {
        printf("No session\n");
        return;
    }
    
    printf("Username: %s\n", sess->username);
    printf("Session ID: %s\n", sess->session_id);
    printf("Permissions: 0x%x\n", sess->permissions);
    printf("Admin: %s\n", sess->is_admin ? "Yes" : "No");
}

int main() {
    SessionManager mgr;
    init_manager(&mgr);
    
    printf("Session Manager\n");
    printf("==============\n\n");
    
    while (1) {
        printf("Commands:\n");
        printf("  1 - Create session\n");
        printf("  2 - Set permissions\n");
        printf("  3 - View session\n");
        printf("  4 - List sessions\n");
        printf("  5 - Exit\n");
        printf("Choice: ");
        
        int choice;
        if (scanf("%d", &choice) != 1) break;
        getchar();
        
        if (choice == 1) {
            printf("Username: ");
            char username[128];
            if (!fgets(username, sizeof(username), stdin)) continue;
            username[strcspn(username, "\n")] = 0;
            
            Session *sess = create_session(username);
            if (sess && add_session(&mgr, sess) == 0) {
                printf("Session created\n");
            }
        } else if (choice == 2) {
            printf("Session index: ");
            int idx;
            scanf("%d", &idx);
            
            printf("Permissions (hex): ");
            int perms;
            scanf("%x", &perms);
            
            if (idx >= 0 && idx < mgr.count && mgr.sessions[idx]) {
                set_permissions(mgr.sessions[idx], perms);
                printf("Permissions set\n");
            }
        } else if (choice == 3) {
            printf("Session index: ");
            int idx;
            scanf("%d", &idx);
            
            if (idx >= 0 && idx < 10) {
                display_session(mgr.sessions[idx]);
            }
        } else if (choice == 4) {
            for (int i = 0; i < mgr.count; i++) {
                if (mgr.sessions[i]) {
                    printf("[%d] %s\n", i, mgr.sessions[i]->username);
                }
            }
        } else if (choice == 5) {
            break;
        }
        printf("\n");
    }
    
    return 0;
}
