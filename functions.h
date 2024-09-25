#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXTASKLEN 500

struct task
{
    char taskName[MAXTASKLEN]; //name of task
    int startMonth; //stores start month of task
    int endMonth; //stores last month of task
    int numOfDependencies; //stores number of dependencies
    int dependencies[10]; //stores which tasks the task is dependent on

} task[10];


enum month {January, February, March, April, May, June, July, August, September, October, November, December};

void printGantt(FILE *fptr);
void printLine(FILE *fptr);
void createGantt(struct task *task);
void printTemplate(FILE *fptr);
void printSchedule(FILE *fptr, int startMonth, int endMonth, int numOfDependencies, int dependencies[], char taskName[]);
void editGantt(int tasksNum, struct task *task);
int getTaskID(int tasksNum, struct task *testTask);
int printDependentTasks(struct task *testTask, int visitedTasks[], int taskID);
void checkifCircular(struct task *testTask, int taskID, int visitedTasks[]);
