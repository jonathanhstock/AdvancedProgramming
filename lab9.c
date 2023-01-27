#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
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
void *autosave(void*);
void read_auto_saved(char*);

//declaring mutex
pthread_mutex_t mutex;

//structure with each entry including name, size, and next pointer
struct entry{
	char name[LENGTH];
	int size;
	ENTRY *next;
};

//declaration of functions
void free_all(ENTRY *);
void list_backwards(ENTRY *);
void name_backwards(ENTRY *);
void recursion(char *);

//declaring head and tail pointers
ENTRY *head=NULL;
ENTRY *tail=NULL;

//main function that prints interface and runs case scenarios
int main(int argc, char *argv[]){
	int answer;
	char nametest[LENGTH];
	int size;
	
	//declares all thread
	pthread_t thr;
	pthread_mutex_init (&mutex, NULL);
	pthread_create(&thr, NULL, autosave, (void*)argv[2]);


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
		printf("5 - List Backwards\n");
		printf("6 - Name Backwards\n");
		printf("7 - Read Auto Save\n");
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
				//locks thread
				pthread_mutex_lock(&mutex);
				insert(nametest, size);
				//unlocks thread
				pthread_mutex_unlock(&mutex);
				break;
			case 2:
				//locks thread
				pthread_mutex_lock(&mutex);
				delete();
				//unlocks thread
				pthread_mutex_unlock(&mutex);
				break;
			case 3:
				show();
				break;
			case 4:
				fit_in();
				break;
			case 5:
				list_backwards(head);
				break;
			case 6:
				name_backwards(head);
				break;
			case 7:
				read_auto_saved(argv[2]);
				break;
			case 0:
				
				save_file(argv[1]);
				//locks thread
				pthread_mutex_lock(&mutex);
				//cancels thread
				pthread_cancel(thr);
				//unlocks thread
				pthread_mutex_unlock(&mutex);
				free_all(head);
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

//reads file
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
	//closes the file
	fclose(fp);

	printf("Read file\n");
}

//saves file
void save_file(char *point){
	FILE *fp;
	int x;
	char n[LENGTH];
	ENTRY *p = head;
		

	fp = fopen(point, "w");
	//if the file is empty lets user know
	if(fp == NULL){
		printf("empty file\n");
	}
	
	//fprintf(fp, "Name\tGroup Size\n");
	//fprintf(fp, "------------------------\n");
	
	//while the list is not empty prints the name and size of each party
	while(p != NULL){
		fprintf(fp, "%s\t%d\n", p->name, p->size);
		p=p->next;
	}
	
	//closes the file
	fclose(fp);
	printf("Saved File\n");
}

//function that prints list backwards
void list_backwards(ENTRY *p){
	//when the pointer reaches the end of the list it returns
	if(p==NULL){
		return;
	}
	else{
		//function calls itself until it reaches the end of the list then returns
		list_backwards(p->next);
		//prints each name and size in order of last to first
		printf("%s %d\n", p->name, p->size);	
	}
	return;
}
//function that prints each name out backwards
void name_backwards(ENTRY *p){
	//when the pointer reaches the end of the list it returns
	if(p==NULL){
		return;
	}
	else{
		//calls recursion function for each name
		recursion(p->name);
		printf("\n");
		//after the each name is printed calls the next name
		name_backwards(p->next);
	}
	return;
}

//function that goes through the name and prints each letter from last to first
void recursion(char *p){
	//when the pointer reaches the end of the name it returns
	if(*p=='\0'){
		return;
	}
	else{
		//keeps calling recursion function until it reaches the end of the name then prints letters
		recursion(p+1);
		printf("%c", *p);
	}
}

//frees all nodes remaining in the list and quits
void free_all(ENTRY *p){
	//when the pointer reaches the end of the list it returns
	if(p==NULL){
		return;
	}
	else{
		//goes through each node and frees them from last to first
		free_all(p->next);
		free(p);
	}
	return;
}

//auto saves file every 15 seconds
void *autosave(void*arg){
	
	FILE *fp;
	char* file_name = (char*) arg;
	ENTRY *p;
	
	while(1){
		//opens file
		fp = fopen(file_name, "wb");
		
		//if file empty prints error reading file
		if(fp == NULL){
			printf("Error writing file");
		}
		p = head;
		//locks thread	
		pthread_mutex_lock(&mutex);
		//writes each node to the file
		while(p != NULL){
			fwrite(p, sizeof(ENTRY), 1, fp);
			p=p->next;
		}
		printf("Autosave successful\n");
		
		//closes file
		fclose(fp);
		//unlocks thread
		pthread_mutex_unlock(&mutex);
		sleep(15);
	}
	return 0;
}

//reads the auto saved file
void read_auto_saved(char*arg){
	FILE *fpbin;
	ENTRY temp;
	
	//opens file
	fpbin = fopen(arg, "rb");
	//checks if there is no one in the list
	if(fpbin == NULL){
		printf("File does not exist\n");
		return;
	}
	//locks thread
	pthread_mutex_lock(&mutex);	
	//prints out all of the list
	while(fread(&temp, sizeof(ENTRY), 1, fpbin)==1){
		printf("%s, %d\n", temp.name, temp.size);
	}
	//closes file
	fclose(fpbin);
	//unlocks thread
	pthread_mutex_unlock(&mutex);
	return;
}
