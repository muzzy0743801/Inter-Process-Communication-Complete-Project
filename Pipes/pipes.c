#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
int main() 
{
 //seed the random number generator with current time so that the numbers will be different every time we run the program
 srand(time(NULL));
 //creating a pipe, returning '-1' if not created
 int pipe1[2];
 if (pipe(pipe1) == -1) 
 return -1;
 //creating child-parent process
 pid_t pid = fork();
 //parent process
 if (pid == 0) 
 {
 //initializing a variable to store values in the pipe
 int parent;
 //creating a client text file to write the produced random numbers
 FILE *file = fopen("client.txt", "w");
 //closing read end of parent in the pipe
 close(pipe1[0]);
 //initializing loop upto 'i' times
 for (int i = 0; i < 20; i++)
 {
 //integer parent saving all the random numbers
 parent = rand();
 //writing from the write end of parent in the pipe
 write(pipe1[1], &parent, sizeof(int));
 //writing all the values stored in int parent to the text file we created above
 fprintf(file, "client (parent) sending value: %d\n", parent);
 }
 //closing the file and the write end of parent which inserts a EOF value into the pipe
 fclose(file);
 close(pipe1[1]);
 wait(NULL);
 } 
 //child process
 else 
 {
 //creating a server text file to read the consumed random numbers that were produced by parent process
 FILE *file = fopen("server.txt", "w");
 //initializing a variable to store the values from the pipe
 int child = 0;
 //closing write end of child in the pipe
 close(pipe1[1]);
 //reading from the read end of child in the pipe and executes the loop until read zero
 //The function read returns zero when the EOF value has been read from the pipe
 while(read(pipe1[0], &child, sizeof(int)) != 0)
 {
 //reading all the values stored in int child and saving it in the server text file
 fprintf(file, "server (child) receiving value: %d\n", child);
 }
 //closing the file and the read end of child, which concludes the execution. Inter process communication was done through pipes.
 fclose(file);
 close(pipe1[0]);
 }
}

