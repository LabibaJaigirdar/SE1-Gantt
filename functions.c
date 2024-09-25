#include "functions.h"

//function to print gantt chart from file
void printGantt(FILE *fptr)
{
    char c;

    if (fptr == NULL)
    {
        printf("Cannot open file.");
        return;
    }

    do
    {
        c = fgetc(fptr);
        printf("%c", c);
    }
    while (c != EOF);

}

//function to print the template of a gantt (i.e. the top row)
void printTemplate(FILE *fptr)
{
    fprintf(fptr, "\n");

    printLine(fptr);

    fprintf(fptr, "Tasks%28s|%3sJan%3s|%3sFeb%3s|%3sMar%3s|%3sApr%3s|%3sMay%3s|%3sJun%3s|%3sJul%3s|%3sAug%3s|%3sSept%3s|%3sOct%3s|%3sNov%3s|%3sDec%3s|%2sDependencies%2s|\n"," ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ");

    printLine(fptr);
}

//function to print a line of dashes
void printLine(FILE *fptr)
{

    for (int i = 0; i < 172; i++)
    {
        fprintf(fptr, "-");
    }

    fprintf(fptr, "\n");
}

//function to fill out gantt with task names, months, and dependencies
void printSchedule(FILE *fptr, int startMonth, int endMonth, int numOfDependencies, int dependencies[], char taskName[])
{
    int month[12];
    int status = 0;
    int i;

    fprintf(fptr, "%s", taskName);
    for (i = 0; i < 33 - strlen(taskName); i++)
    {
        fprintf(fptr, "%s", " ");
    }

    //set during which months the task takes place
    for (i = 0; i < 12; i++)
    {
        if (i + 1 == startMonth)
        {
            status = 1;
        }
        if (i == endMonth)
        {
            status = 0;
        }

        month[i] = status;
    }

    for (i = 0; i < 13; i ++)
    {

        //print months to file
        if(month[i] == 1 && i != September)
        {
            fprintf(fptr, "|%3sXXX%3s", " ", " ");
        }

        if(month[i] == 1 && i == September)
        {
            fprintf(fptr, "|%3sXXX%4s", " ", " ");
        }

        if(month[i] == 0 && i != September)
        {
            fprintf(fptr, "|%9s", " ");
        }

        if(month[i] == 0 && i == September)
        {
            fprintf(fptr, "|%10s", " ");
        }
    }

    //print no. of dependencies to file
    if(numOfDependencies == 1)
    {
        fprintf(fptr, "|%7s%d%8s|\n", " ", dependencies[0], " ");
    }

    if (numOfDependencies == 0)
    {
        fprintf(fptr, "|%16s|\n", " ");
    }

    if(numOfDependencies > 1)
    {

        fprintf(fptr, "|");

        for (i = 0; i < (16 - (2 * numOfDependencies)) / 2; i++)
        {
            fprintf(fptr, "%s", " ");
        }

        for(i = 0; i < numOfDependencies; i++)
        {
            fprintf(fptr, "%d ", dependencies[i]);
        }

        for (i = 0; i < (16 - (2 * numOfDependencies)) / 2; i++)
        {
            fprintf(fptr, "%s", " ");
        }

        fprintf(fptr, "|\n");
    }
}

//function to take in user input of task name, start and end months, and dependencies for creating a gantt
void createGantt(struct task *task)
{
    int i;

    //get task name
    puts("\nPlease enter the task name:");

    fgets(task->taskName, MAXTASKLEN, stdin);

    //remove trailing newline
    task->taskName[strlen(task->taskName) - 1] = '\0';

    //get start month
    do
    {
        puts("\nPlease enter start month:");
        scanf("%d", &task->startMonth);

        if(task->startMonth > 12 || task->startMonth < 1)
        {
            puts("Invalid month. Please enter a value between 1 and 12.");
        }

    }
    while(task->startMonth < 1 || task->startMonth > 12);

    //get end month
    do
    {
        puts("\nPlease enter end month:");
        scanf("%d", &task->endMonth);

        if(task->endMonth > 12 || task->endMonth < 1)
        {
            puts("Invalid month. Please enter a value between 1 and 12.");
        }

    }
    while(task->endMonth < 1 || task->endMonth > 12);

    //get number of dependencies
    do
    {
        puts("\nEnter how many dependencies this task has:");
        scanf("%d", &task->numOfDependencies);

        if(task->numOfDependencies < 0 || task->numOfDependencies > 10)
        {
            puts("Invalid number of dependencies. Please enter a value between 0 and 10.");
        }
    }
    while (task->numOfDependencies < 0 || task->numOfDependencies > 10);

    //get dependendent tasks
    if (task->numOfDependencies > 0)
    {
        for (i = 0; i < task->numOfDependencies; i++)
        {
            puts("\nEnter dependent task:");
            scanf("%d", &task->dependencies[i]);
        }
    }
}

