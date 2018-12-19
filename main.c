#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX 100

int max_person_tasks;

typedef struct date_struct{
    int day;
    int mounth;
    int year;
}Date;

typedef struct task_struct *Task;
typedef struct task_struct{
    struct card_struct{
        int priority;
        int id_c;
        char content[MAX];
        Date creation;
        Date deadline;
        Date conclusion;
        int step;
    }Card;
    struct person_struct *owner;
    Task next_task;
}task_node;

typedef struct person_tasks_struct *Person_Tasks;
typedef struct person_tasks_struct{
    Task info_person_tasks;
    Person_Tasks next_person_tasks;
}person_tasks_node;

typedef struct person_struct *Person;
typedef struct person_struct{
    struct worker_struct{
        char name[MAX];
        char email[MAX];
        int id_w;
    }Worker;
    Person_Tasks assignment;
    Person next_person;
}person_node;

typedef struct to_do_struct *ToDo;
typedef struct to_do_struct{
    Task info_todo;
    ToDo next_todo;
}todo_node;

typedef struct doing_struct *Doing;
typedef struct doing_struct{
    Task info_doing;
    Doing next_doing;
}doing_node;

typedef struct done_struct *Done;
typedef struct done_struct{
    Task info_done;
    Done next_done;
}done_node;

int check_input_number(int *number)
{
    if (scanf("%d",number)==1)
        return 0;
    else{
        printf("Wrong format!\n");
        printf(">>>");
        fflush(stdin);
        check_input_number(number);
    }
    return 0;
}

int verify_id(int id_number,Task list_task){

    while(list_task!=NULL&&list_task->Card.id_c!=id_number){
        list_task=list_task->next_task;
    }
    if(list_task==NULL)
        return 0;
    else if(list_task->Card.id_c==id_number)
        return 1;

    return 0;
}

int check_date_after(Date old_date,Date new_date){

    if(new_date.year>old_date.year){
        return 0;
    }
    if(new_date.year==old_date.year){
        if(new_date.mounth>old_date.mounth)
            return 0;
        if (new_date.mounth==old_date.mounth){
            if(new_date.day>old_date.day){
                return 0;
            }
            if(new_date.day==old_date.day){
                return 0;
            }else
                return 1;
        }else
            return 1;
    }else
        return 1;

    return 1;
}

int check_number_of_days(Date old_date,Date new_date){

    Date temp=new_date;
    int days=1;

    while((old_date.year!=temp.year) || (old_date.mounth!=temp.mounth) || (old_date.day!=temp.day)){

        if(temp.year>old_date.year){
            temp.year=temp.year-1;
            days=days+365;
        }
        if(temp.mounth>old_date.mounth){
            temp.mounth=temp.mounth-1;
            if (temp.mounth ==(4 || 6 || 9 || 11))
                days=days+30;
            else if (temp.mounth == 2)
                days=days+28;
            else if (temp.mounth == (1||3||5||7||8||10||12))
                days=days+31;
        }
        else if(temp.mounth<old_date.mounth){
            temp.mounth=temp.mounth+1;
            if (temp.mounth ==(4 || 6 || 9 || 11))
                days=days-30;
            else if (temp.mounth == 2)
                days=days-28;
            else if (temp.mounth == (1||3||5||7||8||10||12))
                days=days-31;
        }
        if(temp.day>old_date.day){
            temp.day=temp.day-1;
            days=days+1;
        }
        else if(temp.day<old_date.day){
            temp.day=temp.day+1;
            days=days-1;
        }
    }

    return days;
}

int check_current_date(Date date_to_check){

    time_t rawtime = time(NULL);
    struct tm *ptm = localtime(&rawtime);

    if(rawtime == -1){
        puts("The time() function failed");
        return 1;
    }

    if(ptm == NULL){
        puts("The localtime() function failed");
        return 1;
    }


    if(date_to_check.day==ptm->tm_mday&&date_to_check.mounth==(1+ptm->tm_mon)&&date_to_check.year==(ptm->tm_year+1900))
        return 0;
    else{
        return 1;
    }

    return 1;
}

int check_date(Date date_to_check){

    if ((date_to_check.day<1||date_to_check.day>31) || (date_to_check.mounth <1||date_to_check.mounth>12) || (date_to_check.year<2018)){
        return 1;
    }else{
        switch (date_to_check.mounth){
            case 2:
                if (date_to_check.day>29)
                    return 1;
                else if (date_to_check.day==29){
                    if (date_to_check.year%4==0 && (date_to_check.year%100==0 && date_to_check.year%400==0))
                        return 0;
                    else
                        return 1;
                }
                break;
            case 4:
                if (date_to_check.day>30)
                    return 1;
                break;
            case 6:
                if (date_to_check.day>30)
                    return 1;
                break;
            case 9:
                if (date_to_check.day>30)
                    return 1;
                break;
            case 11:
                if (date_to_check.day>30)
                    return 1;
                break;
        }
    }
    return 0;
}

Person insert_node_person_tasks(Person aux_person, Task *aux_task){

    Person_Tasks new_node_person_tasks, aux_person_tasks;

    if(aux_person->assignment==NULL){
        new_node_person_tasks=malloc(sizeof(person_tasks_node));
        new_node_person_tasks->info_person_tasks=*aux_task;
        new_node_person_tasks->next_person_tasks=NULL;
        aux_person->assignment=new_node_person_tasks;
        aux_person_tasks=aux_person->assignment;
    }else{
        aux_person_tasks=aux_person->assignment;
        while(aux_person_tasks!=NULL)
            aux_person_tasks=aux_person_tasks->next_person_tasks;

        new_node_person_tasks=malloc(sizeof(person_tasks_node));
        new_node_person_tasks->info_person_tasks=*aux_task;
        new_node_person_tasks->next_person_tasks=aux_person->assignment;
        aux_person->assignment=new_node_person_tasks;
    }

    return aux_person;
}

Task insert_node_task_orderly_by_date(Task order_list_by_date, Task new_node){

    Task previous=NULL,aux_task=order_list_by_date;

    while((aux_task!=NULL)&&((aux_task->Card.creation.year>new_node->Card.creation.year)||
    ((aux_task->Card.creation.year==new_node->Card.creation.year)&&(aux_task->Card.creation.mounth>new_node->Card.creation.mounth))||
    ((aux_task->Card.creation.year==new_node->Card.creation.year)&&((aux_task->Card.creation.mounth==new_node->Card.creation.mounth)&&(aux_task->Card.creation.day>new_node->Card.creation.day))))){
        previous=aux_task;
        aux_task=aux_task->next_task;
    }

    new_node->next_task=aux_task;

    if(previous!=NULL)
        previous->next_task=new_node;
    else
        order_list_by_date=new_node;


    return order_list_by_date;
}

