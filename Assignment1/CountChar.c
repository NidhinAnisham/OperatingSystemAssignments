/*
To Compile and Run:

gcc CountChar.c -o CountChar
./CountChar 5 test.txt test2.txt readme SOP_Brief.txt SelectionSort.java result

*/

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/file.h>

void main(int argc,char* argv[]){
    int i;
    int j;
    int n = atoi(argv[1]); //get value of n = number of files to be read
    
    //check if n is negative or 0
    if(n <=0){
        printf("Number of files is 0 or less than 0.");
        printf("Terminating");
        exit(-1);
    }
    
    //check if n input filenames are passed
    if((argc - 3) < n){
        printf("Not enough files");
        printf("Terminating");
        exit(-1);
    }
    
    int pro = n;     //to keep a count of processes
    pid_t pids[n];  //to store pid of the n processes
    FILE *fptr[n];  //pointer to read input files
    FILE *fout[n];  //pointer for temporary output files
    char filenames[n][15]; //for storing filenames
    char buf[12];  //for temporary output filenames
    for(i = 0;i<n;++i){
        strcpy(filenames[i] ,argv[i+2]);
        fptr[i] = fopen(filenames[i],"r");  //open the input files
        snprintf(buf, 12, "%d", i);
        fout[i] = fopen(buf,"w+");  //open temporary output files
    }
    char out_file[15]; //to store final result
    strcpy(out_file ,argv[argc-1]);

    /* Start children. */
    for (i = 0; i < n; ++i) {
      if ((pids[i] = fork()) < 0) { //forking n processes
        perror("Fork Error");
        abort();
      } else if (pids[i] == 0) { 
        if(fptr[i] == NULL){
            printf("Cannot Open File %d\n",i);
        }
        else{ 
            printf("Opened File %d\n",i);
            int countC[26] = {0};  //Storing count of capital letters
            int countS[26] = {0};  //Storing count of small letters
            int c;
            char ch;
            ch = fgetc(fptr[i]); //get character by character from input file
            
            //loop for counting characters till end of file is reached
            while(ch!=EOF){            
                if(ch>='a' && ch<='z'){  
                    countS[(int)(ch - 'a')] =  countS[(int)(ch - 'a')] + 1;
                }
                else if(ch>='A' && ch<='Z'){
                    countC[(int)(ch - 'A')] = countC[(int)(ch - 'A')] + 1;
                }
                ch = fgetc(fptr[i]);
            } 
            
            fprintf(fout[i],"\n\n%s\n",filenames[i]); //filename of the input file that is being processed
            
            //loop for inserting count of characters to temporary output files
            for (c = 0; c < 26; c++)
            {
                 if(countS[c] > 0){
                    fprintf(fout[i],"%c occurs %d time(s)\n", c + 'a', countS[c]);
                 }
                 if(countC[c] > 0){
                    fprintf(fout[i],"%c occurs %d time(s)\n", c + 'A', countC[c]);
                 }
            }
            
            //closing of output and input files
            fclose(fout[i]);            
            fclose(fptr[i]);
            exit(0);
        }            
      }
     
    }
    
    int status;
    FILE *fp3 = fopen(out_file, "w"); //for final result
    char c;
    pid_t pid;
    
    //Parent waiting for children to exit
    while (pro > 0) {
        pid = wait(&status);
        printf("Child with PID %ld exited.\n", (long)pid);
        --pro;      
    }  
    
    //loop for writing to final result file 
    for(i = 0;i<n;++i){
        FILE *ftemp;
        snprintf(buf, 12, "%d", i);
        ftemp = fopen(buf, "r");
        while ((c = fgetc(ftemp)) != EOF) 
            fputc(c, fp3);
        fclose(ftemp);  
        remove(buf); //deleting temporary file
    }
}