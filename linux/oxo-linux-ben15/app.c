#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VARS 20

typedef struct {
    char name[32];
    char value[64];
} Variable;

typedef struct {
    Variable vars[MAX_VARS];
    int count;
    void (*error_handler)(const char*);
} Environment;

void default_error_handler(const char *msg) {
    fprintf(stderr, "Error: %s\n", msg);
}

void init_environment(Environment *env) {
    env->count = 0;
    env->error_handler = default_error_handler;
    memset(env->vars, 0, sizeof(env->vars));
}

int set_variable(Environment *env, const char *name, const char *value) {
    if (env->count >= MAX_VARS) {
        env->error_handler("Too many variables");
        return -1;
    }
    
    for (int i = 0; i < env->count; i++) {
        if (strcmp(env->vars[i].name, name) == 0) {
            strcpy(env->vars[i].value, value);
            return 0;
        }
    }
    
    strcpy(env->vars[env->count].name, name);
    strcpy(env->vars[env->count].value, value);
    env->count++;
    return 0;
}

const char* get_variable(Environment *env, const char *name) {
    for (int i = 0; i < env->count; i++) {
        if (strcmp(env->vars[i].name, name) == 0) {
            return env->vars[i].value;
        }
    }
    return NULL;
}

void expand_variables(Environment *env, const char *input, char *output) {
    char temp[256];
    strcpy(temp, input);
    
    strcpy(output, "");
    char *ptr = temp;
    
    while (*ptr) {
        if (*ptr == '$') {
            ptr++;
            char varname[64];
            int i = 0;
            while (*ptr && (*ptr == '_' || (*ptr >= 'a' && *ptr <= 'z') || 
                          (*ptr >= 'A' && *ptr <= 'Z') || (*ptr >= '0' && *ptr <= '9'))) {
                varname[i++] = *ptr++;
            }
            varname[i] = '\0';
            
            const char *value = get_variable(env, varname);
            if (value) {
                strcat(output, value);
            }
        } else {
            char c[2] = {*ptr, '\0'};
            strcat(output, c);
            ptr++;
        }
    }
}

int main() {
    Environment env;
    init_environment(&env);
    
    printf("Variable Expansion Shell\n");
    printf("=======================\n\n");
    
    set_variable(&env, "HOME", "/home/user");
    set_variable(&env, "PATH", "/usr/bin:/bin");
    
    while (1) {
        printf("> ");
        char input[256];
        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "exit") == 0) {
            break;
        } else if (strncmp(input, "set ", 4) == 0) {
            char *eq = strchr(input + 4, '=');
            if (eq) {
                *eq = '\0';
                set_variable(&env, input + 4, eq + 1);
            }
        } else if (strcmp(input, "list") == 0) {
            for (int i = 0; i < env.count; i++) {
                printf("%s=%s\n", env.vars[i].name, env.vars[i].value);
            }
        } else {
            char output[512];
            expand_variables(&env, input, output);
            printf("%s\n", output);
        }
    }
    
    return 0;
}