Task order_task_list_by_date(Task list_task){

    Task order_list_by_date=NULL, aux_task=list_task;

    while(aux_task!=NULL){
        list_task=aux_task->next_task;
        order_list_by_date=insert_node_task_orderly_by_date(order_list_by_date,aux_task);
        aux_task=list_task;
    }

    return order_list_by_date;
}

Done insert_node_done_orderly_by_date(Done order_list_by_date, Done new_node){

    Done previous=NULL,aux_done=order_list_by_date;

    while((aux_done!=NULL)&&((aux_done->info_done->Card.conclusion.year>new_node->info_done->Card.conclusion.year)||
    ((aux_done->info_done->Card.conclusion.year==new_node->info_done->Card.conclusion.year)&&(aux_done->info_done->Card.conclusion.mounth>new_node->info_done->Card.conclusion.mounth))||
    ((aux_done->info_done->Card.conclusion.year==new_node->info_done->Card.conclusion.year)&&((aux_done->info_done->Card.conclusion.mounth==new_node->info_done->Card.conclusion.mounth)&&(aux_done->info_done->Card.conclusion.day>new_node->info_done->Card.conclusion.day))))){
        previous=aux_done;
        aux_done=aux_done->next_done;
    }

    new_node->next_done=aux_done;

    if(previous!=NULL)
        previous->next_done=new_node;
    else
        order_list_by_date=new_node;


    return order_list_by_date;
}

Done order_done_list_by_date(Done list_done){

    Done order_list_by_date=NULL, aux_done=list_done;

    while(aux_done!=NULL){
        list_done=aux_done->next_done;
        order_list_by_date=insert_node_done_orderly_by_date(order_list_by_date,aux_done);
        aux_done=list_done;
    }

    return order_list_by_date;
}

ToDo insert_node_todo_orderly_by_date(ToDo order_list_by_date, ToDo new_node){

    ToDo previous=NULL,aux_todo=order_list_by_date;

    while((aux_todo!=NULL)&&((aux_todo->info_todo->Card.creation.year>new_node->info_todo->Card.creation.year)||
    ((aux_todo->info_todo->Card.creation.year==new_node->info_todo->Card.creation.year)&&(aux_todo->info_todo->Card.creation.mounth>new_node->info_todo->Card.creation.mounth))||
    ((aux_todo->info_todo->Card.creation.year==new_node->info_todo->Card.creation.year)&&((aux_todo->info_todo->Card.creation.mounth==new_node->info_todo->Card.creation.mounth)&&(aux_todo->info_todo->Card.creation.day>new_node->info_todo->Card.creation.day))))){
        previous=aux_todo;
        aux_todo=aux_todo->next_todo;
    }

    new_node->next_todo=aux_todo;

    if(previous!=NULL)
        previous->next_todo=new_node;
    else
        order_list_by_date=new_node;


    return order_list_by_date;
}

ToDo order_todo_list_by_date(ToDo list_todo){

    ToDo order_list_by_date=NULL, aux_todo=list_todo;

    while(aux_todo!=NULL){
        list_todo=aux_todo->next_todo;
        order_list_by_date=insert_node_todo_orderly_by_date(order_list_by_date,aux_todo);
        aux_todo=list_todo;
    }

    return order_list_by_date;
}

ToDo insert_node_todo_orderly_by_priority(ToDo order_list_by_priority, ToDo new_node){

    ToDo previous=NULL,aux_todo=order_list_by_priority;

    while((aux_todo!=NULL)&&((aux_todo->info_todo->Card.priority)>(new_node->info_todo->Card.priority)||(aux_todo->info_todo->Card.priority)==(new_node->info_todo->Card.priority))){
        previous=aux_todo;
        aux_todo=aux_todo->next_todo;
    }

    new_node->next_todo=aux_todo;
    if(previous!=NULL)
        previous->next_todo=new_node;
    else
        order_list_by_priority=new_node;


    return order_list_by_priority;
}

ToDo order_todo_list_by_priority(ToDo list_todo){

    ToDo order_list_by_priority=NULL, aux_todo=list_todo;

    aux_todo=list_todo;

    while(aux_todo!=NULL){
        list_todo=aux_todo->next_todo;
        order_list_by_priority=insert_node_todo_orderly_by_priority(order_list_by_priority,aux_todo);
        aux_todo=list_todo;
    }

    return order_list_by_priority;
}

Doing insert_node_doing_orderly_by_name(Doing order_list_by_name, Doing new_node){

    Doing previous=NULL,aux_doing=order_list_by_name;

    while((aux_doing!=NULL)&&(strcmp(aux_doing->info_doing->owner->Worker.name,new_node->info_doing->owner->Worker.name)<0)){
        previous=aux_doing;
        aux_doing=aux_doing->next_doing;
    }

    new_node->next_doing=aux_doing;
    if(previous!=NULL)
        previous->next_doing=new_node;
    else
        order_list_by_name=new_node;


    return order_list_by_name;
}

Doing order_doing_list_by_name(Doing list_doing){

    Doing order_list_by_name=NULL, aux_doing=list_doing;

    aux_doing=list_doing;

    while(aux_doing!=NULL){
        list_doing=aux_doing->next_doing;
        order_list_by_name=insert_node_doing_orderly_by_name(order_list_by_name,aux_doing);
        aux_doing=list_doing;
    }

    return order_list_by_name;
}

