#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Student
{
    int id;
    char name[50];
    float marks;
    struct Student *left, *right;
};

/* Create a new student */
struct Student* createStudent(int id, char name[], float marks)
{
    struct Student *node = malloc(sizeof(struct Student));
    
    if(node == NULL)
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    
    node->id = id;
    strcpy(node->name, name);
    node->marks = marks;
    node->left = NULL;
    node->right = NULL;
    
    return node;
}

/* Search student by ID */
struct Student* search(struct Student *root, int id)
{
    if(root == NULL || root->id == id)
        return root;
        
    if(id < root->id)
        return search(root->left, id);
        
    return search(root->right, id);
}

/* Insert student */
struct Student* insert(struct Student *root, int id,
                       char name[], float marks)
{
    if(root == NULL)
        return createStudent(id, name, marks);
        
    if(id < root->id)
        root->left = insert(root->left, id, name, marks);
    else if(id > root->id)
        root->right = insert(root->right, id, name, marks);
        
    return root;
}

/* Display all students in ascending ID order */
void display(struct Student *root)
{
    if(root == NULL)
        return;
        
    display(root->left);
    
    printf("| %-8d | %-20s | %7.2f |\n",
           root->id, root->name, root->marks);
           
    display(root->right);
}

/* Find minimum node */
struct Student* minimum(struct Student *root)
{
    while(root != NULL && root->left != NULL)
        root = root->left;
        
    return root;
}

/* Delete student */
struct Student* deleteStudent(struct Student *root, int id)
{
    if(root == NULL)
        return NULL;
        
    if(id < root->id)
    {
        root->left = deleteStudent(root->left, id);
    }
    else if(id > root->id)
    {
        root->right = deleteStudent(root->right, id);
    }
    else
    {
        if(root->left == NULL)
        {
            struct Student *temp = root->right;
            free(root);
            return temp;
        }
        
        if(root->right == NULL)
        {
            struct Student *temp = root->left;
            free(root);
            return temp;
        }
        
        struct Student *temp = minimum(root->right);
        
        root->id = temp->id;
        strcpy(root->name, temp->name);
        root->marks = temp->marks;
        
        root->right = deleteStudent(root->right, temp->id);
    }
    
    return root;
}

/* Display BST with root at the top */
void displayTree(struct Student *root, int level)
{
    int i;
    
    if(root == NULL)
        return;
        
    for(i = 0; i < level; i++)
        printf("    ");
        
    printf("[%d]\n", root->id);
    
    if(root->left != NULL || root->right != NULL)
    {
        for(i = 0; i < level; i++)
            printf("    ");
            
        printf("   / \\\n");
        
        if(root->left != NULL)
        {
            for(i = 0; i < level; i++)
                printf("    ");
            printf("L: ");
            displayTree(root->left, level + 1);
        }
        
        if(root->right != NULL)
        {
            for(i = 0; i < level; i++)
                printf("    ");
            printf("R: ");
            displayTree(root->right, level + 1);
        }
    }
}

/* Count students */
int countStudents(struct Student *root)
{
    if(root == NULL)
        return 0;
        
    return 1 + countStudents(root->left)
           + countStudents(root->right);
}

/* Calculate total marks */
float totalMarks(struct Student *root)
{
    if(root == NULL)
        return 0;
        
    return root->marks
           + totalMarks(root->left)
           + totalMarks(root->right);
}

/* Release memory */
void freeTree(struct Student *root)
{
    if(root != NULL)
    {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

int main()
{
    struct Student *root = NULL;
    struct Student *found;
    
    int choice, id, total;
    char name[50];
    float marks;
    
    do
    {
        printf("\n");
        printf("============================================\n");
        printf("       STUDENT MANAGEMENT SYSTEM\n");
        printf("       Using Binary Search Tree\n");
        printf("============================================\n");
        printf("1. Insert Student Record\n");
        printf("2. Search Student Record\n");
        printf("3. Display All Students\n");
        printf("4. Delete Student Record\n");
        printf("5. Display BST Structure\n");
        printf("6. Student Statistics\n");
        printf("7. Exit\n");
        printf("--------------------------------------------\n");
        printf("Enter your choice: ");
        
        if(scanf("%d", &choice) != 1)
        {
            printf("Invalid input!\n");
            break;
        }
        
        switch(choice)
        {
            case 1:
                printf("\nEnter Student ID: ");
                if(scanf("%d", &id) != 1)
                {
                    printf("Invalid ID!\n");
                    freeTree(root);
                    return 1;
                }
                
                if(search(root, id) != NULL)
                {
                    printf("Student ID already exists!\n");
                    break;
                }
                
                printf("Enter Student Name: ");
                scanf(" %49[^\n]", name);
                
                printf("Enter Marks (0-100): ");
                if(scanf("%f", &marks) != 1)
                {
                    printf("Invalid marks!\n");
                    freeTree(root);
                    return 1;
                }
                
                if(marks < 0 || marks > 100)
                {
                    printf("Marks must be between 0 and 100!\n");
                    break;
                }
                
                root = insert(root, id, name, marks);
                printf("Student added successfully!\n");
                break;
                
            case 2:
                printf("\nEnter Student ID to search: ");
                scanf("%d", &id);
                
                found = search(root, id);
                
                if(found != NULL)
                {
                    printf("\nStudent Found!\n");
                    printf("Student ID : %d\n", found->id);
                    printf("Name       : %s\n", found->name);
                    printf("Marks      : %.2f\n", found->marks);
                }
                else
                {
                    printf("Student not found!\n");
                }
                break;
                
            case 3:
                if(root == NULL)
                {
                    printf("\nNo student records available!\n");
                }
                else
                {
                    printf("\nSTUDENT RECORDS (SORTED BY ID)\n");
                    printf("+----------+----------------------+---------+\n");
                    printf("| %-8s | %-20s | %7s |\n",
                           "ID", "Student Name", "Marks");
                    printf("+----------+----------------------+---------+\n");
                    
                    display(root);
                    
                    printf("+----------+----------------------+---------+\n");
                }
                break;
                
            case 4:
                printf("\nEnter Student ID to delete: ");
                scanf("%d", &id);
                
                if(search(root, id) == NULL)
                {
                    printf("Student not found!\n");
                }
                else
                {
                    root = deleteStudent(root, id);
                    printf("Student deleted successfully!\n");
                }
                break;
                
            case 5:
                printf("\n========== BST STRUCTURE ==========\n");
                
                if(root == NULL)
                    printf("Tree is empty!\n");
                else
                    displayTree(root, 0);
                    
                break;
                
            case 6:
                total = countStudents(root);
                
                printf("\n========== STUDENT STATISTICS ==========\n");
                printf("Total Students : %d\n", total);
                
                if(total > 0)
                    printf("Average Marks  : %.2f\n",
                           totalMarks(root) / total);
                else
                    printf("Average Marks  : Not available\n");
                    
                break;
                
            case 7:
                printf("\nThank you for using the system!\n");
                break;
                
            default:
                printf("Invalid choice! Try again.\n");
        }
        
    }
    while(choice != 7);
    
    freeTree(root);
    
    return 0;
}