//function to edit an existing gantt chart
void editGantt(int tasksNum, struct task *task)
{

    char editTask[MAXTASKLEN];
    char input[5];
    FILE *editptr;
    int editTaskNum;
    int i;
    int editMatch = 1;
    int visitedTasks[10] = {0};

    while ((getchar()) != '\n');

    //enter task to be edited
    do
    {

        puts("\nPlease enter the exact name of the task you wish to change:");
        fgets(editTask, MAXTASKLEN, stdin);
        editTask[strlen(editTask) - 1] = '\0';

        //find the task to be edited
        for(i = 0; i < tasksNum; i++)
        {

            if (strcmp(task[i].taskName, editTask) == 0)
            {
                editTaskNum = i;
                editMatch = 0;
                break;
            }
        }

        //error message if the task name entered does not match any of the tasks
        if(editMatch)
        {
            puts("No task found. Please enter the exact name of the task you wish to change");
        }

    }
    while(editMatch);

    //calls function to get input for the new task name, start and end months, and dependencies
    createGantt(&task[editTaskNum]);

    editptr = fopen("createGantt.txt", "w");

    //print new gantt chart with the edited task
    printTemplate(editptr);

    for(i = 0; i < tasksNum; i++)
    {
        printSchedule(editptr, task[i].startMonth, task[i].endMonth, task[i].numOfDependencies, task[i].dependencies, task[i].taskName);
        printLine(editptr);
    }

    fclose(editptr);


    editptr = fopen("createGantt.txt", "r");

    if (editptr == NULL)
    {
        puts("\nCannot open file.");
        return;
    }

//clear screen
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    printGantt(editptr);

    //ask user if they would like to edit again, test, or exit
    puts("\nIf you wish to edit the gantt again please type 'edit' / If you wish to run a test, type 'test' / or to exit type 'quit' and then press enter to execute your option.");

    do
    {
        scanf("%s", input);

        if (strcmp(input, "Edit") != 0 && strcmp(input, "edit") != 0 && strcmp(input, "Test") != 0 && strcmp(input, "test") != 0 && strcmp(input, "Quit") != 0 && strcmp(input, "quit") != 0)
        {
            puts("\nInvalid input. If you wish to edit the gantt please type 'edit' / If you wish to run a test, type 'test' / or to exit type 'quit' and then press enter to execute your option.\n");
        }

    }
    while (strcmp(input, "Edit") != 0 && strcmp(input, "edit") != 0 && strcmp(input, "Test") != 0 && strcmp(input, "test") != 0 && strcmp(input, "Quit") != 0 && strcmp(input, "quit") != 0);

    if(strcmp(input, "Quit") == 0 || strcmp(input, "quit") == 0)
    {
        fclose(editptr);
        return;
    }

    if(strcmp(input, "Edit") == 0 || strcmp(input, "edit") == 0)
    {
        editGantt(tasksNum, task);
    }

    if(strcmp(input, "Test") == 0 || strcmp(input, "test") == 0)
    {

        //get index of task to test
        while ((getchar()) != '\n');
        int taskID = getTaskID(tasksNum, task);

        //test for potential circular dependencies

        int circular = printDependentTasks(task, visitedTasks, taskID);

        if(circular == 0)
        {
            puts("\nNo circular dependency found.");
        }
    }

}

int getTaskID(int tasksNum, struct task *testTask)
{

    char taskName[MAXTASKLEN];
    int taskMatch = 1;
    int taskID;

    do
    {

        puts("\nPlease enter the exact name of the task you wish to test for a circular dependency:");
        fgets(taskName, MAXTASKLEN, stdin);
        taskName[strlen(taskName) - 1] = '\0';

        //find the task to be edited
        for(int i = 0; i < tasksNum; i++)
        {

            if (strcmp(testTask[i].taskName, taskName) == 0)
            {
                taskID = i;
                taskMatch = 0;
            }
        }

        //error message if the task name entered does not match any of the tasks
        if(taskMatch)
        {
            puts("No task found. Please enter the exact name of the task you wish to test for a circular dependency");
        }

    }
    while(taskMatch);

    return taskID;
}

int printDependentTasks(struct task *testTask, int visitedTasks[], int taskID)
{
    int dependentTaskID;
    int i;
    int circularFound = 0;

    //print the current task
    printf("%d -> ", taskID + 1);

    //set the current task as having been visited
    visitedTasks[taskID] = 1;

    //go through each dependency of the tassk
    for(i = 0; i < testTask[taskID].numOfDependencies; i++)
    {
        //assigns the index of the dependency to dependentTaskID
        dependentTaskID = testTask[taskID].dependencies[i] - 1;

        //recursive call if the task has not been visited before
        if (visitedTasks[dependentTaskID] == 0)
        {
            printDependentTasks(testTask, visitedTasks, dependentTaskID);
        }

        //otherwise, a circular dependency has been found
        else
        {
            puts("\nCircular dependency found!");
            circularFound = 1;
        }

    }

    //when guard breaks, set the final task in the path as not having been visited
    visitedTasks[taskID] = 0;
    return circularFound;
}