Doing move_from_todo_to_doing(ToDo *list_todo, Person *list_person, Doing list_doing, Task *list_task){

    Doing new_node, aux_doing=list_doing;
    ToDo aux_todo=*list_todo;
    Person aux_person=*list_person;
    Person_Tasks aux_person_tasks;
    ToDo previous_node_todo=NULL;
    Task aux_task=*list_task;
    int card_id,worker_id,d1,d2, tasks_in_doing=0, number_of_person_tasks=0;
    Date date_to_check;
    int days_to_check=8;

    if (*list_todo==NULL){
        printf("There are no tasks in To Do!\n");
        printf("\n");
    }else{

        printf("What is the I.D. of Card?\n");
        check_input_number(&card_id);

        printf("What is the I.D. of the Worker?\n");
        check_input_number(&worker_id);

        while(aux_todo->next_todo!=NULL&&card_id!=aux_todo->info_todo->Card.id_c){
            previous_node_todo=aux_todo;
            aux_todo=aux_todo->next_todo;
        }

        while(aux_person->next_person!=NULL&&worker_id!=aux_person->Worker.id_w){
            aux_person=aux_person->next_person;
        }

        while(aux_task->next_task!=NULL&&card_id!=aux_task->Card.id_c){
            aux_task=aux_task->next_task;
        }

        if(card_id!=aux_todo->info_todo->Card.id_c)
            printf("There is no Card with this I.D. in To Do!\n\n");

        if(worker_id!=aux_person->Worker.id_w)
            printf("There is no the Worker with this I.D.!\n\n");

        if(card_id==aux_todo->info_todo->Card.id_c&&worker_id==aux_person->Worker.id_w){

            while(aux_doing){
                aux_doing=aux_doing->next_doing;
                tasks_in_doing++;
            }

            if(tasks_in_doing>5){
                printf("The Doing column is already filled!\n");
                return list_doing;
            }
            else{

                aux_person_tasks=aux_person->assignment;

                while(aux_person_tasks!=NULL){
                    number_of_person_tasks++;
                    aux_person_tasks=aux_person_tasks->next_person_tasks;

                }


                if(number_of_person_tasks<max_person_tasks||max_person_tasks==0){

                    printf("Deadline to conclusion of the Task:");
                    scanf("%d/%d/%d", &date_to_check.day, &date_to_check.mounth, &date_to_check.year);
                    fflush(stdin);
                    d1=check_date(date_to_check);
                    d2=check_date_after(aux_todo->info_todo->Card.creation,date_to_check);

                    while(d1||d2){
                        printf("Wrong date!\n");
                        printf(">>>");
                        scanf("%d/%d/%d", &date_to_check.day, &date_to_check.mounth, &date_to_check.year);
                        fflush(stdin);
                        d1=check_date(date_to_check);
                        d2=check_date_after(aux_todo->info_todo->Card.creation,date_to_check);
                    }

                    aux_person_tasks=aux_person->assignment;

                    if(aux_person_tasks!=NULL){
                        while(aux_person_tasks!=NULL&&days_to_check>=7){
                            days_to_check=check_number_of_days(aux_person_tasks->info_person_tasks->Card.deadline,date_to_check);
                            if(days_to_check>=7)
                                aux_person_tasks=aux_person_tasks->next_person_tasks;
                        }
                    }else{
                        days_to_check=check_number_of_days(aux_person_tasks->info_person_tasks->Card.deadline,date_to_check);
                    }

                    if(days_to_check>=7){

                        if(aux_todo!=NULL){
                            new_node=(Doing)malloc(sizeof(doing_node));
                            new_node->next_doing=list_doing;
                            list_doing=new_node;

                            list_doing->info_doing=aux_todo->info_todo;
                            list_doing->info_doing->Card.step=2;

                            list_doing->info_doing->Card.creation=aux_todo->info_todo->Card.creation;
                            list_doing->info_doing->Card.deadline=date_to_check;

                            aux_task->owner=aux_person;


                            insert_node_person_tasks(aux_person, &aux_task);


                            if(aux_todo->next_todo!=NULL&&previous_node_todo!=NULL){
                                previous_node_todo->next_todo=aux_todo->next_todo;
                                *list_todo=previous_node_todo;
                                free(aux_todo);
                            }
                            else if (aux_todo->next_todo!=NULL&&previous_node_todo==NULL){
                                *list_todo=aux_todo->next_todo;
                                free(aux_todo);
                            }
                            else if(aux_todo->next_todo==NULL&&previous_node_todo!=NULL){
                                previous_node_todo->next_todo=NULL;
                                free(aux_todo);
                            }else if(aux_todo->next_todo==NULL&&previous_node_todo==NULL){
                                free(aux_todo);
                                *list_todo=NULL;
                            }
                            else if(previous_node_todo==NULL){
                                free(aux_todo);
                                *list_todo=NULL;
                            }
                        }

                    }else
                        printf("This person has already reached the task limit per week!\n");
                }else
                    printf("This person has already reached the task limit!\n");
            }
        }
        printf("\n");
    }

    return list_doing;
}

ToDo move_from_doing_to_todo(Doing *list_doing, ToDo list_todo){

    ToDo new_node;
    Doing aux_doing=*list_doing;
    Doing previous=NULL;
    int id;

    if(list_doing==NULL){
        printf("There are no tasks in Doing!\n");
        printf("\n");
    }else{
        printf("What is the I.D. of Card?\n");
        check_input_number(&id);

        while(aux_doing->next_doing!=NULL&&id!=aux_doing->info_doing->Card.id_c){
            previous=aux_doing;
            aux_doing=aux_doing->next_doing;
        }

        if(id!=aux_doing->info_doing->Card.id_c)
            printf("There is no Card with this I.D. in Doing!\n\n");
        else{
            if(aux_doing!=NULL){

                new_node=(ToDo)malloc(sizeof(doing_node));
                new_node->next_todo=list_todo;
                list_todo=new_node;

                list_todo->info_todo=aux_doing->info_doing;
                list_todo->info_todo->Card.step=1;

                if(aux_doing->next_doing!=NULL&&previous!=NULL){
                    previous->next_doing=aux_doing->next_doing;
                    *list_doing=previous;
                    free(aux_doing);

                }
                else if (aux_doing->next_doing!=NULL&&previous==NULL){
                    *list_doing=aux_doing->next_doing;
                    free(aux_doing);
                }
                else if(aux_doing->next_doing==NULL&&previous!=NULL){
                    previous->next_doing=NULL;
                    free(aux_doing);
                }
                else if(aux_doing->next_doing==NULL&&previous==NULL){
                    free(aux_doing);
                    *list_doing=NULL;
                }
                else if(previous==NULL){
                    free(aux_doing);
                    *list_doing=NULL;
                }
            }
            printf("\n");
        }
    }

    return list_todo;
}

