#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 10
#define LENGTH 20

//declaration of all variables and functions
void insert(void);
void delete(void);
void show(void);
void fit_in(void);
int duplicate(char*);

//declaration of structure
typedef struct entry{
	char name[LENGTH];
	int size;
}ENTRY;

ENTRY list[SIZE];
int counter=0;

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
				return 0;
			default:
				printf("Bad Input\n");
				break;
				
		}
	}
}

//insert function that inserts party and their size
void insert(void){
	
	char nametest[LENGTH];
	
	//checks if list is full
	if(counter == 10){
		printf("The list is currently full\n");
		return;
	}
	
	//runs duplicate function
	printf("Enter your name here: ");
	scanf("%s", nametest);
	if(duplicate(nametest)== 1){
		printf("You have entered a duplicate name\n");
		return;
	}
	//moves temporary name to the list
	strcpy(list[counter].name, nametest);
	printf("Enter your party size here: ");
	scanf("%d", &list[counter].size); 
	printf("You have created a reservation for %s, party size of %d\n", list[counter].name, list[counter].size);
	
	//increases counter
	counter++;
}

//delete function that deletes party when it fits the size and moves list up
void delete(void){
	int table;
	int found=0;

	//checks if no one is in the list
	if(counter == 0){
		printf("The list is currently empty, there is nothing to delete\n");
		return;
	}

	//runs if there is someone in the list
	printf("Enter the size of the table available: ");
	scanf("%d", &table);

	//runs a loop that looks through the list to see which of the oldest reservations can fit the table
	//seats parties and deletes reservations and shifts list up
	for(int i=0; i<counter; i++){
		if(list[i].size<=table){
			table-=list[i].size;
			printf("You have seated %s party size of %d\n", list[i].name, list[i].size);
			
			for(int j=i;j<(counter-1);j++){
				strcpy(list[j].name, list[j+1].name);
				list[j].size = list[j+1].size;
			}
			found=1;
			counter--;
			i--;
		}
	}
	//if there is no reservation available for the table size prints no reservation available
	if (found == 0)
		printf("No reservation available\n");
	return;
}

//list function that prints out the current list
void show(void){
	//checks if there is no one in the list
	while(counter == 0){
		printf("The list is currently empty\n");
		return;
	}
	
	//prints out all of the list
	for(int i=0; i<counter; i++){
		printf("%s, %d\n", list[i].name, list[i].size);
	}
}

//fit in function that searches and prints what parties can fit in the size
void fit_in(void){
	ENTRY *p = list;
	int input;
	int flag;

	//prints if there is nobody in the list
	if(counter == 0){
		printf("The list is currently empty, there is nothing to show\n");
		return;
	}
	printf("Enter the party size you would like to fit: ");
	scanf("%d", &input);
	//searches through the list of parties and checks if the the part fits in the size
	for(int i=0; i<counter; i++,p++){
		if(p->size <= input){
			printf("%s\n",p->name);
			flag++;
		}
		
	}

	//prints if there aren't any parties that fit
	if(flag == 0){
		printf("There are no parties that fit\n");
	}	
}

//checks if name is duplicate
int duplicate(char*name){	
	for(int i=0; i<counter; i++){
		if(strcmp(list[i].name,name) == 0)
			return 1;
	}
	return 0;
}
