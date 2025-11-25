#include <stdio.h>
#include <string.h>

struct Shelf
{
    int ID;
    int popularityScore;
};
void access(struct Shelf s[],int count,int id);
void add(struct Shelf s[], int max, int *count,int id,int score);
int main()
{
    int capacity;
    int Q;
    int count = 0;
    int id,score;
    scanf("%d", &capacity);
    scanf("%d", &Q);
    char command[10];
    struct Shelf s[capacity];
    for (int i = 0; i < Q; i++)
    {
       scanf("%s", command);
       if(strcmp(command,"ADD")){
        scanf("%d %d",&id,&score);
        add(s, capacity, &count,id,score);
       }else if(strcmp(command,"ACCESS")){
            scanf("%d",&id);
           access(s, count,id);
        }  
    }
    return 0;
}
void moveToFront(struct Shelf s[], int index) {
    struct Shelf temp = s[index];
    
    // Shift everything from 0 to index-1 one step to the right
    for (int i = index; i > 0; i--) {
        s[i] = s[i-1];
    }
    
    // Place the item at the front
    s[0] = temp;
}

void add(struct Shelf s[], int capacity, int *count, int id, int score) {
    // Case 1: Check if book already exists
    for (int i = 0; i < *count; i++) {
        if (s[i].ID == id) {
            s[i].popularityScore = score; // Update score
            moveToFront(s, i);            // Mark as most recently used
            return;
        }
    }

    // Case 2: New Book, but Shelf is NOT full
    if (*count < capacity) {
        // Shift everyone to the right to make space at index 0
        for (int i = *count; i > 0; i--) {
            s[i] = s[i-1];
        }
        (*count)++; // Increase count
    } 
    // Case 3: New Book, Shelf IS FULL
    else {
        // Shift everyone to the right (overwriting the last element)
        for (int i = capacity - 1; i > 0; i--) {
            s[i] = s[i-1];
        }
    }

    // Insert new book at the front (Most Recently Used position)
    s[0].ID = id;
    s[0].popularityScore = score;
}

void access(struct Shelf s[], int count, int id) {
    for (int i = 0; i < count; i++) {
        if (s[i].ID == id) {
            printf("%d\n", s[i].popularityScore);
            moveToFront(s, i); // Important: Move accessed book to front
            return;
        }
    }
    printf("-1\n");
}