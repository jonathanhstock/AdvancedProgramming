#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void){

	srand((int)time (NULL));

	//int all variables in the program
	int dividend;
	int answer;
	int counter=0;
	
	//runs program 10 times and runs random variables each time
	for(int i=0;i<10;i++){
		int denominator = rand()% 12 + 1;
		int quotient = rand()% 13;


		//giving user a problem and asks for input
		int dividend = denominator*quotient;
		printf("%d/%d=\n",dividend,denominator);
		scanf("%d",&answer);
		
		//prints whether user input is correct or wrong
		//if user is right increases counter otherwise doesn't
		if(answer == quotient){
			printf("Correct!\n");
			counter++;
		}
		else{
			printf("Wrong! Correct answer is %d\n", quotient); 
		}
		
	}

	//prints score
	printf("Your score is %d\n", counter);	
	return 0;
}
