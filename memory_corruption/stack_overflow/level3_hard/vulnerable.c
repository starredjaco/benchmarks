/*
 * Level 3: Hard Stack Buffer Overflow
 * 
 * Description:
 * This program demonstrates a buffer overflow with modern protections enabled.
 * Requires advanced exploitation techniques including:
 * - Stack canary bypass via leak
 * - Return-oriented programming (ROP) due to NX
 * - Defeating ASLR via information disclosure
 * 
 * Vulnerability: Stack-based buffer overflow with multiple mitigations
 * Exploitability: HARD
 * 
 * Why it's hard:
 * - Stack canaries ENABLED
 * - NX ENABLED (non-executable stack - need ROP)
 * - Partial ASLR (PIE disabled but libraries randomized)
 * - Requires information leak to bypass protections
 * - Need to construct ROP chain
 * - Must preserve stack canary
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 128

// Global data for potential leaks
char global_buffer[256];
void *global_ptr = NULL;

// Useful gadgets for ROP
void print_flag() {
    printf("\n*** Flag captured! ***\n");
    printf("*** You successfully exploited a hardened binary! ***\n");
}

void give_shell() {
    printf("*** Spawning shell... ***\n");
    system("/bin/sh");
}

// Information disclosure vulnerability
void leak_info(char *input) {
    char buffer[64];
    void *stack_var = &buffer;
    
    printf("\n=== Information Disclosure ===\n");
    printf("Stack address: %p\n", stack_var);
    printf("print_flag() address: %p\n", print_flag);
    printf("give_shell() address: %p\n", give_shell);
    printf("system() address: %p\n", system);
    printf("Libc printf() address: %p\n", printf);
    
    // Format string vulnerability for additional leaks
    printf("Custom format: ");
    printf(input);
    printf("\n");
    
    // Show some stack contents
    unsigned long *ptr = (unsigned long *)buffer;
    printf("\nStack dump (first 16 values):\n");
    for (int i = 0; i < 16; i++) {
        printf("stack[%d] = 0x%lx\n", i, ptr[i]);
    }
}

// The vulnerable function
void vulnerable_copy(char *src) {
    char buffer[BUFFER_SIZE];
    char *ptr = buffer;
    
    printf("\n=== Vulnerable Function ===\n");
    printf("Buffer at: %p\n", buffer);
    printf("Saved RIP should be at approximately: %p\n", &buffer + BUFFER_SIZE + 16);
    
    // VULNERABILITY: No bounds checking
    strcpy(buffer, src);
    
    printf("Data copied successfully\n");
    printf("Buffer contents: %s\n", buffer);
}

// Helper function with useful gadgets
void gadget_function(unsigned long arg1, unsigned long arg2) {
    printf("Gadget called with: %lx, %lx\n", arg1, arg2);
    // This function contains useful instruction sequences (gadgets)
    // that can be used in a ROP chain
    __asm__ volatile (
        "pop %rdi\n"
        "pop %rsi\n"
        "pop %rdx\n"
        "ret\n"
    );
}

void process_command(char *cmd) {
    if (strcmp(cmd, "leak") == 0) {
        printf("Enter format string for additional leak: ");
        char input[256];
        if (fgets(input, sizeof(input), stdin)) {
            input[strcspn(input, "\n")] = 0;
            leak_info(input);
        }
    } else if (strcmp(cmd, "overflow") == 0) {
        printf("Enter payload: ");
        char payload[512];
        if (fgets(payload, sizeof(payload), stdin)) {
            payload[strcspn(payload, "\n")] = 0;
            vulnerable_copy(payload);
        }
    } else {
        printf("Unknown command\n");
    }
}

int main(int argc, char *argv[]) {
    // Disable buffering for easier interaction
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);
    
    printf("=== Level 3: Hard Buffer Overflow Challenge ===\n");
    printf("This binary has multiple protections enabled:\n");
    printf("  - Stack Canaries: YES\n");
    printf("  - NX (DEP): YES (stack is non-executable)\n");
    printf("  - PIE: NO (partial ASLR)\n");
    printf("  - RELRO: Partial\n\n");
    
    printf("You will need to:\n");
    printf("  1. Leak stack canary value\n");
    printf("  2. Leak addresses to defeat ASLR\n");
    printf("  3. Build a ROP chain (stack is non-executable)\n");
    printf("  4. Preserve the canary in your exploit\n\n");
    
    if (argc < 2) {
        printf("Usage: %s <command>\n", argv[0]);
        printf("Commands:\n");
        printf("  leak     - Leak memory information\n");
        printf("  overflow - Trigger buffer overflow\n");
        return 1;
    }
    
    process_command(argv[1]);
    
    printf("\nProgram exiting normally.\n");
    return 0;
}
