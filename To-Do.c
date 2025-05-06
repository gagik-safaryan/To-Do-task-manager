#include <stdio.h>
#include "tasks.h"

#define MAX_LIMIT 20

char search[MAX_LIMIT];

int main()
{
    FILE *fp = fopen("save.csv","r");
    bool IsTrue = true;
    Task *tasks_list = NULL;
    int ids;

    if(fp)
    {
        printf("File exists, moving on.\n");
        fclose(fp);

        fp = fopen("save.csv","a+");
        if(!fp) {
            perror("Failed to open the file, exiting.\n");
            return 1;
        } else {
            printf("The file opened successfully, moving on.\n");
        }
        
        ids = GetLatestId(fp);
        LoadTasks(&tasks_list, fp);

    } else {

        printf("The file doesnt exit, creating new one.\n");
        fp = fopen("save.csv","a+");
        if(!fp)
        {
            perror("Failed to create new file, exiting.\n");
            return 1;
        }
        Configuration(fp);
        ids = 0;
    }

    printf("Latest id: %i\n",ids);
    int s,id;

    while(IsTrue)
    {
        printf("Welcome to your To-Do list\n");
        switch(Menu())
        {
            case 1:
                ViewTasks(&tasks_list);
                break;
            case 2:
                AddTask(&tasks_list, fp, ids);
                ids++;
                break;
            case 3:
                printf("Enter the id: \n");
                scanf("%i",&s);
                DeleteTask(&tasks_list,s);
                break;
            case 4:
                printf("Enter the keyword: \n");
                scanf("%s", search);
                printf("Value of tasks_list before SearchTask: %p\n", tasks_list);
                printf("Search keyword: %s\n", search);
                SearchTask(&tasks_list,search);
                break;
            case 5:
                printf("Enter the ID of the task you want to mark complete/incomplete: \n");
                scanf("%i",&id);
                MarkTask(&tasks_list,id);
                break;
            case 6:
                fclose(fp);
                IsTrue = false;
                break;
            default:
                printf("Invalid choice. Please try choosing another option.\n");
        }
    }
    return 0;
}
