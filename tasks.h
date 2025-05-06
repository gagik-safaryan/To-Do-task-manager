#ifndef TASKS_H
#define TASKS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { false, true } bool;

typedef struct Task {
    unsigned short int task_id;
    char task_name[100];
    char task_description[800];
    bool status;
    struct Task *next;
} Task;

int Menu();
int GetLatestId(FILE *fp);
void SearchTask(Task **head,const char *keyword);
void Configuration(FILE *fp);
void AddTask(Task **startpnt, FILE *fp, int i);
void DeleteTask(Task **pnt, int id);
void UpdateFile(Task *startpnt, FILE *fp);
void LoadTasks(Task **head, FILE *fp);
void MarkTask(Task **head,int id);
void ViewTasks(Task **head);
void SortTasks(Task *head,const char *keyword);

#endif