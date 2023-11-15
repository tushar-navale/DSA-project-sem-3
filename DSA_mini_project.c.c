#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Deadline
{
    int date;
    int month;
} Deadline;

typedef struct Task
{
    char task[100];
    int prio;
    Deadline DL;
    struct Task *link;
} Task;

typedef struct Queue
{
    Task *front;
} Queue;

typedef struct Stack
{
    Task *top;
} Stack;

Queue *create_queue(Queue *ptr)
{
    ptr->front = NULL;
    return ptr;
}

Stack *create_stack(Stack *ptr)
{
    ptr->top = NULL;
    return ptr;
}

Task *create_task(char t[100])
{
    Task *temp = (Task *)malloc(sizeof(Task));
    strcpy(temp->task, t);
    temp->prio = 0;
    temp->DL.date = 0;
    temp->DL.month = 0;
    temp->link = NULL;
    return temp;
}


void enqueue_prio(Queue *ptr, char t[100], int p)
{
    Task *temp = create_task(t);
    temp->prio = p;

    if (ptr->front == NULL)
    {
        ptr->front = temp;
        return;
    }
    if (p <= ptr->front->prio)
    {
        temp->link = ptr->front;
        ptr->front = temp;
    }
    else
    {
        Task *cur = ptr->front;
        Task *prev = NULL;
        while (cur != NULL && p > cur->prio)
        {
            prev = cur;
            cur = cur->link;
        }
        prev->link = temp;
        temp->link = cur;
    }
}

void enqueue_DL(Queue *ptr, char t[100], int dd, int mm)
{
    Task *temp = create_task(t);
    temp->DL.date = dd;
    temp->DL.month = mm;

    if (ptr->front == NULL)
    {
        ptr->front = temp;
    }
    else if (mm < ptr->front->DL.month || (mm == ptr->front->DL.month && dd < ptr->front->DL.date))
    {
        temp->link = ptr->front;
        ptr->front = temp;
    }
    else
    {
        Task *cur = ptr->front;
        Task *prev = NULL;
        while (cur != NULL && (mm > cur->DL.month || (mm == cur->DL.month && dd > cur->DL.date)))
        {
            prev = cur;
            cur = cur->link;
        }
        prev->link = temp;
        temp->link = cur;
    }
}


void push(Stack *ptr, char t[])
{
    Task *temp = create_task(t);
    if (ptr->top == NULL)
    {
        ptr->top = temp;
    }
    else
    {
        Task *cur = ptr->top;
        while (cur->link != NULL)
        {
            cur = cur->link;
        }
        cur->link = temp;
    }
}

void delete_queue(Queue *ptr, char t[100])
{
    Task *cur = ptr->front;
    Task *prev = NULL;
    while (cur != NULL && strcmp(cur->task, t) != 0)
    {
        prev = cur;
        cur = cur->link;
    }
    if (cur != NULL)
    {
        if (prev == NULL)
        {
            ptr->front = cur->link;
        }
        else
        {
            prev->link = cur->link;
        }
        free(cur);
    }
}

void delete_que_prio(Queue *Qptr_prio, Queue *Qptr_DL, Stack *sptr)
{
    if (Qptr_prio->front == NULL)
    {
        printf("NO TASKS\n");
        return;
    }
    char task[100];
    strcpy(task, Qptr_prio->front->task);
    Task *cur = Qptr_prio->front;
    push(sptr, task);
    Qptr_prio->front = Qptr_prio->front->link;
    delete_queue(Qptr_DL, task);
    free(cur);
}

void delete_que_DL(Queue *Qptr_prio, Queue *Qptr_DL, Stack *sptr)
{
    if (Qptr_DL->front == NULL)
    {
        printf("NO TASKS\n");
        return;
    }
    char task[100];
    strcpy(task, Qptr_DL->front->task);
    Task *cur = Qptr_DL->front;
    push(sptr, task);
    Qptr_DL->front = Qptr_DL->front->link;
    delete_queue(Qptr_prio, task);
    free(cur);
}

