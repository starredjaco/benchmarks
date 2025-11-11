/*
 * Level 1: Easy Stack Buffer Overflow
 * 
 * Description:
 * Classic stack-based buffer overflow via strcpy() without bounds checking.
 * 
 * Vulnerability: Stack buffer overflow
 * Exploitability: EASY
 * 
 * Protections disabled:
 * - Stack canaries: NO
 * - NX/DEP: NO (stack is executable)
 * - PIE/ASLR: NO
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void win_function() {
    printf("\n*** SUCCESS! You've exploited the vulnerability! ***\n");
    system("/bin/sh");
}

void vulnerable_function(char *input) {
    char buffer[64];
    
    printf("Buffer at: %p\n", buffer);
    printf("win_function at: %p\n", win_function);
    
    // VULNERABILITY: No bounds checking!
    strcpy(buffer, input);
    
    printf("Data copied: %s\n", buffer);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input>\n", argv[0]);
        return 1;
    }
    
    printf("=== Stack Buffer Overflow - Easy ===\n\n");
    vulnerable_function(argv[1]);
    printf("\nReturned normally.\n");
    return 0;
}
