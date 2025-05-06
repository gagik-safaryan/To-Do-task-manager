#include "tasks.h"

int Menu() {
    int selection;
    printf("1) View my tasks\n");
    printf("2) Add new task\n");
    printf("3) Delete task\n");
    printf("4) Search\n");
    printf("5) Mark task by id\n");
    printf("6) Exit\n");
    scanf("%i", &selection);
    return selection;
}

int GetLatestId(FILE *fp) {
    char line[1024];
    int latest_id = -1;

    // Skip header
    if (fgets(line, sizeof(line), fp) == NULL) {
        return 0;
    }

    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '\n' || line[0] == '\0') continue;
        int id;
        if (sscanf(line, "%d,", &id) == 1 && id > latest_id)
            latest_id = id;
    }
    return latest_id == -1 ? 0 : latest_id + 1;
}



void Configuration(FILE *fp) {
    //fprintf(fp, "TaskID,Name,Description,Status\n");
}

void AddTask(Task **startpnt, FILE *fp, int i) {
    Task *new_task = (Task *)malloc(sizeof(Task));
    if (!new_task) {
        perror("Failed to allocate memory");
        return;
    }
    new_task->task_id = i;

    printf("\nPlease enter the task name: ");
    getchar();
    fgets(new_task->task_name, sizeof(new_task->task_name), stdin);
    new_task->task_name[strcspn(new_task->task_name, "\n")] = '\0';

    printf("\nPlease enter the task description: ");
    fgets(new_task->task_description, sizeof(new_task->task_description), stdin);
    new_task->task_description[strcspn(new_task->task_description, "\n")] = '\0';


    new_task->status = false;
    new_task->next = *startpnt;
    *startpnt = new_task;


    printf("\nThe task was successfully added to the list");
    FILE *rewrite_fp = fopen("save.csv", "w");
    if (!rewrite_fp) {
        perror("Failed to open file for rewriting");
        return;
    }
    UpdateFile(*startpnt, rewrite_fp);
    fclose(rewrite_fp);
}

void DeleteTask(Task **pnt, int id) {
    if (*pnt == NULL) {
        printf("You have no tasks to delete.\n");
        return;
    }

    Task *temp = *pnt;
    Task *prev = NULL;

    while (temp != NULL && temp->task_id != id) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("There is no such task.\n");
        return;
    }

    if (prev == NULL) {
        *pnt = temp->next;
    } else {
        prev->next = temp->next;
    }
    free(temp);
    printf("\nTask with ID %d deleted.\n", id);

    FILE *rewrite_fp = fopen("save.csv", "w");
    if (!rewrite_fp) {
        perror("Failed to open file for rewriting");
        return;
    }

    UpdateFile(*pnt, rewrite_fp);
    fclose(rewrite_fp);
}

void UpdateFile(Task *startpnt, FILE *fp) {
    Task *temp = startpnt;
    fprintf(fp, "TaskID,Name,Description,Status\n");
    while (temp != NULL) {
        fprintf(fp, "%d,%s,%s,%s\n",
                temp->task_id,
                temp->task_name,
                temp->task_description,
                temp->status ? "Complete" : "Incomplete");
        temp = temp->next;
    }
}

void LoadTasks(Task **head, FILE *fp) {
    char line[2048];
    Task *last = NULL;

    rewind(fp);
    if (fgets(line, sizeof(line), fp) == NULL) {
        return;
    }

    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '\n') continue;
        Task *new_task = (Task *)malloc(sizeof(Task));
        if (!new_task) {
            perror("Memory allocation failed");
            return;
        }
        char status_str[50];
        int ret = sscanf(line, "%hu,%99[^,],%799[^,],%49[^\n]",
                         &new_task->task_id,
                         new_task->task_name,
                         new_task->task_description,
                         status_str);
        if (ret != 4)
        {
           fprintf(stderr, "Error reading line: %s", line);
           free(new_task);
           continue;
        }

        new_task->status = (strcmp(status_str, "Complete") == 0);
        new_task->next = NULL;

        if (*head == NULL) {
            *head = new_task;
        } else {
            last->next = new_task;
        }
        last = new_task;
    }
}



void SearchTask(Task **head,const char *keyword)
{
	printf("SearchTask called with head: %p\n", head);
    Task *current = *head;
    bool found = false;

    if (head == NULL) {
        printf("No tasks to search.\n");
        return;
    }

    printf("Task ID: %d\n",current->task_id);

    printf("\nSearching for tasks containing keyword: \"%s\"\n", keyword);
    printf("----------------------------------------------------\n");

    while (current != NULL) {
        // Use strstr to perform a case-insensitive search
        if (strstr(current->task_name, keyword) != NULL || strstr(current->task_description, keyword) != NULL) 
        {
            printf("Task ID: %d\n", current->task_id);
            printf("Name: %s\n", current->task_name);
            printf("Description: %s\n", current->task_description);
            printf("Status: %s\n", current->status ? "Complete" : "Incomplete");
            printf("----------------------------------------------------\n");
            found = true;
        }
        current = current->next;
    }

    if (!found) {
        printf("No tasks found containing the keyword \"%s\".\n", keyword);
    }
}

void ViewTasks(Task **head)
{
	Task *current = *head;
	while(current != NULL)
	{
		printf("Task ID: %d\n", current->task_id);
        printf("Name: %s\n", current->task_name);
        printf("Description: %s\n", current->task_description);
        printf("Status: %s\n", current->status ? "Complete" : "Incomplete");
        printf("----------------------------------------------------\n");
        current = current->next;
	}
}

void MarkTask(Task **head,int id)
{
    Task *current = *head;
    bool notFound = true;

    if (head == NULL) {
        printf("No task with such ID.\n");
        return;
    }

    while (notFound) {
        if(current->task_id == id)
        	{
        		notFound = false;
        		current->status = !(current->status);
        	}
        current = current->next;
    }

    FILE *rewrite_fp = fopen("save.csv", "w");
    if (!rewrite_fp) {
        perror("Failed to open file for rewriting");
        return;
    }

    UpdateFile(*head, rewrite_fp);
    fclose(rewrite_fp);
}