void display_pending(Queue *Qptr_prio, Queue *Qptr_DL)
{
    if (Qptr_prio->front == NULL)
    {
        printf("NO PENDING TASKS\n");
        return;
    }
    Task *cur1 = Qptr_prio->front;
    Task *cur2 = Qptr_DL->front;
    printf("PENDING TASKS with PRIORITY: \n");
    while (cur1 != NULL)
    {
        printf("%s\n", cur1->task);
        cur1 = cur1->link;
    }
    printf("PENDING TASKS with DEADLINE: \n");
    while (cur2 != NULL)
    {
        printf("%s\n", cur2->task);
        cur2 = cur2->link;
    }
}

void display_completed(Stack *ptr)
{
    if (ptr->top == NULL)
    {
        printf("NO COMPLETED TASKS\n");
        return;
    }
    printf("COMPLETED TASKS:\n");
    Task *cur = ptr->top;
    while (cur != NULL)
    {
        printf("%s\n", cur->task);
        cur = cur->link;
    }
}

void change_date(Queue *Qptr_DL, char t[100], int new_dd, int new_mm) {
    delete_queue(Qptr_DL, t);
    enqueue_DL(Qptr_DL, t, new_dd, new_mm);
}

void change_priority(Queue *Qptr_prio, char t[100], int new_prio) {
    delete_queue(Qptr_prio, t);
    enqueue_prio(Qptr_prio, t, new_prio);
}


int main()
{
    Queue Qptr_prio;
    create_queue(&Qptr_prio);

    Queue Qptr_DL;
    create_queue(&Qptr_DL);

    Stack Sptr;
    create_stack(&Sptr);

    int choice;
    char task[100];
    int prio;
    int dd, mm;
    int new_dd, new_mm;
    int new_prio;

    while (1)
    {
        printf("\nYour Task Manager:\n");
        printf("Enter your choice: \n");
        printf("1. Enter a task\n");
        printf("Mark a task as complete\n");
        printf("2. By priority\n");
        printf("3. By Deadline\n");
        printf("4. View tasks\n");
        printf("5. Change task date\n");
        printf("6. Change task priority\n");
        printf("7. Quit\n");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Enter the task description: ");
            scanf(" %99[^\n]", task);
            printf("Enter the priority: ");
            scanf("%d", &prio);
            enqueue_prio(&Qptr_prio, task, prio);
            printf("Enter the date in dd/mm format: ");
            scanf("%d/%d", &dd, &mm);
            enqueue_DL(&Qptr_DL, task, dd, mm);
            break;
        case 2:
            delete_que_prio(&Qptr_prio, &Qptr_DL, &Sptr);
            break;
        case 3:
            delete_que_DL(&Qptr_prio, &Qptr_DL, &Sptr);
            break;
        case 4:
            display_pending(&Qptr_prio, &Qptr_DL);
            display_completed(&Sptr);
            break;

        case 5:
            printf("Enter the task description: ");
            scanf(" %99[^\n]", task);
            printf("Enter the new date in dd/mm format: ");
            scanf("%d/%d", &dd, &mm);
            change_date(&Qptr_DL, task, dd, mm);
            break;

        case 6:
            printf("Enter the task description: ");
            scanf(" %99[^\n]", task);
            printf("Enter the new priority: ");
            scanf("%d", &prio);
            change_priority(&Qptr_prio, task, prio);
            break;

            case 7:
                while (Qptr_prio.front != NULL)
                {
                    Task *temp = Qptr_prio.front;
                    Qptr_prio.front = Qptr_prio.front->link;
                    free(temp);
                }
                while (Qptr_DL.front != NULL)
            {
                Task *temp = Qptr_DL.front;
                Qptr_DL.front = Qptr_DL.front->link;
                free(temp);
            }
            while (Sptr.top != NULL)
            {
                Task *temp = Sptr.top;
                Sptr.top = Sptr.top->link;
                free(temp);
            }
            return 0;
                return 0;
            default:
                printf("Invalid choice\n");
        }
    }
}

