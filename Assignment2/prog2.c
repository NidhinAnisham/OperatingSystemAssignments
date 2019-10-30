/*
* Author: Nidhin Anisham
*
* :: Input Files ::
* ================
* a.txt
* b.txt
*
* :: Result Files ::
* ================
* c2.txt
*
* Use command "gcc -pthread prog2.c -o prog2" to compile
* Use command "./prog2" to run the sample case
*
*/

#include <stdio.h> 
#include <stdlib.h> 
#include <pthread.h> 
#include <string.h>
#include <time.h>

// Declaration of global variables
#define MAX 100001
/*
short* A;
short* B;
short* result;
short **ans; 
*/
int A[MAX], B[MAX], result[2*MAX], ans[MAX][2*MAX]; 
int l1, l2, productArray[10][10], carryArray[10][10];

// Structure which is used as an argument to the multi-threaded function
struct mul_large_num {
	int start, offset;
};

// A multi-threaded implementation of the multiply method
void *multiply(void *arg) {
	// Initializing variables. The start and offset tell the thread which digits to work on.
	struct mul_large_num *arg_struct = (struct mul_large_num*)arg;
	int carry, i, j;
	int start = arg_struct->start;
	int offset = arg_struct->offset;
  
	// Form a 2D matrix by multiplying each digit with the other number and then add it in a multithreaded way to avoid sequentialness
	for(i=start-1; i>=(start-offset); i--) {
		carry = 0;
		for(j = l1-1; j>-1; j--) {
			ans[l2-i-1][j+i+1] = (carry + productArray[B[i]][A[j]])%10;
			carry = (carryArray[B[i]][A[j]]) + ((carry + productArray[B[i]][A[j]])/10);
		}
		ans[l2-i-1][j+i+1] += carry;
	}

	// Exit the thread
	pthread_exit(0);
}

// A multi-threaded implementation of the add method
void *add(void *arg) {
	// Initializing variables. The start and offset tell the thread which digits to work on.
	struct mul_large_num *arg_struct = (struct mul_large_num*)arg;
	int i,j;
	int start = arg_struct->start;
	int offset = arg_struct->offset;
	// Add all the numbers in the specific column. Ex: All units' digits
	for(i=start-1; i>=(start-offset); i--) {
		for(j=0; j<l2; j++) {
			result[i] += ans[j][i];
		}
	}

	// Exit the thread
	pthread_exit(0);
}

int main() {

  time_t rawtime;
  struct tm * timeinfo;

	// Delaring variables
	int n, q, sum, i, j, k;
	char s1[MAX],s2[MAX];

	// Reading the number lengths from the files
	FILE* P = fopen("a.txt","r");
	FILE* Q = fopen("b.txt","r");
	fgets(s1, MAX, P);
	fgets(s2, MAX, Q);
	l1 = atoi(s1);
	l2 = atoi(s2);

	// Take the no. of threads as input
	printf("\nEnter number of threads: ");
	scanf("%d", &n);
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  printf("\nCurrent time and date: %s", asctime (timeinfo) );
	struct mul_large_num args[n];
  // printf("Allocating memory.\n");
  // char chA = fgetc(P);
  // char chB = fgetc(Q);\
  s1[0] = '\0';
  s2[0] = '\0';
	fgets(s1, MAX, P);
	fgets(s2, MAX, Q);
  int num1 = strlen(s1);
  int num2 = strlen(s2);
  if(num1==0 || num2==0) {
    printf("There are no digits present in one/both the files.\n");
    return 0;
  }
  if(num1!=l1){
    printf("The no. of digits in file a.txt, %d do not match the length given %d. Replacing the length by no. of digits.\n", num1, l1);
    l1 = num1;
  }
  if(num2!=l2){
    printf("The no. of digits in file b.txt, %d do not match the length given %d. Replacing the length by no. of digits.\n", num2, l2);
    l2 = num2;
  }
	// Close the file pointers
	
 /*
  A = (short*) malloc(l1*sizeof(short));
  B = (short*) malloc(l2*sizeof(short));
  result = (short*) malloc((l1+l2)*sizeof(short));
  ans = (short **)malloc(l2 * sizeof(short *)); 
  printf("DONE\n");
  
  for(i=0;i<l2; i++){
    ans[i] = (short *)malloc((l1+l2) * sizeof(short));
    for(j=0;j<(l1+l2);j++){
       ans[i][j] = 0;
       result[j] = 0;
    }
    B[i] = (short)(chB- '0');
    chB = fgetc(Q);
  }
  printf("DONE\n");
  for(i=0; i<l1; i++) {
		A[i] = (short)(chA- '0');
    chA = fgetc(P);
	}
  printf("DONE\n");
  */
	// Create a 2D lookup array inorder to avoid repeated multiplication
	for (i=0; i<10; i++) {
		for (j=0; j<10; j++) {
			productArray[i][j] = (i*j)%10;
			carryArray[i][j] = (i*j)/10;
		}
	}
  
  
	// Processing the input numbers
	for(i=0; i<l1; i++) {
		A[i] = s1[i] - '0';
	}
	for(i=0; i<l2; i++) {
		B[i] = s2[i] - '0';
	}
  
  
  
  fclose(P);
	fclose(Q);
	// Splitting the tasks so that each and every thread almost do the the same amount of work
	int defaultTasks = l2/n;
	int remainder = l2%n;
	int start = l2;
	pthread_t tid[n];
  printf("Creating %d multiplication threads.\n",n);
	// Spawning give no. of threads for multiplying all the digits with the other number before adding
	for(k=0; k<n; k++) {
		args[k].start = start;
		args[k].offset = (k<remainder) ? defaultTasks+1 : defaultTasks;
		start -= args[k].offset;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tid[k], &attr, multiply, &args[k]);
	}

	// Joining all the threads
  printf("Joining %d multiplication threads.\n",n);
	for(i=0; i<n; i++) {
		pthread_join(tid[i],NULL);
	}

	printf("All multiplication threads have finished executing.\n");

	// Splitting the tasks so that each and every thread almost do the the same amount of work
	pthread_t tidAdd[n];
	struct mul_large_num argsAddition[n];
	defaultTasks = (l1+l2)/n;
	remainder = (l1+l2)%n;
	start = l1+l2;
  
  printf("Creating %d addition threads.\n",n);
	// Spawning give no. of threads for adding all the digits in the 2D array
	for(k=0; k<n; k++) {
		argsAddition[k].start = start;
		argsAddition[k].offset = (k<remainder) ? defaultTasks+1 : defaultTasks;
		start -= argsAddition[k].offset;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tidAdd[k], &attr, add, &argsAddition[k]);
	}
 
  printf("Joining %d addition threads.\n",n);
	for(i=0; i<n; i++) {
		pthread_join(tidAdd[i],NULL);
	}

	// Cascading the carry to complete the addition
	int carry=0;
	for(i=l1+l2-1; i >= 0; i--) {
		int sum = carry + result[i];
		result[i] = sum%10;
		carry = sum/10;
	}
  printf("All addition threads have finished executing.\n");
	
	// Writing the product to a file
	//printf("Product : \n");
  FILE *fout;
	fout=fopen("c2.txt","w");
	for(i=0; i<(l1+l2); i++) {
		//printf("%d",result[i]);
		fprintf(fout,"%d",result[i]);
	}
	fclose(fout);
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  printf("Current time and date: %s\n", asctime (timeinfo) );
	return 0;
}
