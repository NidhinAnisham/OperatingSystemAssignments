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
* c1.txt
*
* Use command "gcc -pthread prog1.c -o prog1" to compile
* Use command "./prog1" to run the sample case
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
int* A;
int* B;
int* result;
int **ans;  
*/

int A[MAX], B[MAX], result[2*MAX], ans[MAX][2*MAX]; 
int l1, l2, productArray[10][10], carryArray[10][10];

// A single-threaded implementation of the multiply method
void *multiply(void *arg) {
	// Initializing variables. 
	int carry, i, j;

	// Form a 2D matrix by multiplying each digit with the other number
	for(i=l2-1; i>-1; i--) {
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

// A single-threaded implementation of the add method
void *add(void *arg) {
	// Initializing variables. 
	int i,j;

	// Add all the numbers in the specific column. Ex: All units' digits
	for(i=(l1+l2-1); i>-1; i--) {
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

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  printf("\nCurrent time and date: %s", asctime (timeinfo) );
  
	// Declaring variables
	int q, sum, i, j, k;
	char s1[MAX],s2[MAX];
  
	// Reading the number lengths from the files
	FILE* P = fopen("a.txt","r");
	FILE* Q = fopen("b.txt","r");
	fgets(s1, MAX, P);
	fgets(s2, MAX, Q);
	l1 = atoi(s1);
	l2 = atoi(s2);

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
    printf("The no. of digits in file a.txt do not match the length given. Replacing the length by no. of digits.\n");
    l1 = num1;
  }
  if(num2!=l2){
    printf("The no. of digits in file b.txt do not match the length given. Replacing the length by no. of digits.\n");
    l2 = num2;
  }
  
  /*
	// Close the file pointers
	fclose(P);
	fclose(Q);
  A = (int*) malloc(l1*sizeof(int));
  B = (int*) malloc(l2*sizeof(int));
  result = (int*) malloc((l1+l2)*sizeof(int));
  ans = (int **)malloc(l2 * sizeof(int *)); 
  for(i=0;i<l2; i++){
    ans[i] = (int *)malloc((l1+l2) * sizeof(int));
    for(j=0;j<(l1+l2);j++){
       ans[i][j] = 0;
       result[j] = 0;
    }
  } 
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

	// Printing the input numbers
	/*
  for(i=MAX-l1; i<MAX; i++) {
		printf("%d", A[i]);
	}
	printf("\n");
	for(i=MAX-l2; i<MAX; i++) {
		printf("%d", B[i]);
	}
	printf("\n");
  */
	pthread_t tid;

	// Spawning thread for multiplying all the digits with the other number before adding
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	printf("Creating multiplication thread.\n");
	pthread_create(&tid, &attr, multiply, (void *)NULL);

	// Joining the thread
	printf("Joining multiplication thread.\n");
	pthread_join(tid,NULL);
  //free(A);
  //free(B);
	printf("All multiplication threads have finished executing.\n");
	
  // Spawning thread for addition
	pthread_t tidAdd;
  pthread_attr_t attrAdd;
	pthread_attr_init(&attrAdd);
	printf("Creating addition thread.\n");
	pthread_create(&tidAdd, &attrAdd, add, (void *)NULL);
 
	printf("Joining addition thread.\n");
	pthread_join(tidAdd,NULL);
  //free(ans);
	// Cascading the carry to complete the addition
	int carry=0;
	for(i=(l1+l2-1); i >= 0; i--) {
		int sum = carry + result[i];
		result[i] = sum%10;
		carry = sum/10;
	}
  printf("All addition threads have finished executing.\n");
	
	// Writing product to a file
	//printf("Product : \n");
  FILE *fout;
	fout=fopen("c1.txt","w");
	for(i=0; i<(l1+l2); i++) {
		//printf("%d ",result[i]);
		fprintf(fout,"%d",result[i]);
	}
  //free(result);
	fclose(fout);
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  printf("Current time and date: %s\n", asctime (timeinfo) );
	return 0;
}
