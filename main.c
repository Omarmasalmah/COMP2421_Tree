/*   ---------|------------********************-----------|--------------------------------------
              |      --------->OMAR MASALMAH<---------    |
              |       --------->1200060<---------         |
              |      --------->SECTION (1)<---------      |
              |       --------->PROJECT 2<---------       |
     ---------|------------********************-----------|--------------------------------------

*/

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>  // for getch

#define size 200
typedef char String[size];
typedef struct Tree *tree;

tree ReadFile(tree T);
tree FindMin(tree T);
int Height( tree P );
int Max( int Lhs, int Rhs );
tree SingleRotateLeft( tree K2 );
tree SingleRotateRight( tree K1 );
tree DoubleRotateLeft( tree K3 );
tree DoubleRotateRight( tree K1 );
tree InsertCourse(  String courseName,int creditHours,String courseCode,String department,String token, tree T );
void PrintInOrder( tree T);
tree MakeEmpty( tree T );
void printTopic(tree T);
tree InsertNewCourse(tree T);
tree FindByCourseCode(String courseCode, tree T);
void updateCourseInfo(tree T);
void FindAndPrintByDepartment(tree T, String department);
int getBalance(tree B);
tree deleteCourse(tree root, String courseCode);
void PrintOutToFile(tree T,FILE *fileOut);
void getlettar( tree T,char c);
void getDepartment( tree T,String department);

struct Tree{
    String CourseName;
    int CreditHours;
    String CourseCode;
    String Department;
    char Topic[200];
    tree Left;
    tree Right;
    int Height;

};

String courses[100];
int courseNum=0;

int main() {
    tree T = MakeEmpty(NULL); //create an empty tree
    String coursCode,department;
    char l;
    FILE *fileOut;

    int number;
    while(number!=11){ //menu to display to the user
        printf("\n-------------------------------------------------------------------------------------------------\n");
        printf(" 1- Read the file courses.txt file and create the tree.\n");
        printf(" 2- Insert a new course from the user with all its associated data.\n");
        printf(" 3- Find a course and support updating of its information.  \n");
        printf(" 4- List courses in lexicographic order with their associated information(credit hours, IDs, and topics). \n");
        printf(" 5- List all topics associated with a given course\n");
        printf(" 6- List all courses in lexicographic order that belong to the same department.\n");
        printf(" 7- Delete a course.\n");
        printf(" 8- Delete all courses that start with a specific letter.\n");
        printf(" 9- Delete all courses belong to a given department.\n");
        printf(" 10- Save all words in file offered_courses.txt\n");
        printf(" To exit the program enter -1.\n");



        scanf("%d",&number);


        switch(number){ //switch case

            case 1:
                T = ReadFile(T);
                break;
            case 2:
                InsertNewCourse(T);
                break;
            case 3:
                printf("Enter the course code to find it: \n");
                scanf("\n%[^\n]s",coursCode);
                if (FindByCourseCode(coursCode, T) == NULL) {
                    printf("Not Found !! Press any key to back to menu...\n");
                    getch();
                    break;
                }
                int x;
                printf("Course is found...\n");
                printf("If you want to update the information press 1\n");
                scanf("%d",&x);
                if(x == 1){
                    updateCourseInfo(T);
                }
                break;
            case 4:
                printf("-----------------------------------------------------------------------\n");
                PrintInOrder(T);
                printf("-----------------------------------------------------------------------\n");

                break;
            case 5:
                printf("Enter the course code to find it: \n");
                scanf("\n%[^\n]s",coursCode);
                if (FindByCourseCode(coursCode, T) == NULL) {
                    printf("Not Found !! Press any key to back to menu...\n");
                    getch();
                    break;
                }
                tree temp = FindByCourseCode(coursCode,T);
                printTopic(temp);

                break;
            case 6:
                printf("Enter the department to find it: \n");
                scanf("\n%[^\n]s",department);
                FindAndPrintByDepartment(T,department);

                break;
            case 7:
                printf("Enter the course code to find it: \n");
                scanf("\n%[^\n]s",coursCode);
                if (FindByCourseCode(coursCode, T) == NULL) {
                    printf("Not Found !! Press any key to back to menu...\n");
                    getch();
                    break;
                }
                deleteCourse(T,coursCode);
                printf("The course is deleted...\n");
                break;
            case 8:
                printf("Enter the letter: \n");
                scanf(" %c",&l);

                getlettar(T,l);

                for (int i = 0; i < courseNum; ++i) {
                    printf("<<%s>>\n",courses[i]);
                    T = deleteCourse(T,courses[i]);
                    printf("ok\n");
                }

                break;
            case 9:
                printf("Enter the department: \n");
                scanf("\n%[^\n]s",department);

                getDepartment(T,department);
                for (int i = 0; i < courseNum; ++i) {
                    printf("<<%s>>\n",courses[i]);
                    T = deleteCourse(T,courses[i]);
                    printf("ok\n");
                }

                break;

            case 10:
                fileOut = fopen("offered_courses.txt", "w");
                if (fileOut == NULL) {
                    printf("Error In File !! Could not open File !!\n");
                    exit(-1);

                }
                PrintOutToFile(T,fileOut);
                fclose(fileOut);
                printf("Save to file...\n");
                break;
            case -1:
                printf("Exit the program.\n");
                break;
            default:
                printf("Wrong input!! Please try again.\n\n");

        }
    }
    return 0;
}