Done move_from_doing_to_done(Doing *list_doing, Done list_done){

    Done new_node;
    Doing aux_doing= *list_doing;
    Doing previous=NULL;
    int id,d1,d2;

    if (list_doing==NULL){
        printf("There are no tasks in Doing!\n");
        printf("\n");
    }else{
        printf("What is the I.D. of Card?\n");
        check_input_number(&id);

        while(aux_doing->next_doing!=NULL&&id!=aux_doing->info_doing->Card.id_c){
            previous=aux_doing;
            aux_doing=aux_doing->next_doing;
        }

        if (id!=aux_doing->info_doing->Card.id_c)
            printf("There is no Card with this I.D. in Doing\n\n");
        else{
            if(aux_doing!=NULL){

                new_node=(Done)malloc(sizeof(done_node));
                new_node->next_done=list_done;
                list_done=new_node;


                list_done->info_done=aux_doing->info_doing;
                list_done->info_done->Card.step=3;


                if(aux_doing->next_doing!=NULL&&previous!=NULL){
                    previous->next_doing=aux_doing->next_doing;
                    *list_doing=previous;
                    free(aux_doing);
                }
                else if (aux_doing->next_doing!=NULL&&previous==NULL){
                    *list_doing=aux_doing->next_doing;
                    free(aux_doing);
                }
                else if(aux_doing->next_doing==NULL&&previous!=NULL){
                    previous->next_doing=NULL;
                    free(aux_doing);
                }
                else if(aux_doing->next_doing==NULL&&previous==NULL){
                    free(aux_doing);
                    *list_doing=NULL;
                }
                else if(previous==NULL){
                    free(aux_doing);
                    *list_doing=NULL;
                }


                printf("Date of conclusion of the Task:");
                scanf("%d/%d/%d", &new_node->info_done->Card.conclusion.day, &new_node->info_done->Card.conclusion.mounth, &new_node->info_done->Card.conclusion.year);
                fflush(stdin);
                d1=check_date(new_node->info_done->Card.conclusion);
                d2=check_date_after(new_node->info_done->Card.creation,new_node->info_done->Card.conclusion);

                while(d1||d2){
                    printf("Wrong date!\n");
                    printf(">>>");
                    scanf("%d/%d/%d", &new_node->info_done->Card.conclusion.day, &new_node->info_done->Card.conclusion.mounth, &new_node->info_done->Card.conclusion.year);
                    fflush(stdin);
                    d1=check_date(new_node->info_done->Card.conclusion);
                    d2=check_date_after(new_node->info_done->Card.creation,new_node->info_done->Card.conclusion);
                }
            }
            printf("\n");
        }
    }
    return list_done;
}

ToDo move_from_done_to_todo(Done *list_done, ToDo list_todo){

    ToDo new_node;
    Done aux_done=*list_done;
    Done previous=NULL;
    int id;

    if (list_done==NULL){
        printf("There are no tasks in Done!\n");
        printf("\n");
    }else{
        printf("What is I.D. of Card?\n");
        check_input_number(&id);

        while(aux_done->next_done!=NULL&& id!=aux_done->info_done->Card.id_c){
            previous=aux_done;
            aux_done=aux_done->next_done;
        }
        if (id!=aux_done->info_done->Card.id_c)
            printf("There is no Card with this I.D. in Done\n\n");
        else{
            if(aux_done!=NULL){

                new_node=(ToDo)malloc(sizeof(todo_node));
                new_node->next_todo=list_todo;
                list_todo=new_node;

                list_todo->info_todo=aux_done->info_done;
                list_todo->info_todo->Card.step=1;
                new_node->info_todo->Card.deadline.day=0;
                new_node->info_todo->Card.deadline.mounth=0;
                new_node->info_todo->Card.deadline.year=0;
                new_node->info_todo->Card.conclusion.day=0;
                new_node->info_todo->Card.conclusion.mounth=0;
                new_node->info_todo->Card.conclusion.year=0;

                if (aux_done->next_done!=NULL&&previous!=NULL){
                    previous->next_done=aux_done->next_done;
                    *list_done=previous;
                    free(aux_done);
                }
                else if (aux_done->next_done!=NULL&&previous==NULL){
                    *list_done=aux_done->next_done;
                    free(aux_done);
                }
                else if(aux_done->next_done==NULL&&previous!=NULL){
                    previous->next_done=NULL;
                    free(aux_done);
                }
                else if(aux_done->next_done==NULL&&previous==NULL){
                    free(aux_done);
                    *list_done=NULL;
                }
                else if(previous==NULL){
                    free(aux_done);
                    *list_done=NULL;
                }
            }
            printf("\n");
        }
    }

    return list_todo;
}

Task delete_task(Task list_task, int *id_to_delete){

    Task previous=NULL, current=list_task;
    int id=*id_to_delete;

    if(list_task==NULL){
        printf("Empty list!\n\n");
        return list_task;
    }

    if((current->next_task==NULL)&&(current->Card.id_c==id)){
        free(current);
        list_task=NULL;
        return list_task;
    }else if((current->next_task==NULL)&&(current->Card.id_c!=id)){
        printf("The task with I.D. %d doesn't exist!\n\n", id);
        return list_task;
    }

    while((current!=NULL)&&((current->Card.id_c)!=id)){
        previous=current;
        current=current->next_task;
        if(previous->next_task==NULL){
            printf("The task with I.D. %d doesn't exist!\n\n", id);
            return list_task;
        }
    }

    if(current->next_task!=NULL){
        if(previous==NULL)
            list_task=current->next_task;
        else
            previous->next_task=current->next_task;
    }
    else
        previous->next_task=NULL;

    free(current);

    return list_task;
}

ToDo delete_task_of_todo(ToDo list_todo, int *id_to_delete){

    ToDo previous=NULL, current=list_todo;
    int id=*id_to_delete;

    if(list_todo==NULL){
        return list_todo;
    }

    if((current->next_todo==NULL)&&(current->info_todo->Card.id_c==id)){
        free(current);
        list_todo=NULL;
        return list_todo;
    }else if((current->next_todo==NULL)&&(current->info_todo->Card.id_c!=id))
        return list_todo;


    while((current!=NULL)&&((current->info_todo->Card.id_c)!=id)){
        previous=current;
        current=current->next_todo;
        if(previous->next_todo==NULL)
            return list_todo;
    }

    if(current->next_todo!=NULL){
        if(previous==NULL)
            list_todo=current->next_todo;
        else
            previous->next_todo=current->next_todo;
    }
    else
        previous->next_todo=NULL;

    free(current);

    return list_todo;
}

