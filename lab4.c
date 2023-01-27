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

//union declared
typedef union extra{
	char birthday_name[20];
	int years;
	float average_age;
}EXTRA;

//structure declared with union included
typedef struct entry{
	char name[LENGTH];
	int size;
	int occasion;
	EXTRA extra_info;
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
	//duplicate(list->name);
	printf("Enter your name here: ");
	scanf("%s", nametest);
	if(duplicate(nametest)== 1){
		printf("You have entered a duplicate name\n");
		return;
	}
	strcpy(list[counter].name, nametest);
	printf("Enter your party size here: ");
	scanf("%d", &list[counter].size);
	printf("Enter your occasion here: ");
	scanf("%d", &list[counter].occasion);

	//checks for different occasions and prompts user to enter input for different occasion
	if (list[counter].occasion == 1){
		printf("Name of birthday person? ");
		scanf("%s", list[counter].extra_info.birthday_name);
	}
	if (list[counter].occasion == 2){
		printf("How many years? ");
		scanf("%d", &list[counter].extra_info.years);
	}
	if (list[counter].occasion != 1 && list[counter].occasion != 2){
		printf("Average age? ");
		scanf("%f", &list[counter].extra_info.average_age);   
	}
	printf("You have created a reservation for %s, party size of %d\n", list[counter].name, list[counter].size);
	
	counter++;
}

//delete function that deletes party when it fits the size and moves list up
void delete(void){
	int table;
	int found=0;
	ENTRY *p=list;

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
			p+=i;
			printf("You have seated %s party size of %d\n", list[i].name, list[i].size);
			
			//for loop to shift all elements up
			for(int j=i;j<(counter-1);j++,p++){
				strcpy(p->name, (p+1)->name);
				p->size=(p+1)->size;
				p->occasion=(p+1)->occasion;
				strcpy(p->extra_info.birthday_name, (p+1)->extra_info.birthday_name);
				p->extra_info.years=(p+1)->extra_info.years;
				p->extra_info.average_age=(p+1)->extra_info.average_age;
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
	ENTRY *p = list;
	while(counter == 0){
		printf("The list is currently empty\n");
		return;
	}
	
	//prints out all of the list for different occasions
	for(int i=0; i<counter; i++, p++){
		if(p->occasion == 1){
			printf("%s, %d, %s\n", p->name, p->size, p->extra_info.birthday_name);
		}
		if(p->occasion == 2){
			printf("%s, %d, %d\n", p->name, p->size, p->extra_info.years);
		}
		if(p->occasion != 1 && p->occasion != 2){
			printf("%s, %d, %.2f\n", p->name, p->size, p->extra_info.average_age);
		}
	}
}

//fit in function that searches and prints what parties can fit in the size
void fit_in(void){
	ENTRY *p = list;
	int input;
	int flag;
	if(counter == 0){
		printf("The list is currently empty, there is nothing to show\n");
		return;
	}
	printf("Enter the party size you would like to fit: ");
	scanf("%d", &input);
	for(int i=0; i<counter; i++,p++){
		if(p->size <= input){
			printf("%s\n",p->name);
			flag++;
		}
		
	}
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