tree ReadFile(tree T){ //function to read file and insert the information to tree
    char line[250];

    FILE *treeFile;
    treeFile = fopen("courses.txt", "r"); //open tree file to read
    if (treeFile ==NULL)
    {
        printf("The File is empty,Try Again");
        exit (-1) ;
    }
    char *token;
    String courseName;
    int creditHours;
    String courseCode,department;
    while(fgets(line,250,treeFile)!=NULL) {
        token = strtok(line, "/");
        sscanf(token, "%[^:]:%d#%[^#]#%[^/]/", courseName, &creditHours, courseCode,department);//to read the line and split it

        token = strtok(NULL, "\n");

        T = InsertCourse(courseName, creditHours, courseCode, department, token, T);

    }
    printf("<< Reading file is done >>\n");
    return T;

}

tree MakeEmpty( tree T ) //function to make the tree empty
{
    if( T != NULL )
    {
        MakeEmpty( T->Left );
        MakeEmpty( T->Right );
        free( T );
    }
    return NULL;
}
// this function finds the the smallest Course ( the one on the far left)
tree FindMin(tree T) {
    if (T == NULL)
        return NULL;
    else if (T->Left == NULL)
        return T;
    else
        return FindMin(T->Left);
}

// this function returns the height of a node , and -1 if its NULL
int Height( tree P )
{
    if( P == NULL )
        return -1;
    else
        return P->Height;
}
// this function returns the biggest number between the two entries
int Max( int Lhs, int Rhs )
{
    return Lhs > Rhs ? Lhs : Rhs;
}
// this function perform a a single left rotate
tree SingleRotateLeft( tree K2 )
{
    tree  K1;

    K1 = K2->Left;
    K2->Left = K1->Right;
    K1->Right = K2;

    K2->Height = Max( Height( K2->Left ), Height( K2->Right ) ) + 1;
    K1->Height = Max( Height( K1->Left ), K2->Height ) + 1;

    return K1;  /* New root */
}

/* This function can be called only if K1 has a right child */
/* Perform a rotate between a node (K1) and its right child */
/* Update heights, then return new root */


tree SingleRotateRight( tree K1 )  // this function perform a a single right rotate

{
    tree K2;

    K2 = K1->Right;
    K1->Right = K2->Left;
    K2->Left = K1;

    K1->Height = Max( Height( K1->Left ), Height( K1->Right ) ) + 1;
    K2->Height = Max( Height( K2->Right ), K1->Height ) + 1;

    return K2;  /* New root */
}

/* This function can be called only if K3 has a left child and K3's left child has a right child */
/* Do the left-right double rotation */
/* Update heights, then return new root */

tree DoubleRotateLeft( tree K3 )
{
    /* Rotate between K1 and K2 */
    K3->Left = SingleRotateRight( K3->Left );

    /* Rotate between K3 and K2 */
    return SingleRotateLeft( K3 );
}

/* This function can be called only if K1 has a right */
/* child and K1's right child has a left child */
/* Do the right-left double rotation */
/* Update heights, then return new root */

tree DoubleRotateRight( tree K1 )
{
    /* Rotate between K3 and K2 */
    K1->Right = SingleRotateLeft( K1->Right );

    /* Rotate between K1 and K2 */
    return SingleRotateRight( K1 );
}