Doing delete_task_of_doing(Doing list_doing, int *id_to_delete){

    Doing previous=NULL, current=list_doing;
    int id=*id_to_delete;

    if(list_doing==NULL)
        return list_doing;

    if((current->next_doing==NULL)&&(current->info_doing->Card.id_c==id)){
        free(current);
        list_doing=NULL;
        return list_doing;
    }else if((current->next_doing==NULL)&&(current->info_doing->Card.id_c!=id))
        return list_doing;


    while((current!=NULL)&&((current->info_doing->Card.id_c)!=id)){
        previous=current;
        current=current->next_doing;
        if(previous->next_doing==NULL)
            return list_doing;
    }

    if(current->next_doing!=NULL){
        if(previous==NULL)
            list_doing=current->next_doing;
        else
            previous->next_doing=current->next_doing;
    }
    else
        previous->next_doing=NULL;

    free(current);

    return list_doing;
}

Done delete_task_of_done(Done list_done, int *id_to_delete){

    Done previous=NULL, current=list_done;
    int id=*id_to_delete;

    if(list_done==NULL)
        return list_done;

    if((current->next_done==NULL)&&(current->info_done->Card.id_c==id)){
        free(current);
        list_done=NULL;
        return list_done;
    }else if((current->next_done==NULL)&&(current->info_done->Card.id_c!=id))
        return list_done;


    while((current!=NULL)&&((current->info_done->Card.id_c)!=id)){
        previous=current;
        current=current->next_done;
        if(previous->next_done==NULL)
            return list_done;
    }

    if(current->next_done!=NULL){
        if(previous==NULL)
            list_done=current->next_done;
        else
            previous->next_done=current->next_done;
    }
    else
        previous->next_done=NULL;

    free(current);

    return list_done;
}

Task create_task(Task list_task){

    Task new_task;
    int d1, d2, check;

    new_task=(Task)malloc(sizeof(task_node));

    fflush(stdin);
    printf("Describe the Task (max.100 letters):");
    scanf("%[^\n]s", new_task->Card.content);
    fflush(stdin);

    printf("I.D. of the Task:");
    check_input_number(&new_task->Card.id_c);

    check=verify_id(new_task->Card.id_c,list_task);
    while(check){
        printf("I.D. already assigned to another Card\n");
        printf(">>>");
        check_input_number(&new_task->Card.id_c);
        check=verify_id(new_task->Card.id_c,list_task);
    }

    printf("Date of creation:");
    scanf("%d/%d/%d", &new_task->Card.creation.day, &new_task->Card.creation.mounth, &new_task->Card.creation.year);
    fflush(stdin);

    d1=check_date(new_task->Card.creation);
    d2=check_current_date(new_task->Card.creation);
    while(d1||d2){
        printf("Wrong date!\n");
        printf(">>>");
        scanf("%d/%d/%d", &new_task->Card.creation.day, &new_task->Card.creation.mounth, &new_task->Card.creation.year);
        fflush(stdin);
        d1=check_date(new_task->Card.creation);
        d2=check_current_date(new_task->Card.creation);
    }

    printf("Priority(1-10):");
    check_input_number(&new_task->Card.priority);
    printf("\n");

    while(new_task->Card.priority<1||new_task->Card.priority>10){
        check_input_number(&new_task->Card.priority);
    }

    new_task->Card.deadline.day=0;
    new_task->Card.deadline.mounth=0;
    new_task->Card.deadline.year=0;
    new_task->Card.conclusion.day=0;
    new_task->Card.conclusion.mounth=0;
    new_task->Card.conclusion.year=0;
    new_task->Card.step=1;

    new_task->next_task=list_task;
    list_task=new_task;

    return list_task;
}

void performance_statistics(Person list_person){

    Person aux_person=list_person;
    Person_Tasks aux_person_tasks;
    int done_in_time=0, not_done_in_time=0, date, number_of_tasks=0;
    float performance;


    while(aux_person){

        aux_person_tasks=aux_person->assignment;

        if(aux_person_tasks){
            if(aux_person_tasks->info_person_tasks->Card.step==3){

                while(aux_person_tasks){

                    date=check_date_after(aux_person_tasks->info_person_tasks->Card.deadline,aux_person_tasks->info_person_tasks->Card.conclusion);

                    if(date)
                        done_in_time++;
                    else
                        not_done_in_time++;

                    number_of_tasks++;
                    aux_person_tasks=aux_person_tasks->next_person_tasks;
                }

                printf("%s\n",aux_person->Worker.name);
                printf("Tasks done in time: %d\n", done_in_time);
                printf("Tasks not done in time: %d\n", not_done_in_time);
                performance=((float)done_in_time/number_of_tasks)*100;
                printf("Performance: %.1f%% in time\n", performance);
                printf("\n");
            }
        }
        aux_person=aux_person->next_person;
    }

}

void print_list_task(Task list_task){

    if(list_task->Card.step==1){
        printf("Task: %s\n", list_task->Card.content);
        printf("I.D. of the task: %d\n", list_task->Card.id_c);
        printf("Date of creation: %d/%d/%d\n", list_task->Card.creation.day, list_task->Card.creation.mounth, list_task->Card.creation.year);
        printf("Priority(1-10): %d\n", list_task->Card.priority);
    }
    else if(list_task->Card.step==2){
        printf("Task: %s\n", list_task->Card.content);
        printf("I.D. of the task: %d\n", list_task->Card.id_c);
        printf("Deadline: %d/%d/%d\n", list_task->Card.deadline.day, list_task->Card.deadline.mounth, list_task->Card.deadline.year);
        printf("Person in charge: %s\n", list_task->owner->Worker.name);
    }
    else if(list_task->Card.step==3){
        printf("Task: %s\n", list_task->Card.content);
        printf("I.D. of the task: %d\n", list_task->Card.id_c);
        printf("Deadline: %d/%d/%d\n", list_task->Card.deadline.day, list_task->Card.deadline.mounth, list_task->Card.deadline.year);
        printf("Date of conclusion: %d/%d/%d\n", list_task->Card.conclusion.day, list_task->Card.conclusion.mounth, list_task->Card.conclusion.year);
        printf("Person who did the Task: %s\n", list_task->owner->Worker.name);
    }
    printf("\n");

}

