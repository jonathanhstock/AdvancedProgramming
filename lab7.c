#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LENGTH 20
#define ENTRY struct entry

//declaration of all variables and functions
void insert(char *, int);
void delete(void);
void show(void);
void fit_in(void);
int duplicate(char*);
void read_file(char*);
void save_file(char*);
void free_all(void);

//structure with each entry including name, size, and next pointer
struct entry{
	char name[LENGTH];
	int size;
	ENTRY *next;
};

//declaring head and tail pointers
ENTRY *head=NULL;
ENTRY *tail=NULL;

//main function that prints interface and runs case scenarios
int main(int argc, char *argv[]){
	int answer;
	char nametest[LENGTH];
	int size;


	if(argc == 1){
		printf("The name of the file is missing\n");
		return 1;
	}

	read_file(argv[1]);

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
				//scans for name
				printf("Enter your name here: ");
				scanf("%s", nametest);
	
				//runs duplicate function and returns if name is duplicate
				if(duplicate(nametest)== 1){
					printf("You have entered a duplicate name\n");
					break;
				}
				//receives party size
				printf("Enter your party size here: ");
				scanf("%d", &size);
				insert(nametest, size);
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
				save_file(argv[1]);
				free_all();
				return 0;
				break;
			default:
				printf("Bad Input\n");
				break;
				
		}
	}
}

//insert function that inserts party and their size
void insert(char *x, int s){
	ENTRY *p;

	if((p = (ENTRY *)malloc(sizeof (ENTRY))) == NULL){
		printf("malloc error\n");
		exit(1);
	}
	//enters p into the head and tail if list is empty
	//else enters p into the next node
	p->next=NULL;
	if(head==NULL && tail==NULL){
		head=tail=p;
	}
	else{
		tail->next=p;
		tail=p;
	}
	//if not duplicate continues with function and name is copied into the structure
	strcpy(p->name, x);
	p->size = s;
	printf("You have created a reservation for %s, party size of %d\n", p->name, p->size);
	return;
}

//delete function that deletes party when it fits the size and moves list up
void delete(void){
	ENTRY *p = head;
	ENTRY *q = head;
	int table;
	int found=0;
	
	//checks if no one is in the list
	if(p == NULL){
		printf("The list is currently empty, there is nothing to delete\n");
		return;
	}

	//runs if there is someone in the list
	printf("Enter the size of the table available: ");
	scanf("%d", &table);

	//runs a loop that looks through the list to see which of the oldest reservations can fit the table
	//seats parties and deletes reservations and shifts list up
	while(p != NULL){
		if(p->size<=table){
			table-=p->size;
			found=1;
			printf("You have seated %s party size of %d\n", p->name, p->size);
			
			if(p == head){
				head = p->next;
				if(head == NULL){
					tail=NULL;
				}
				free(p);
				q=p=head;
			} 
			else{
				q->next=p->next;
				free(p);
				p=q->next;
				if(p==NULL){
					tail=p;
				}
			}
		}
		else{
			q=p;
			p=p->next;
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
	ENTRY *p = head;
	//checks if there is no one in the list
	if(p == NULL){
		printf("The list is currently empty\n");
		return;
	}
	
	//prints out all of the list
	while(p != NULL){
		printf("%s, %d\n", p->name, p->size);
		p=p->next;
	}
}

//fit in function that searches and prints what parties can fit in the size
void fit_in(void){
	ENTRY *p = head;
	int input;
	int flag;
	//checks if list is empty
	if(p == NULL){
		printf("The list is currently empty, there is nothing to show\n");
		return;
	}
	printf("Enter the party size you would like to fit: ");
	scanf("%d", &input);
	//prints all parties that can fit the size received
	while(p!=NULL){
		if(p->size <= input){
			printf("%s\n",p->name);
			flag++;
		}
		p=p->next;
		
	}
	//if no parties can fit the list prints no parties fit
	if(flag == 0){
		printf("There are no parties that fit\n");
	}	
}

//checks if name is duplicate returns 1 if duplicate returns 0 if not
int duplicate(char*name){	
	ENTRY *p = head;
	while(p != NULL){
		if(strcmp(p->name,name) == 0){
			return 1;
		}
		p=p->next;
	}
	return 0;
}

void read_file(char *p){
	FILE *fp;
	int x;
	char n[LENGTH];

	fp = fopen(p, "r");

	//if file is empty then doesn't open file
	if(fp == NULL){
		printf("Could not open file\n");
		return;
	}
	//seeks for the end of the file
	//fseek(fp, 41, SEEK_SET);
	while(fscanf(fp,"%s %d", n, &x) == 2){
		insert(n, x);
	}
	fclose(fp);

	printf("Read file\n");
}

void save_file(char *point){
	FILE *fp;
	int x;
	char n[LENGTH];
	ENTRY *p = head;
		

	fp = fopen(point, "w");
	
	if(fp == NULL){
		printf("empty file\n");
	}
	
	//fprintf(fp, "Name\tGroup Size\n");
	//fprintf(fp, "------------------------\n");
	
	//if list is empty nothing runs
	while(p != NULL){
		fprintf(fp, "%s\t%d\n", p->name, p->size);
		p=p->next;
	}
	
	fclose(fp);
	printf("Saved File\n");
}
//frees all nodes remaining in the list and quits
void free_all(void){
	ENTRY *p = head;
	
	while(p != NULL){
		head = p->next;
		free(p);
		p = head;
	}
}