// this function inserts a new course to the tree in correct position and regain the balance if needed
tree InsertCourse(  String courseName,int creditHours,String courseCode,String department,char *token, tree T )
{
    if( T == NULL )
    {
        /* Create and return a one-node tree */
        T = malloc( sizeof( struct Tree ) );
        if( T == NULL )
            printf( "Out of space!!!" );
        else
        {
            strcpy(T->CourseName, courseName);
            T->CreditHours = creditHours;
            strcpy(T->CourseCode, courseCode);
            strcpy(T->Department, department);
            strcpy(T->Topic, token);

            T->Height = 0;
            T->Left = T->Right = NULL;
        }
    }
    else if( strcmp(courseCode , T->CourseCode)== -1)
    {
        T->Left = InsertCourse( courseName, creditHours, courseCode, department, token, T->Left );

        if( Height( T->Left ) - Height( T->Right ) == 2 )
            if( strcmp(courseCode , T->CourseCode)==- 1 )
                T = SingleRotateLeft( T );
            else
                T = DoubleRotateLeft( T );
    }
    else if( strcmp(courseCode , T->CourseCode)== 1)
    {
        T->Right = InsertCourse (   courseName, creditHours, courseCode, department, token, T->Right );
        if( Height( T->Right ) - Height( T->Left ) == 2 )
            if( strcmp(courseCode , T->CourseCode)== 1 )
                T = SingleRotateRight( T );
            else
                T = DoubleRotateRight( T );

    }
    /* Else X is in the tree already; we'll do nothing */

    T->Height = Max( Height( T->Left ), Height( T->Right ) ) + 1;
    return T;
}

void PrintInOrder( tree T) // this function will prints all the data in the tree inOrder
{
    if( T != NULL)
    {
        PrintInOrder( T->Left );
        printf("\n %s\n %d \t%s \t%s\n", T->CourseName,T->CreditHours,T->CourseCode, T->Department);
        printf("%s \n",T->Topic);
        PrintInOrder( T->Right );
    }
}

void getlettar( tree T,char c) // this function will get all courses that have letter in the begining and store it to array
{
    if( T != NULL)
    {
        getlettar( T->Left ,c);
        if(T->CourseCode[0] == c){
            strcpy(courses[courseNum++],T->CourseCode);

        }
        getlettar( T->Right ,c);
    }
}

void printTopic(tree T){ //print topics for the course
    if( T != NULL)
    {
        printTopic( T->Left );
        printf("%s\n", T->Topic);
        printTopic( T->Right );
    }
}
tree InsertNewCourse(tree T){ // this function inserts a new course to the tree
    String courseName;
    int creditHours;
    String courseCode,department,topic;
    printf(" Enter the cousre name:\n ");
    scanf("\n%[^\n]s",courseName);
    printf("Enter the credit hours:\n ");
    scanf("%d",&creditHours);
    printf("Enter the cousre code:\n ");
    scanf("\n%[^\n]s",courseCode);
    printf("Enter the department of the course:\n ");
    scanf("\n%[^\n]s",department);
    printf("Enter the topics of course :\n ");
    scanf("\n%[^\n]s",topic);
    T = InsertCourse(courseName,creditHours,courseCode,department,topic,T);
    printf("Insert new course is done\n");

    return T;
}
tree FindByCourseCode(String courseCode, tree T) // this function searches for a course code  in the tree.
{
    if (T == NULL)
        return NULL; // -> empty tree
    if (strcmp(courseCode, T->CourseCode) < 0)
        return FindByCourseCode(courseCode, T->Left);
    else if (strcmp(courseCode, T->CourseCode) > 0)
        return FindByCourseCode(courseCode, T->Right);
    else if (strcmp(courseCode, T->CourseCode) == 0)
        return T; // -> found !
    else
        return NULL; // -> not found !
}
void updateCourseInfo(tree T) { //update course data

    String courseName;
    int creditHours;
    String courseCode,department,topic;
    printf("Update the information >>\n");
    printf(" Enter the cousre name:\n ");
    scanf("\n%[^\n]s",courseName);
    printf("Enter the credit hours:\n ");
    scanf("%d",&creditHours);
    printf("Enter the cousre code:\n ");
    scanf("\n%[^\n]s",courseCode);
    printf("Enter the department of the course:\n ");
    scanf("\n%[^\n]s",department);
    printf("Enter the topics of course :\n ");
    scanf("\n%[^\n]s",topic);
    strcpy(T->CourseName, courseName);
    T->CreditHours = creditHours;
    strcpy(T->CourseCode, courseCode);
    strcpy(T->Department, department);
    strcpy(T->Topic, topic);

}
void FindAndPrintByDepartment(tree T, String department) { // this function searches for a course department  in the tree and print his data
    if (T != NULL) {
        FindAndPrintByDepartment(T->Left, department);
        if (strcmp(department, T->Department) == 0) {
            printf("\n %s\n %d \t%s \t%s\n", T->CourseName,T->CreditHours,T->CourseCode, T->Department);
            printf("%s \n",T->Topic);
        }
        FindAndPrintByDepartment(T->Right, department);
    }
}