void print_list_person(Person list_person){

    Person aux_person=list_person;

    if(aux_person==NULL)
        printf("Empty list!\n\n");
    else{
        while(aux_person){
            printf("Name: %s\n", aux_person->Worker.name);
            printf("Email: %s\n", aux_person->Worker.email);
            printf("ID: %d\n\n", aux_person->Worker.id_w);
            aux_person=aux_person->next_person;
        }
    }
}

void print_board (ToDo list_todo,Doing list_doing,Done list_done){

    ToDo aux_todo=list_todo;
    Doing aux_doing=list_doing;
    Done aux_done=list_done;

    printf("--------------- TO DO ---------------\n");
    if (aux_todo==NULL){
        printf("\n");
        printf("There are no cards in To Do\n");
        }

    printf("\n");
    while(aux_todo){
        print_list_task(aux_todo->info_todo);
        aux_todo=aux_todo->next_todo;
    }

    printf("--------------- Doing ---------------\n");
    if (aux_doing==NULL){
        printf("\n");
        printf("There are no cards in Doing\n");
        }

    printf("\n");
    while(aux_doing){
        print_list_task(aux_doing->info_doing);
        aux_doing=aux_doing->next_doing;
    }

    printf("--------------- Done ----------------\n");
    if (aux_done==NULL){
        printf("\n");
        printf("There are no cards in Done\n");
        }

    printf("\n");
    while(aux_done){
        print_list_task(aux_done->info_done);
        aux_done=aux_done->next_done;
    }
}

void write_in_file(Task list_task, FILE *fp){

    if(list_task->Card.step==1){
        fprintf(fp,"Task: %s\n", list_task->Card.content);
        fprintf(fp,"I.D. of the task: %d\n", list_task->Card.id_c);
        fprintf(fp,"Date of creation: %d/%d/%d\n", list_task->Card.creation.day, list_task->Card.creation.mounth, list_task->Card.creation.year);
        fprintf(fp,"Priority(1-10): %d\n\n", list_task->Card.priority);
    }
    else if(list_task->Card.step==2){
        fprintf(fp,"Task: %s\n", list_task->Card.content);
        fprintf(fp,"I.D. of the task: %d\n", list_task->Card.id_c);
        fprintf(fp,"Deadline: %d/%d/%d\n", list_task->Card.deadline.day, list_task->Card.deadline.mounth, list_task->Card.deadline.year);
        fprintf(fp,"Person in charge: %s\n\n", list_task->owner->Worker.name);
    }
    else if(list_task->Card.step==3){
        fprintf(fp,"Task: %s\n", list_task->Card.content);
        fprintf(fp,"I.D. of the task: %d\n", list_task->Card.id_c);
        fprintf(fp,"Deadline: %d/%d/%d\n", list_task->Card.deadline.day, list_task->Card.deadline.mounth, list_task->Card.deadline.year);
        fprintf(fp,"Date of conclusion: %d/%d/%d\n", list_task->Card.conclusion.day, list_task->Card.conclusion.mounth, list_task->Card.conclusion.year);
        fprintf(fp,"Person who did the Task: %s\n\n", list_task->owner->Worker.name);
    }

}

void write_file(char name_of_file[], Task list_task, ToDo list_todo, Doing list_doing, Done list_done){

    FILE *fp=fopen(name_of_file, "w");
    ToDo aux_todo=list_todo;
    Doing aux_doing=list_doing;
    Done aux_done=list_done;

    fprintf(fp,"------------------------- KANBAIN BOARD -------------------------\n");

    fprintf(fp,"--------------- TO DO ---------------\n");
    if (aux_todo==NULL){
        fprintf(fp,"\n");
        fprintf(fp,"There are no cards in To Do\n");
    }

    fprintf(fp,"\n");
    while(aux_todo){
        write_in_file(aux_todo->info_todo,fp);
        aux_todo=aux_todo->next_todo;
    }

    fprintf(fp,"--------------- Doing ---------------\n");
    if (aux_doing==NULL){
        fprintf(fp,"\n");
        fprintf(fp,"There are no cards in Doing\n");
    }

    fprintf(fp,"\n");
    while(aux_doing){
        write_in_file(aux_doing->info_doing, fp);
        aux_doing=aux_doing->next_doing;
    }

    fprintf(fp,"--------------- Done ----------------\n");
    if (aux_done==NULL){
        fprintf(fp,"\n");
        fprintf(fp,"There are no cards in Done\n");
    }

    fprintf(fp,"\n");
    while(aux_done){
        write_in_file(aux_done->info_done, fp);
        aux_done=aux_done->next_done;
    }
    fprintf(fp,"-----------------------------------------------------------------\n");

    fclose(fp);
}

void save_to_new_file(Task list_task, ToDo list_todo, Doing list_doing, Done list_done){

    int choice;
    char name_of_file[20];

    printf("Do you want to save the Kanban Board to a new file?\n");
    printf("Yes - press 1 and enter\n");
    printf("No - press 0 and enter\n");
    check_input_number(&choice);

    while(choice!=1&&choice!=0){
        fflush(stdin);
        printf("Wrong number!\n");
        printf(">>>");
        check_input_number(&choice);
    }

    if(choice==1){
        fflush(stdin);
        printf("\nChoose a name to the file:");
        scanf("%[^\n]s", name_of_file);
        fflush(stdin);
        write_file(name_of_file, list_task, list_todo, list_doing, list_done);
    }

}

