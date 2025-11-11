/*
 * Level 2: Medium Stack Buffer Overflow
 * 
 * Description:
 * Stack overflow with stack canaries enabled. Requires leaking the canary
 * value through a format string vulnerability before exploiting the overflow.
 * 
 * Vulnerability: Stack buffer overflow + format string leak
 * Exploitability: MEDIUM
 * 
 * Protections:
 * - Stack canaries: YES (need to bypass)
 * - NX/DEP: NO (stack executable)
 * - PIE/ASLR: NO
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void admin_shell() {
    printf("\n*** ADMIN ACCESS! Canary bypassed! ***\n");
    system("/bin/sh");
}

void normal_user() {
    printf("Normal user access\n");
}

void format_leak(char *input) {
    printf("Debug: ");
    printf(input);
    printf("\n");
}

void buffer_overflow(char *input) {
    char buffer[64];
    
    printf("\nBuffer at: %p\n", buffer);
    printf("admin_shell at: %p\n", admin_shell);
    
    strcpy(buffer, input);
    
    printf("Data copied\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <mode> [data]\n", argv[0]);
        printf("Modes: leak <fmt> | overflow <data>\n");
        return 1;
    }
    
    printf("=== Stack Buffer Overflow - Medium ===\n\n");
    
    if (strcmp(argv[1], "leak") == 0 && argc == 3) {
        format_leak(argv[2]);
    } else if (strcmp(argv[1], "overflow") == 0 && argc == 3) {
        buffer_overflow(argv[2]);
    } else {
        printf("Invalid arguments\n");
    }
    
    return 0;
}
