#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define INITIAL_CAPACITY 2
#define MAX_INPUT_BUFFER 4096 // Buffer for reading user input before exact allocation

// Structure to manage the dynamic text buffer
typedef struct {
    char **lines;   // Dynamic array of char pointers
    int count;      // Number of lines currently used
    int capacity;   // Total slots allocated in the array
} TextBuffer;

// --- Function Prototypes ---
void initBuffer(TextBuffer *buffer);
void insertLine(TextBuffer *buffer, int index, const char *text);
void deleteLine(TextBuffer *buffer, int index);
void printAllLines(const TextBuffer *buffer);
void shrinkToFit(TextBuffer *buffer);
void saveToFile(const TextBuffer *buffer, const char *filename);
void loadFromFile(TextBuffer *buffer, const char *filename);
void freeAll(TextBuffer *buffer);

int main() {
    TextBuffer editor;
    initBuffer(&editor);

    int choice, index;
    char inputBuffer[MAX_INPUT_BUFFER];
    char filename[100];

    printf("=== Minimal Line-Based Text Editor ===\n");

    while (1) {
        printf("\n1. Insert Line\n2. Delete Line\n3. Print All\n4. Shrink Memory\n5. Save to File\n6. Load from File\n7. Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }
        getchar(); // Consume newline left by scanf

        switch (choice) {
            case 1:
                printf("Enter index to insert at (0 to %d): ", editor.count);
                scanf("%d", &index);
                getchar(); // Consume newline
                
                printf("Enter text: ");
                if (fgets(inputBuffer, MAX_INPUT_BUFFER, stdin)) {
                    // Remove trailing newline from fgets
                    inputBuffer[strcspn(inputBuffer, "\n")] = 0;
                    insertLine(&editor, index, inputBuffer);
                }
                break;

            case 2:
                printf("Enter index to delete (0 to %d): ", editor.count - 1);
                scanf("%d", &index);
                deleteLine(&editor, index);
                break;

            case 3:
                printAllLines(&editor);
                break;

            case 4:
                shrinkToFit(&editor);
                break;

            case 5:
                printf("Enter filename to save: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = 0;
                saveToFile(&editor, filename);
                break;

            case 6:
                printf("Enter filename to load: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = 0;
                loadFromFile(&editor, filename);
                break;

            case 7:
                freeAll(&editor);
                printf("Exiting...\n");
                return 0;

            default:
                printf("Invalid choice.\n");
        }
    }
    return 0;
}

// --- Function Implementations ---

// Initialize the buffer with a small default capacity
void initBuffer(TextBuffer *buffer) {
    buffer->count = 0;
    buffer->capacity = INITIAL_CAPACITY;
    buffer->lines = (char **)malloc(buffer->capacity * sizeof(char *));
    
    if (buffer->lines == NULL) {
        perror("Initialization failed");
        exit(EXIT_FAILURE);
    }
}

// Insert a line at a specific index using Dynamic Memory Allocation
void insertLine(TextBuffer *buffer, int index, const char *text) {
    // Validate index
    if (index < 0 || index > buffer->count) {
        fprintf(stderr, "Error: Index out of bounds.\n");
        return;
    }

    // 1. Grow the array if full
    if (buffer->count == buffer->capacity) {
        int newCapacity = buffer->capacity * 2;
        char **newLines = (char **)realloc(buffer->lines, newCapacity * sizeof(char *));
        
        if (newLines == NULL) {
            perror("Realloc failed during expansion"); 
            exit(EXIT_FAILURE);
        }
        
        buffer->lines = newLines;
        buffer->capacity = newCapacity;
        printf("Debug: Expanded capacity to %d\n", buffer->capacity);
    }

    // 2. Shift pointers to make space (pointer manipulation, not string copying)
    if (index < buffer->count) {
        // memmove handles overlapping memory regions safely 
        memmove(&buffer->lines[index + 1], 
                &buffer->lines[index], 
                (buffer->count - index) * sizeof(char *));
    }

    // 3. Allocate EXACT memory for the new string
    // strlen + 1 ensures space for the null terminator 
    buffer->lines[index] = (char *)malloc((strlen(text) + 1) * sizeof(char));
    
    if (buffer->lines[index] == NULL) {
        perror("Malloc failed for string allocation");
        exit(EXIT_FAILURE);
    }

    // 4. Copy text into the allocated memory
    strcpy(buffer->lines[index], text);
    buffer->count++;
    printf("Line inserted at index %d.\n", index);
}

// Delete a line and free its memory
void deleteLine(TextBuffer *buffer, int index) {
    if (index < 0 || index >= buffer->count) {
        fprintf(stderr, "Error: Index out of bounds.\n");
        return;
    }

    // 1. Free the memory of the specific string
    free(buffer->lines[index]); // 

    // 2. Compact the array by shifting pointers left
    if (index < buffer->count - 1) {
        memmove(&buffer->lines[index], 
                &buffer->lines[index + 1], 
                (buffer->count - index - 1) * sizeof(char *));
    }

    buffer->count--;
    printf("Line deleted at index %d.\n", index);
}

// Print all stored lines with their line numbers
void printAllLines(const TextBuffer *buffer) {
    printf("\n--- Buffer Content ---\n");
    if (buffer->count == 0) {
        printf("(Empty)\n");
    } else {
        for (int i = 0; i < buffer->count; i++) {
            printf("%d: %s\n", i, buffer->lines[i]);
        }
    }
    printf("----------------------\n");
}

// Reduce memory capacity to match the actual number of lines
void shrinkToFit(TextBuffer *buffer) {
    if (buffer->count == 0) return; // Avoid reducing to 0 capacity

    if (buffer->count < buffer->capacity) {
        char **newLines = (char **)realloc(buffer->lines, buffer->count * sizeof(char *));
        
        if (newLines == NULL && buffer->count > 0) {
            perror("Realloc failed during shrink");
            return;
        }
        
        buffer->lines = newLines;
        buffer->capacity = buffer->count;
        printf("Debug: Memory shrunk to fit %d lines.\n", buffer->capacity); 
    } else {
        printf("Debug: Buffer is already optimal size.\n");
    }
}

// Save the current state to a file
void saveToFile(const TextBuffer *buffer, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("Error opening file for writing"); 
        return;
    }

    for (int i = 0; i < buffer->count; i++) {
        fprintf(fp, "%s\n", buffer->lines[i]);
    }

    fclose(fp);
    printf("Buffer saved to '%s'.\n", filename);
}

// Load lines from a file (rebuilding the dynamic structure)
void loadFromFile(TextBuffer *buffer, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error opening file for reading");
        return;
    }

    // Clear existing buffer before loading
    freeAll(buffer);
    initBuffer(buffer);

    char tempBuffer[MAX_INPUT_BUFFER];
    
    // Read file line by line
    while (fgets(tempBuffer, MAX_INPUT_BUFFER, fp)) {
        // Remove newline character if present
        tempBuffer[strcspn(tempBuffer, "\n")] = 0;
        
        // Insert at the end (append)
        insertLine(buffer, buffer->count, tempBuffer); 
    }

    fclose(fp);
    printf("Loaded content from '%s'.\n", filename);
}

// Free all dynamically allocated memory
void freeAll(TextBuffer *buffer) {
    for (int i = 0; i < buffer->count; i++) {
        free(buffer->lines[i]); // Free each string
    }
    free(buffer->lines); // Free the array of pointers
    buffer->lines = NULL;
    buffer->count = 0;
    buffer->capacity = 0;
    printf("Memory cleaned up.\n");
}