int getBalance(tree B) {// this function return the value of the height difference
    if (B == NULL)
        return 0;
    return Height(B->Left) - Height(B->Right);
}

tree deleteCourse(tree root, String courseCode) { // this function delete node from the tree

    // STEP 1: BASE CASE
    if (root == NULL)
        return root;

    // If the key to be deleted is smaller than the root's key, then it lies in left subtree
    if (strcmp(courseCode, root->CourseCode) < 0)
        root->Left = deleteCourse(root->Left, courseCode);

    // If the key to be deleted is greater than the root's key, then it lies in right subtree
    if (strcmp(courseCode, root->CourseCode) > 0)
        root->Right = deleteCourse(root->Right, courseCode);

        // if key is same as root's key, then This is the node to be deleted

    else if (strcmp(courseCode, root->CourseCode) == 0) {
        // node with only one child or no child
        if ((root->Left == NULL) || (root->Right == NULL)) {
            tree temp;
            //        temp = root->Left ? root->Left : root->Right;

            if(root->Left == NULL){
                temp = root->Right;
            } else{
                temp = root->Left;
            }

            // No child case
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else  // One child case
            {
                *root = *temp; // Copy the contents of

            }
            // the non-empty child
            free(temp);
        }
        else {

            // node with two children: Get the inorder successor (smallest in the right subtree)
            tree temp = FindMin(root->Right);

            // Copy the inorder successor's data to this node
            strcpy(root->CourseName,temp->CourseName);
            root->CreditHours = temp->CreditHours;
            strcpy(root->CourseCode,temp->CourseCode);
            strcpy(root->Department,temp->Department);
            strcpy(root->Topic,temp->Topic);

            // Delete the inorder successor
            root->Right = deleteCourse(root->Right, temp->CourseCode);
        }
    }
    // If the tree had only one node then return
    if (root == NULL)
        return root;

    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
    root->Height = 1 + Max(Height(root->Left), Height(root->Right));
    // STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to
    // check whether this node became unbalanced)
    int balance = getBalance(root);

    // If this node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balance > 1 && getBalance(root->Left) >= 0)
        return SingleRotateRight(root);

    // Left Right Case
    if (balance > 1 && getBalance(root->Left) < 0) {
        root->Left = SingleRotateLeft(root->Left);
        return SingleRotateRight(root);
    }

    // Right Right Case
    if (balance < -1 && getBalance(root->Right) <= 0)
        return SingleRotateLeft(root);

    // Right Left Case
    if (balance < -1 && getBalance(root->Right) > 0) {
        root->Right = SingleRotateRight(root->Right);
        return SingleRotateLeft(root);
    }
    return root;
}

void getDepartment( tree T,String department) // this function will get all courses that have the same department and store it to array
{
    if( T != NULL)
    {
        getDepartment( T->Left ,department);
        if (strcmp(department, T->Department) == 0){
            strcpy(courses[courseNum++],T->CourseCode);
        }
        getDepartment( T->Right ,department);
    }
}
void PrintOutToFile(tree T, FILE *fileOut) { //print data out to file

    if (T != NULL) {

        PrintOutToFile(T->Left,fileOut);
        fprintf(fileOut, "%s:", T->CourseName);
        fprintf(fileOut, "%d,", T->CreditHours);
        fprintf(fileOut, "%s,", T->CourseCode);
        fprintf(fileOut, "%s/", T->Department);
        fprintf(fileOut, "%s.\n", T->Topic);

        PrintOutToFile(T->Right,fileOut);
    }
}