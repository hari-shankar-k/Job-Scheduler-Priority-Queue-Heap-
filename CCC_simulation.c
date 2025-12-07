#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_JOBS 100
#define NAME_LEN 50

// ---------------------------
// Job Structure
// ---------------------------
typedef struct {
    int id;                  // Unique job ID
    char name[NAME_LEN];     // Job name / description
    int priority;            // 1 = Low, 2 = Medium, 3 = High
    int burstTime;           // CPU time or duration
    int arrivalOrder;        // To break ties: smaller = earlier
} Job;

// ---------------------------
// Max-Heap (Priority Queue)
// ---------------------------
Job heap[MAX_JOBS + 1];  // 1-based indexing
int heapSize = 0;
int nextJobId = 1;
int arrivalCounter = 0;

// ---------------------------
// Utility Functions
// ---------------------------
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int isEmpty() {
    return heapSize == 0;
}

int isFull() {
    return heapSize == MAX_JOBS;
}

void swapJobs(int i, int j) {
    Job temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
}

// Return 1 if a has higher priority than b
int isHigherPriority(Job a, Job b) {
    if (a.priority > b.priority)
        return 1;
    if (a.priority < b.priority)
        return 0;

    // Same priority → earlier arrival wins
    return a.arrivalOrder < b.arrivalOrder;
}

// ---------------------------
// Heapify Up (for Insert)
// ---------------------------
void heapifyUp(int index) {
    while (index > 1) {
        int parent = index / 2;
        if (isHigherPriority(heap[index], heap[parent])) {
            swapJobs(index, parent);
            index = parent;
        } else {
            break;
        }
    }
}

// ---------------------------
// Heapify Down (for Delete)
// ---------------------------
void heapifyDown(int index) {
    while (1) {
        int left = 2 * index;
        int right = 2 * index + 1;
        int largest = index;

        if (left <= heapSize && isHigherPriority(heap[left], heap[largest]))
            largest = left;
        if (right <= heapSize && isHigherPriority(heap[right], heap[largest]))
            largest = right;

        if (largest != index) {
            swapJobs(index, largest);
            index = largest;
        } else {
            break;
        }
    }
}

// ---------------------------
// Print a Single Job
// ---------------------------
void printJob(Job j) {
    const char *pText;
    if (j.priority == 3) pText = "High";
    else if (j.priority == 2) pText = "Medium";
    else pText = "Low";

    printf("---------------------------------------------\n");
    printf("Job ID       : %d\n", j.id);
    printf("Name         : %s\n", j.name);
    printf("Priority     : %s (%d)\n", pText, j.priority);
    printf("Burst Time   : %d\n", j.burstTime);
    printf("Arrival Order: %d\n", j.arrivalOrder);
    printf("---------------------------------------------\n");
}

// ---------------------------
// Add Job (Insert into Heap)
// ---------------------------
void addJob() {
    if (isFull()) {
        printf("Job queue is full! Cannot add more jobs.\n");
        return;
    }

    Job j;
    j.id = nextJobId++;
    j.arrivalOrder = ++arrivalCounter;

    printf("Enter job name/description: ");
    fgets(j.name, NAME_LEN, stdin);
    size_t len = strlen(j.name);
    if (len > 0 && j.name[len - 1] == '\n')
        j.name[len - 1] = '\0';

    printf("Select priority (1 = Low, 2 = Medium, 3 = High): ");
    scanf("%d", &j.priority);
    clearInputBuffer();

    if (j.priority < 1 || j.priority > 3) {
        printf("Invalid priority! Setting to Low (1).\n");
        j.priority = 1;
    }

    printf("Enter burst time (e.g., required CPU time): ");
    scanf("%d", &j.burstTime);
    clearInputBuffer();

    heapSize++;
    heap[heapSize] = j;
    heapifyUp(heapSize);

    printf("Job added successfully with ID %d.\n", j.id);
}

// ---------------------------
// Execute Highest Priority Job
// ---------------------------
void executeJob() {
    if (isEmpty()) {
        printf("No jobs available to execute.\n");
        return;
    }

    Job topJob = heap[1];  // Highest priority job

    // Replace root with last element and heapify down
    heap[1] = heap[heapSize];
    heapSize--;
    if (heapSize > 0)
        heapifyDown(1);

    printf("\n=== Executing Job ===\n");
    printJob(topJob);
    printf("Job %d executed successfully.\n", topJob.id);
}

// ---------------------------
// View All Jobs in the Queue
// ---------------------------
void viewJobs() {
    if (isEmpty()) {
        printf("No jobs in the scheduler.\n");
        return;
    }

    printf("\n=== Current Jobs in Priority Queue ===\n");
    printf("(Note: Display order is heap order, not sorted.)\n");
    for (int i = 1; i <= heapSize; i++) {
        printf("\nJob at position %d in heap:\n", i);
        printJob(heap[i]);
    }
}

// ---------------------------
// Main Menu
// ---------------------------
int main() {
    int choice;

    do {
        printf("\n=============================================\n");
        printf("         Job Scheduler (Priority Queue)      \n");
        printf("=============================================\n");
        printf("1. Add Job\n");
        printf("2. Execute Highest Priority Job\n");
        printf("3. View All Jobs\n");
        printf("4. Exit\n");
        printf("---------------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
        case 1:
            addJob();
            break;
        case 2:
            executeJob();
            break;
        case 3:
            viewJobs();
            break;
        case 4:
            printf("Exiting Job Scheduler. Goodbye!\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }

    } while (choice != 4);

    return 0;
}
