#include<stdio.h>
#include<stdlib.h>

struct node
{
  struct node *prev;
  int info;
  struct node *next;
};

struct node *start = NULL;

struct node *createcdll(struct node *);
struct node *printcdll(struct node *);
struct node *inscdllfirst(struct node *);
struct node *inscdllend(struct node *);
struct node *delcdlfirst(struct node *);
struct node *delcdlend(struct node *);
struct node *inscdllafgn(struct node *);


struct node *createcdll(struct node *start)
{
  struct node *new,*ptr;
  int item;

  printf("enter -1 to end circular linked list\n");
  printf("enter the item you want\n");
  scanf("%d",&item);
  while (item!=-1)
  {
    new = (struct node* )malloc(sizeof(struct node));
    new->info=item;

    if(start==NULL)
    {
      start=new;
      new->prev=start;
      new->next=start;

    }
    else
    {
      ptr=start;
      while(ptr->next!=start)
      {
        ptr=ptr->prev;
      }
      new->prev=ptr;
      ptr->next=new;
      new->next=start;
      start->prev=new;
    }
    scanf("%d",&item);
  }
  return start;
}

  struct node *printcdll(struct node *start)
  {
    struct node *ptr;
    ptr = start;
    if(start==NULL)
    {
      printf("the linked list is empty\n");
    }
    else
    {
    do
      {
    printf("%d  ",ptr->info);
    ptr=ptr->next;
  }while(ptr!=start);
    }
    return start;
  }

  struct node *inscdllfirst(struct node *start)
  {
    struct node *new,*ptr;
    int item;
    printf("enter the the item\n");
    scanf("%d",&item);

    ptr=start;
    new = (struct node*)malloc(sizeof(struct node));
    new->info=item;
    ptr=start->prev;
    ptr->next=new;
    new->prev=ptr;
    new->next=start;
    start->prev=new;
    start=new;
    start=printcdll(start);

    return start;

  }

struct node *inscdllend(struct node *start)
{
  struct node *ptr,*new;
  int item;

  printf("ENTER THE ELEMENT\n");
  scanf("%d",&item);

  ptr=start;
  new = (struct node*)malloc(sizeof(struct node *));
  new->info=item;
  while(ptr->next!=start)
  {
     ptr=ptr->next;
  }
  ptr->next=new;
  new->next=start;
  new->prev=ptr;
  start=printcdll(start);

return start;
}

struct node *delcdlfirst(struct node *start)
{
  struct node *ptr,*save;
  save = start;
  if(start==NULL)
  {
    printf("the linked list is empty");
  }
  else
  {
    ptr=start->prev;
    ptr->next=start->next;
    (start->next)->prev=ptr;
    start->prev=NULL;
    start=start->next;
    free(save);
   }
     start=printcdll(start);
     return start;
}

struct node *delcdlend(struct node *start)
{
  struct node *ptr;
  ptr=start;
	ptr=ptr->prev;
	(ptr->prev)->next=start;
	start->prev=ptr->prev;
	free(ptr);
	ptr=NULL;
   start=printcdll(start);
	return start;
}

struct node *inscdllafgn(struct node *start)
{
  struct node *ptr,*new;
  int value,item;
  printf("enter the value after which you want to enter the new element\n");
  scanf("%d",&value);
  printf("enter the new item");
  scanf("%d",&item);
  ptr=start;

  new = (struct node*)malloc(sizeof(struct node));
  new->info=item;
   while(ptr->info!=value)
   {
     ptr=ptr->next;
   }
   new->next=ptr->next;
   ptr->next=new;
   new->prev=ptr;
   ptr=new->next;
   ptr->prev=new;
   start=printcdll(start);
 return start;

}

int main()
{
  	int choice,g,an,y;

  do
  {

	printf("\nOptions:\n");
	printf("1.create the list\n");
  printf("2.print the list\n");
  printf("3.to enter element in the bigning\n");
  printf("4.to enter element in the end\n");
  printf("5.to delete the first element in the linkedlist\n");
  printf("6.to delete the last element in the linkedlist\n");
  printf("7.to enter a ite after the value you want\n");
  scanf("%d",&choice);

  switch(choice)
  {
    case 1:
    start = createcdll(start);
    break;
    case 2:
    start = printcdll(start);
    break;
    break;
    case 3:
    start = inscdllfirst(start);
    break;
    case 4:
    start = inscdllend(start);
    break;
    case 5:
    start = delcdlfirst(start);
    break;
    case 6:
    start = delcdlend(start);
    break;
    case 7:
    start = inscdllafgn(start);
    break;
    default:
    printf("ivalid input");

  }
  printf("\npress 0 if you want to exit\n");
  printf("OR enter 9 to continue\n");
  scanf("%d",&an);
   }while(an!= 0);

return 0;
}
