//Muhammad Anas
//25k-0899
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct Employee
    {
        int ID;
        char Name[100];
        char Designation[100];
        float salary;
    };



void addEmployee(struct Employee employee[], int *count);
void displayEmployees(struct Employee employee[], int count);
void findHighestSalary(struct Employee employee[], int count);
void searchEmployee(struct Employee employee[], int count);
void applyingBonus(struct Employee employee[], int count);

int main()

{
    
    int count = 0;
    struct Employee employee[100];
    int choice;
    while (1)
    {
        printf("---------------------------------------\n");
        printf("        Employee Review System         \n");
        printf("---------------------------------------\n");
        printf("1.Input Employee Reocord\n");
        printf("2.Display Employee data\n");
        printf("3.Find highest salary employee\n");
        printf("4.search empoyee\n");
        printf("5. Apply Bonus (Low Salary)\n");
        printf("0.Exit\n");
        printf("Enter your choice:");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            addEmployee(employee, &count);
            break;
        case 2:
            displayEmployees(employee, count);
            break;
        case 3:
            findHighestSalary(employee, count);
            break;
        case 4:
            searchEmployee(employee, count);
            break;
        case 5:
            applyingBonus(employee, count);
            break;
        case 0:
            printf("program exiting...............\n");
            exit(1);
        default:
            printf("Invalid choice!\n");
        }
    }
}
void addEmployee(struct Employee employee[], int *count)
{
    int n, i;
    printf("Enter how many Employees you want to add:");
    scanf("%d", &n);
    for (i = 0; i < n; i++)
    {
        printf("Enter for Employee %d:\n", *count + 1);
        printf("Enter ID:");
        scanf("%d", &employee[*count].ID);
        printf("Enter Name:");
        fflush(stdin);
        gets(employee[*count].Name);
        printf("Enter Designation: ");
        gets(employee[*count].Designation);
        printf("Enter salary:");
        scanf("%f", &employee[*count].salary);
        (*count)++;
    }
    printf("\n%d Employees added successfully.\n", n);
}
void displayEmployees(struct Employee employee[], int count)
{
    if (count == 0)
    {
        printf("No recored found\n");
        return;
    }
    printf("\n%-10s %-20s %-20s %-10s\n", "ID", "Name", "Designation", "Salary");
    printf("------------------------------------------------------------\n");
    for (int i = 0; i < count; i++)
    {
        printf("%-10d %-20s %-20s %-10.2f\n", employee[i].ID, employee[i].Name, employee[i].Designation, employee[i].salary);
    }
}
void findHighestSalary(struct Employee employee[], int count)
{
    if (count == 0)
    {
        printf("No recored found\n");
        return;
    }
    int max = 0;
    for (int i = 0; i < count; i++)
    {
        if (employee[i].salary > employee[max].salary)
        {
            max = i;
        }
    }
    printf("\n--- Highest Salary Employee ---\n");
    printf("Name: %s\n", employee[max].Name);
    printf("Salary: %.2f\n", employee[max].salary);
    return;
}
void searchEmployee(struct Employee employee[], int count)
{
    int search, id;
    int found =0;
    char name[50];
    printf("Search Option\n");
    printf("1.ID\n");
    printf("2.Name\n");
    scanf("%d", &search);
    if (search == 1)
    {
        printf("Enter ID of Employee:");
        scanf("%d", &id);
        for (int i = 0; i < count; i++)
        {
            if (employee[i].ID == id)
            {
                found = 1;
                printf("Found: %s (Designation: %s)\n", employee[i].Name, employee[i].Designation);
                break;
            }
        }
    }else if(search == 2){
        printf("Enter name of Employee:");
        fflush(stdin);
        gets(name);
        for(int i=0;i<count;i++){
            if(strcmp(employee[i].Name,name) == 0){
                found = 1;
                printf("Found: ID %d (Salary: %.2f)\n", employee[i].ID, employee[i].salary);
            }
        }
    }else{
        printf("Invalid option\n");
        return;
    }

    if(!found){
        printf("No record found!");
        return;
    }
    return;
}
void applyingBonus(struct Employee employee[], int count){
    float threshold = 50000;
    float bonus;
    printf("Applying 10%% bonus to all minimum threshold %.2f salaries\n",threshold);
    for(int i=0;i<count;i++){
        if(employee[i].salary < threshold){
           bonus = .10*employee[i].salary;
           employee[i].salary += bonus;
           printf("Updated %s's salary to %.2f\n", employee[i].Name, employee[i].salary);
        }
    }
}