#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
  
int main() 
{ 
    int i,lower = 0, upper = 9, countA = 100000, countB = 100000; 
    FILE *A,*B;
    A=fopen("a.txt","w");
    B=fopen("b.txt","w");
    srand(time(0)); 
    fprintf(A,"%d\n",countA);
    fprintf(B,"%d\n",countB);
    for (i = 0; i < countA; i++) { 
        int num = (rand() % (upper - lower + 1)) + lower; 
        fprintf(A,"%d",num);
    } 
    for (i = 0; i < countB; i++) { 
        int num = (rand() % (upper - lower + 1)) + lower; 
        fprintf(B,"%d",num);
    } 
    fclose(A);
    fclose(B);
    return 0; 
} 