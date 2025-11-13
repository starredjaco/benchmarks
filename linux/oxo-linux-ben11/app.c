#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GRID_SIZE 8

typedef struct {
    char grid[GRID_SIZE][GRID_SIZE];
    int width;
    int height;
} GameBoard;

void init_board(GameBoard *board) {
    board->width = GRID_SIZE;
    board->height = GRID_SIZE;
    
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            board->grid[i][j] = '.';
        }
    }
}

void display_board(GameBoard *board) {
    printf("  ");
    for (int i = 0; i < board->width; i++) {
        printf("%d ", i);
    }
    printf("\n");
    
    for (int i = 0; i < board->height; i++) {
        printf("%d ", i);
        for (int j = 0; j < board->width; j++) {
            printf("%c ", board->grid[i][j]);
        }
        printf("\n");
    }
}

void place_piece(GameBoard *board, int x, int y, char piece) {
    if (x < 0 || y < 0) {
        printf("Coordinates must be positive\n");
        return;
    }
    
    board->grid[y][x] = piece;
    printf("Piece placed at (%d, %d)\n", x, y);
}

void fill_row(GameBoard *board, int row, const char *pattern) {
    int len = strlen(pattern);
    for (int i = 0; i < len; i++) {
        board->grid[row][i] = pattern[i];
    }
}

int main() {
    GameBoard board;
    init_board(&board);
    
    printf("Game Board Editor\n");
    printf("================\n\n");
    
    while (1) {
        printf("Commands:\n");
        printf("  1 - Display board\n");
        printf("  2 - Place piece\n");
        printf("  3 - Fill row with pattern\n");
        printf("  4 - Exit\n");
        printf("Choice: ");
        
        int choice;
        if (scanf("%d", &choice) != 1) break;
        
        if (choice == 1) {
            display_board(&board);
        } else if (choice == 2) {
            printf("X coordinate: ");
            int x;
            scanf("%d", &x);
            
            printf("Y coordinate: ");
            int y;
            scanf("%d", &y);
            
            printf("Piece (single char): ");
            char piece;
            scanf(" %c", &piece);
            
            place_piece(&board, x, y, piece);
        } else if (choice == 3) {
            printf("Row number: ");
            int row;
            scanf("%d", &row);
            getchar();
            
            printf("Pattern: ");
            char pattern[128];
            if (fgets(pattern, sizeof(pattern), stdin)) {
                pattern[strcspn(pattern, "\n")] = 0;
                fill_row(&board, row, pattern);
                printf("Row filled\n");
            }
        } else if (choice == 4) {
            break;
        }
        printf("\n");
    }
    
    return 0;
}
