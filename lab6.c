#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LENGTH 20
#define ENTRY struct entry
#define LIST struct list

//declaration of all variables and functions
void insert(void);
void delete(void);
void show(void);
void fit_in(void);
int duplicate(char*);
void free_all(void);

//structure with each entry including name, size, and next pointer
struct entry{
	char name[LENGTH];
	int size;
	ENTRY *next;
};

//structure with head and tail pointer
struct list{
	ENTRY *head;
	ENTRY *tail;
};

LIST lists[4]={{NULL,NULL},{NULL,NULL},{NULL,NULL},{NULL,NULL}};

//main function that prints interface and runs case scenarios
int main(){
	int answer;

	while (1){
		//menu list
		printf("Restaurant Waiting List\n");
		printf("\tMenu\n");
		printf("1 - Insert\n");
		printf("2 - Delete\n");
		printf("3 - List\n");
		printf("4 - Search Size\n");
		printf("0 - Exit\n");
		
		scanf("%d", &answer);
		//case scenarios of user input
		switch(answer){
			case 1:
				insert();
				break;
			case 2:
				delete();
				break;
			case 3:
				show();
				break;
			case 4:
				fit_in();
				break;
			case 0:
				free_all();
				return 0;
			default:
				printf("Bad Input\n");
				break;
				
		}
	}
}

//insert function that inserts party and their size
void insert(void){
	ENTRY *p;
	char nametest[LENGTH];
	int i;
	int size;	

	//scans for name
	printf("Enter your name here: ");
	scanf("%s", nametest);
	
	//runs duplicate function and returns if name is duplicate
	if(duplicate(nametest)== 1){
		printf("You have entered a duplicate name\n");
		return;
	}
	//if not duplicate continues with function and name is copied into the structure

	//if statements checking for size
	else{
		if((p=(ENTRY *)malloc(sizeof(ENTRY)))==NULL){
			printf("malloc error\n");
			exit(1);
		}
		printf("Enter your party size here: ");
		scanf("%d", &size);
		if(size == 1 || size == 2){
			i=0;
		}
		else if(size == 3 || size == 4){
			i=1;
		}
		else if(size == 5 || size == 6){
			i=2;
		}
		else if(size > 6){
			i=3;
		}
		strcpy(p->name, nametest);
		p->size=size;
		//if list is empty
		if(lists[i].head == NULL && lists[i].tail == NULL){
			lists[i].head=p;
			lists[i].tail=p;
			p->next = NULL;
		}
		//if list has something in it already
		else{
			
			lists[i].tail->next=p;	
			p->next=NULL;
			lists[i].tail=p;
		}
		printf("You have created a reservation for %s, party size of %d\n", p->name, p->size);
	}
	return;
}

//delete function that deletes party when it fits the size and moves list up
void delete(void){
	ENTRY *p;
	ENTRY *q;
	int table;
	int found=0;
	
	//checks if no one is in the list
	if(lists[0].head == NULL && lists[1].head == NULL && lists[2].head == NULL && lists[3].head == NULL){
		printf("The list is currently empty, there is nothing to delete\n");
		return;
	}

	//runs if there is someone in the list
	printf("Enter the size of the table available: ");
	scanf("%d", &table);

	//runs a loop that looks through the list to see which of the oldest reservations can fit the table
	//seats parties and deletes reservations and shifts list up
	for(int i=0; i<4; i++){
		p = lists[i].head;
		q = lists[i].head;
		while(p != NULL){
			if(p->size<=table){
				printf("You have seated %s party size of %d\n", p->name, p->size);
				if(lists[i].head == lists[i].tail && p->next == NULL){
					lists[i].head = NULL;
					lists[i].tail = NULL;
					table-=p->size;
					found=1;
					free(p);
					p = NULL;
				}
				else if(lists[i].head == p){
					lists[i].head=p->next;
					table-=p->size;
					found=1;
					free(p);
					p=lists[i].head;
					q=p;
				}
				else if(lists[i].tail == p){
					lists[i].tail=q;
					table-=p->size;
					found=1;
					free(p);
					p=NULL;
					q->next=NULL;
					
				}
				else{
					q->next=p->next;
					table-=p->size;
					found=1;
					free(p);
					p=q->next;
				}
			}
			else{
				q=p;
				p=p->next;
			}
		}
	}
	//no reservation available if table size is bigger than size available
	if(found == 0){
		printf("No reservation available\n");
	}
	return;
}

//list function that prints out the current list
void show(void){
	ENTRY *p;
	//checks if there is no one in the list
	if(lists[0].head == NULL && lists[1].head == NULL && lists[2].head == NULL && lists[3].head == NULL){
		printf("The list is currently empty\n");
		return;	
	}
	
	//prints out all of the list
	for(int i=0; i<4; i++){	
		p=lists[i].head;
		while(p != NULL){
			printf("%s, %d\n", p->name, p->size);
			p=p->next;
		}
	}
}

//fit in function that searches and prints what parties can fit in the size
void fit_in(void){
	ENTRY *p;
	int input;
	int flag=0;
	int i;
	
	//checks if list is empty
	if(lists[0].head == NULL && lists[1].head == NULL && lists[2].head == NULL && lists[3].head == NULL){
		printf("The list is currently empty, there is nothing to show\n");
		return;
	}
	printf("Enter the party size you would like to fit: ");
	scanf("%d", &input);
	//prints all parties that can fit the size received
	if(input == 1 || input == 2){
		i=0;
	}
	if(input == 3 || input == 4){
		i=1;
	}
	if(input == 5 || input == 6){
		i=2;
	}
	if(input > 6){
		i=3;
	}
	for(int j=0; j<4; j++){
		p = lists[j].head;
		while(p!=NULL){
			if(p->size <= input){
				printf("%s\n",p->name);
				flag++;
			}
			p=p->next;
		}
	}
	//if no parties can fit the list prints no parties fit
	if(flag == 0){
		printf("There are no parties that fit\n");
	}
	return;	
}

//checks if name is duplicate returns 1 if duplicate returns 0 if not
int duplicate(char*name){	
	ENTRY *p;
	for(int i=0;i<4;i++){
		p=lists[i].head;
		while(p != NULL){
			if(strcmp(p->name,name) == 0){
				return 1;
			}
			p=p->next;
		}
	}
	return 0;
}

//frees all nodes remaining in the list and quits.
void free_all(void){
	ENTRY *p;

	for(int i=0; i<4; i++){
		p=lists[i].head;
		while(p != NULL){
			lists[i].head=p->next;
			free(p);
			p=lists[i].head;
		}
	}
}