void main_menu(Task list_task, Person list_person, ToDo list_todo, Doing list_doing, Done list_done){

    int opt,end=1, id_to_delete;
    ToDo new_node;
    Task aux_task;
    Person aux_person=list_person;
    Person_Tasks aux_person_tasks;
    FILE *fp;
    int card_id, new_priority, worker_id;

    while(end==1){
        printf("MAIN MENU\n");
        printf("1:Create Task\n");
        printf("2:List tasks\n");
        printf("3:List people\n");
        printf("4:Print Kanban Board\n");
        printf("5:Move card from To Do to Doing\n");
        printf("6:Move card from Doing to To Do\n");
        printf("7:Move card from Doing to Done\n");
        printf("8:Move card from Done to To Do\n");
        printf("9:Delete task\n");
        printf("10:Define the maximum number of people's tasks\n");
        printf("11:Change the priority of a task\n");
        printf("12:List tasks of one person\n");
        printf("13:Person's performance statistics\n");
        printf("14:Exit\n");
        printf("Option:");
        check_input_number(&opt);
        printf("\n");

        while(opt>14){
            printf("Write a valid option!\n");
            printf("Option:");
            scanf("%d", &opt);
            printf("\n");
        }

        if(opt==1){
            list_task=create_task(list_task);
            new_node=(ToDo)malloc(sizeof(todo_node));
            new_node->info_todo=list_task;
            new_node->next_todo=list_todo;
            list_todo=new_node;
        }
        if(opt==2){
            if(list_task!=NULL){
                list_task=order_task_list_by_date(list_task);
                aux_task=list_task;
                while(aux_task){
                    print_list_task(aux_task);
                    aux_task=aux_task->next_task;
                }
            }else
                printf("The Task list is empty!\n\n");


        }
        if(opt==3){
            if(list_person!=NULL)
                 print_list_person(list_person);
            else
                printf("The Persons list is empty!\n\n");

        }
        if(opt==4){
            list_todo=order_todo_list_by_date(list_todo);
            list_todo=order_todo_list_by_priority(list_todo);
            list_doing=order_doing_list_by_name(list_doing);
            list_done=order_done_list_by_date(list_done);
            print_board(list_todo,list_doing,list_done);
        }
        if(opt==5){
            list_doing=move_from_todo_to_doing(&list_todo,&list_person,list_doing,&list_task);
        }
        if(opt==6){
            list_todo=move_from_doing_to_todo(&list_doing, list_todo);
        }
        if(opt==7){
            list_done=move_from_doing_to_done(&list_doing,list_done);
        }
        if(opt==8){
            list_todo=move_from_done_to_todo(&list_done, list_todo);
        }
        if(opt==9){
            printf("What is the I.D. number of the card you want to delete?\n");
            printf("\n");

            aux_task=list_task;
            while(aux_task){
                print_list_task(aux_task);
                aux_task=aux_task->next_task;
            }
            aux_task=list_task;

            printf("I.D.:");
            check_input_number(&id_to_delete);
            printf("\n");

            list_todo=delete_task_of_todo(list_todo, &id_to_delete);
            list_doing=delete_task_of_doing(list_doing, &id_to_delete);
            list_done=delete_task_of_done(list_done, &id_to_delete);
            list_task=delete_task(list_task, &id_to_delete);
        }
        if(opt==10){
            printf("Maximum number of tasks each person can have:");
            check_input_number(&max_person_tasks);
            printf("\n");

        }
        if(opt==11){
            list_task=order_task_list_by_date(list_task);

            aux_task=list_task;
            while(aux_task){
                print_list_task(aux_task);
                aux_task=aux_task->next_task;
            }
            aux_task=list_task;

            printf("What is the I.D. of Card?\n");
            check_input_number(&card_id);

            while(aux_task->next_task!=NULL&&card_id!=aux_task->Card.id_c){
                    aux_task=aux_task->next_task;
            }

            if(card_id!=aux_task->Card.id_c)
                printf("There is no Card with this I.D. in task list!\n");
            else{
                printf("What is the new priority for the card with the I.D. %d?\n", card_id);
                check_input_number(&new_priority);
                aux_task->Card.priority=new_priority;
            }
            printf("\n");
        }
        if(opt==12){
            printf("Who is the person you want to choose to see their tasks?\n");
            printf("\n");
            print_list_person(list_person);

            printf("Person I.D.:");
            check_input_number(&worker_id);
            printf("\n");


            while(aux_person->next_person!=NULL&&worker_id!=aux_person->Worker.id_w){
                aux_person=aux_person->next_person;
            }

            if(worker_id!=aux_person->Worker.id_w)
                printf("There is no Worker with this I.D.!\n");
            else{
                aux_person_tasks=aux_person->assignment;

                if(aux_person_tasks==NULL){
                    printf("This person has no tasks!\n");
                    printf("\n");
                }

                while(aux_person_tasks){
                    print_list_task(aux_person_tasks->info_person_tasks);
                    aux_person_tasks=aux_person_tasks->next_person_tasks;
                }
                aux_person_tasks=aux_person->assignment;
            }

        }
        if(opt==13){
            performance_statistics(list_person);
        }
        if(opt==14){
            end=0;

            fp=fopen("tasks.txt", "w");

            aux_task=list_task;

            while(aux_task!=NULL){
                fprintf(fp,"Priority: %d\nID: %d\nContent: %s\nDeadline date: %d/%d/%d\nCreation date: %d/%d/%d\nConclusion date: %d/%d/%d\nStep: %d\n",
                aux_task->Card.priority,aux_task->Card.id_c,
                aux_task->Card.content,
                aux_task->Card.deadline.day,aux_task->Card.deadline.mounth,aux_task->Card.deadline.year,
                aux_task->Card.creation.day,aux_task->Card.creation.mounth,aux_task->Card.creation.year,
                aux_task->Card.conclusion.day,aux_task->Card.conclusion.mounth,aux_task->Card.conclusion.year,
                aux_task->Card.step);
                if(aux_task->owner!=NULL){
                    fprintf(fp,"Responsible: %d\n",aux_task->owner->Worker.id_w);
                }else
                    fprintf(fp,"Responsible: 0\n");

                fprintf(fp,"\n");
                aux_task=aux_task->next_task;
            }
            fclose(fp);

            save_to_new_file(list_task, list_todo, list_doing, list_done);
        }
    }
}

void  insert_tasks_in_lists_by_step(Task list_task, ToDo *list_todo, Doing *list_doing, Done *list_done){

    Task aux_task=list_task;
    ToDo new_node_todo;
    Doing new_node_doing;
    Done new_node_done;

    while(aux_task){
        if(aux_task->Card.step==1){
            new_node_todo=(ToDo)malloc(sizeof(todo_node));
            new_node_todo->info_todo=aux_task;
            new_node_todo->next_todo=*list_todo;
            *list_todo=new_node_todo;
        }
        else if(aux_task->Card.step==2){
            new_node_doing=(Doing)malloc(sizeof(doing_node));
            new_node_doing->info_doing=aux_task;
            new_node_doing->next_doing=*list_doing;
            *list_doing=new_node_doing;
        }
        else if(aux_task->Card.step==3){
            new_node_done=(Done)malloc(sizeof(done_node));
            new_node_done->info_done=aux_task;
            new_node_done->next_done=*list_done;
            *list_done=new_node_done;
        }
        aux_task=aux_task->next_task;
    }
}

