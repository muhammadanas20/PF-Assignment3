#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "members.dat"

// Struct Definitions 
typedef struct {
    int year, month, day;
} Date;

struct Student {
    int ID;                     
    char Name[100];             
    char batch[100];            
    char membershipType[10];    // IEEE or ACM
    Date regDate;               
    Date dob;                   
    char interest[20];          // IEEE/ACM/Both
};

// Global Variables 
struct Student *studentArray = NULL;
int studentCount = 0;

// Function Prototypes 
void loadDatabase(const char *filename);
void saveDatabase(const char *filename);
void addStudent();
void updateStudent(int studentID);
void deleteStudent(int studentID);
void printData();
void batchReport();
void freeMemory();

//  Main Function
int main() {
    int choice, tempID;

    // 1. Load existing data on startup
    loadDatabase(FILE_NAME);

    while(1) {
        printf("\n=== IEEE/ACM Membership Manager ===\n");
        printf("1. Register a new student\n");
        printf("2. Update membership type or batch\n");
        printf("3. Delete a student registration\n");
        printf("4. View all registrations\n");
        printf("5. Generate batch-wise reports\n");
        printf("6. Exit\n");
        printf("Enter choice: ");

        if(scanf("%d", &choice) != 1) {
            while(getchar() != '\n'); // Flush input buffer
            continue;
        }
        while(getchar() != '\n'); 

        switch(choice) {
            case 1:
                addStudent();
                break;
            case 2:
                printf("Enter Student ID to update: ");
                scanf("%d", &tempID);
                updateStudent(tempID);
                break;
            case 3:
                printf("Enter Student ID to delete: ");
                scanf("%d", &tempID);
                deleteStudent(tempID);
                break;
            case 4:
                printData();
                break;
            case 5:
                batchReport();
                break;
            case 6:
                
                saveDatabase(FILE_NAME); 
                freeMemory();
                printf("Exiting program. Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
    return 0;
}


// Load data from binary file into Dynamic Array 
void loadDatabase(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("[Info] No existing database found. Starting fresh.\n");
        return;
    }

    struct Student temp;
    while (fread(&temp, sizeof(struct Student), 1, fp)) {
        // Expand array by 1 for each record found
        struct Student *tempPtr = realloc(studentArray, (studentCount + 1) * sizeof(struct Student));
        if (tempPtr == NULL) {
            printf("[Error] Memory allocation failed during load.\n");
            fclose(fp);
            exit(1);
        }
        studentArray = tempPtr;
        studentArray[studentCount] = temp;
        studentCount++;
    }
    fclose(fp);
    printf("[Success] Loaded %d records from %s.\n", studentCount, filename);
}

// Save entire Dynamic Array to binary file 
void saveDatabase(const char *filename) {
    FILE *fp = fopen(filename, "wb"); // 'wb'
    if (fp == NULL) {
        printf("[Error] Could not open file for writing.\n");
        return;
    }
    if (studentCount > 0) {
        fwrite(studentArray, sizeof(struct Student), studentCount, fp);
    }
    fclose(fp);
    printf("[Success] Database saved to file.\n");
}


// Register New Student 
void addStudent() {
    struct Student s;
    
    printf("\n--- Register Student ---\n");
    printf("Enter Student ID: ");
    scanf("%d", &s.ID);
    while(getchar() != '\n'); // Flush buffer

    // Check ID Uniqueness 
    for(int i=0; i<studentCount; i++) {
        if(studentArray[i].ID == s.ID) {
            printf("[Error] Student ID %d already exists!\n", s.ID);
            return;
        }
    }

    printf("Enter Full Name: ");
    fgets(s.Name, 100, stdin);
    s.Name[strcspn(s.Name, "\n")] = 0; // Remove newline

    printf("Enter Batch (CS, SE, Cyber, AI): ");
    fgets(s.batch, 100, stdin);
    s.batch[strcspn(s.batch, "\n")] = 0;

    printf("Enter Membership Type (IEEE / ACM): ");
    scanf("%s", s.membershipType);

    printf("Enter Registration Date (YYYY MM DD): ");
    scanf("%d %d %d", &s.regDate.year, &s.regDate.month, &s.regDate.day);

    printf("Enter Date of Birth (YYYY MM DD): ");
    scanf("%d %d %d", &s.dob.year, &s.dob.month, &s.dob.day);

    printf("Enter Interest (IEEE/ACM/Both): ");
    scanf("%s", s.interest);

    // Expand RAM
    struct Student *tempPtr = realloc(studentArray, (studentCount + 1) * sizeof(struct Student));
    if (tempPtr == NULL) {
        printf("[Error] Memory allocation failed.\n");
        return;
    }
    studentArray = tempPtr;
    studentArray[studentCount] = s;
    studentCount++;

    // Save to file immediately
    saveDatabase(FILE_NAME); 
}

// Update Student Info
void updateStudent(int studentID) {
    int foundIndex = -1;
    for (int i = 0; i < studentCount; i++) {
        if (studentArray[i].ID == studentID) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        printf("[Error] Student ID %d not found.\n", studentID);
        return;
    }

    int choice;
    printf("\nUpdating Record for: %s\n", studentArray[foundIndex].Name);
    printf("1. Update Batch\n");
    printf("2. Update Membership Type\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    while(getchar() != '\n'); 

    if (choice == 1) {
        printf("Enter new Batch: ");
        fgets(studentArray[foundIndex].batch, 100, stdin);
        studentArray[foundIndex].batch[strcspn(studentArray[foundIndex].batch, "\n")] = 0;
    } else if (choice == 2) {
        printf("Enter new Membership Type: ");
        scanf("%s", studentArray[foundIndex].membershipType);
    } else {
        printf("Invalid choice.\n");
        return;
    }

    printf("[Success] Record updated.\n");
    saveDatabase(FILE_NAME);
}

// Delete Student 
void deleteStudent(int studentID) {
    int foundIndex = -1;
    for (int i = 0; i < studentCount; i++) {
        if (studentArray[i].ID == studentID) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        printf("[Error] Student ID %d not found.\n", studentID);
        return;
    }

    // Shift elements left to overwrite the deleted record
    for (int i = foundIndex; i < studentCount - 1; i++) {
        studentArray[i] = studentArray[i+1];
    }

    studentCount--;
    
   
    if (studentCount > 0) {
        struct Student *tempPtr = realloc(studentArray, studentCount * sizeof(struct Student));
        if (tempPtr != NULL) {
            studentArray = tempPtr;
        }
    } else {
        free(studentArray);
        studentArray = NULL;
    }

    printf("[Success] Student deleted.\n");
    saveDatabase(FILE_NAME); 
}

// Display All 
void printData() {
    if (studentCount == 0) {
        printf("\nNo records found.\n");
        return;
    }

    printf("\n%-5s %-20s %-10s %-10s %-15s\n", "ID", "Name", "Batch", "Type", "Interest");
    printf("---------------------------------------------------------------\n");
    for (int i = 0; i < studentCount; i++) {
        printf("%-5d %-20s %-10s %-10s %-15s\n", 
            studentArray[i].ID, 
            studentArray[i].Name, 
            studentArray[i].batch, 
            studentArray[i].membershipType,
            studentArray[i].interest
        );
    }
}

// Batch-wise Report 
void batchReport() {
    char filterBatch[100];
    while(getchar() != '\n'); // clear buffer just in case
    printf("\nEnter Batch to filter (e.g., CS, SE): ");
    fgets(filterBatch, 100, stdin);
    filterBatch[strcspn(filterBatch, "\n")] = 0;

    printf("\n--- Report for Batch: %s ---\n", filterBatch);
    printf("%-5s %-20s %-10s\n", "ID", "Name", "Membership");
    
    int found = 0;
    for (int i = 0; i < studentCount; i++) {
        if (strcmp(studentArray[i].batch, filterBatch) == 0) {
            printf("%-5d %-20s %-10s\n", 
                studentArray[i].ID, 
                studentArray[i].Name, 
                studentArray[i].membershipType
            );
            found = 1;
        }
    }
    if (!found) printf("No students found in this batch.\n");
}

void freeMemory() {
    if (studentArray != NULL) {
        free(studentArray);
        studentArray = NULL;
    }
}