Task load_task_file(Task list_task, Person *list_person){

    FILE *fp;
    Task new_task;
    Person aux_person=*list_person;
    char junk[50];
    long file_size;
    int responsible;

    fp=fopen("tasks.txt", "r");

    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);

    if(file_size==0){
        fclose(fp);
        return NULL;
    }

    fseek(fp, 0, SEEK_SET);

    while(!feof(fp)){

        new_task=(Task)malloc(sizeof(task_node));

        fscanf(fp,"%s",junk);
        if(feof(fp))
            return list_task;
        fscanf(fp,"%d",&new_task->Card.priority);
        fscanf(fp,"%s",junk);
        fscanf(fp,"%d",&new_task->Card.id_c);
        fscanf(fp,"%s",junk);
        fscanf(fp," %[^\n]s",new_task->Card.content);
        fscanf(fp,"%s",junk);
        fscanf(fp,"%s",junk);
        fscanf(fp,"%d/%d/%d",&new_task->Card.deadline.day,&new_task->Card.deadline.mounth, &new_task->Card.deadline.year);
        fscanf(fp,"%s",junk);
        fscanf(fp,"%s",junk);
        fscanf(fp,"%d/%d/%d",&new_task->Card.creation.day,&new_task->Card.creation.mounth, &new_task->Card.creation.year);
        fscanf(fp,"%s",junk);
        fscanf(fp,"%s",junk);
        fscanf(fp,"%d/%d/%d",&new_task->Card.conclusion.day,&new_task->Card.conclusion.mounth, &new_task->Card.conclusion.year);
        fscanf(fp,"%s",junk);
        fscanf(fp,"%d",&new_task->Card.step);
        fscanf(fp,"%s",junk);
        fscanf(fp,"%d",&responsible);
        fscanf(fp,"%[^\n]s",junk);


        if(list_task==NULL){
            new_task->next_task=NULL;
            list_task=new_task;

            if(responsible!=0){

                while(aux_person->next_person!=NULL&&responsible!=aux_person->Worker.id_w){
                    aux_person=aux_person->next_person;
                }


                if(responsible==aux_person->Worker.id_w){

                    insert_node_person_tasks(aux_person, &new_task);
                    new_task->owner=aux_person;
                }

                aux_person=*list_person;
            }else
                new_task->owner=NULL;
        }else{
            if(responsible!=0){

                while(aux_person->next_person!=NULL&&responsible!=aux_person->Worker.id_w){
                    aux_person=aux_person->next_person;
                }

                if(responsible==aux_person->Worker.id_w){

                    insert_node_person_tasks(aux_person, &new_task);
                    new_task->owner=aux_person;
                }
            }else
                new_task->owner=NULL;

            new_task->next_task=list_task;
            list_task=new_task;
        }
        aux_person=*list_person;
    }

    fclose(fp);

    return list_task;
}

Person load_person_file(Person list_person){

    FILE *fp;
    Person new_person, aux_person;
    char junk[50];
    long file_size;

    fp=fopen("agents.txt", "r");

    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);

    if(file_size==0){
        fclose(fp);
        return NULL;
    }

    fseek(fp, 0, SEEK_SET);

    while(!feof(fp)){

        new_person=(Person)malloc(sizeof(person_node));

        fscanf(fp,"%s",junk);
        if(feof(fp))
            return list_person;
        fscanf(fp," %[^\n]s",new_person->Worker.name);
        fscanf(fp,"%s",junk);
        fscanf(fp," %[^\n]s",new_person->Worker.email);
        fscanf(fp,"%s",junk);
        fscanf(fp,"%d",&new_person->Worker.id_w);
        fscanf(fp,"%[^\n]s",junk);


        if(list_person==NULL){
            new_person->next_person=NULL;
            list_person=new_person;
            aux_person=list_person;
            aux_person->assignment=NULL;

        }else{
            while(aux_person->next_person!=NULL)
                aux_person=aux_person->next_person;

            aux_person->next_person=new_person;
            new_person->next_person=NULL;
            aux_person=aux_person->next_person;
            aux_person->assignment=NULL;
        }

    }

    aux_person=list_person;

    fclose(fp);

    return list_person;
}

void free_all_lists(Person *list_person,Task *list_task,ToDo *list_todo,Doing *list_doing,Done *list_done){

    Person previous_person, aux_person=*list_person;
    Task previous_task, aux_task=*list_task;
    ToDo previous_todo, aux_todo=*list_todo;
    Doing previous_doing, aux_doing=*list_doing;
    Done  previous_done, aux_done=*list_done;
    Person_Tasks previous_person_tasks, aux_person_tasks;

    while(aux_person!=NULL){

        aux_person_tasks=aux_person->assignment;

        while(aux_person_tasks!=NULL){
            previous_person_tasks=aux_person_tasks;
            aux_person_tasks=aux_person_tasks->next_person_tasks;
            aux_person->assignment=aux_person_tasks;
            free(previous_person_tasks);
        }

        previous_person=aux_person;
        aux_person=aux_person->next_person;
        *list_person=aux_person;
        free(previous_person);
    }

    while(aux_task!=NULL){
        previous_task=aux_task;
        aux_task=aux_task->next_task;

        *list_task=aux_task;
        free(previous_task);
    }

    while(aux_todo!=NULL){
        previous_todo=aux_todo;
        aux_todo=aux_todo->next_todo;
        *list_todo=aux_todo;
        free(previous_todo);
    }

    while(aux_doing!=NULL){
        previous_doing=aux_doing;
        aux_doing=aux_doing->next_doing;
        *list_doing=aux_doing;
        free(previous_doing);
    }

    while(aux_done!=NULL){
        previous_done=aux_done;
        aux_done=aux_done->next_done;
        *list_done=aux_done;
        free(previous_done);
    }

}

int main()
{
    Task list_task=NULL;
    Person list_person=NULL;
    ToDo list_todo=NULL;
    Doing list_doing=NULL;
    Done list_done=NULL;

    max_person_tasks=0;

    list_person=load_person_file(list_person);
    list_task=load_task_file(list_task, &list_person);
    insert_tasks_in_lists_by_step(list_task, &list_todo, &list_doing, &list_done);

    main_menu(list_task, list_person, list_todo, list_doing, list_done);

    free_all_lists(&list_person, &list_task, &list_todo, &list_doing, &list_done);

    return 0;
}
