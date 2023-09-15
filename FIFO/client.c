#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
int main()
{
int fd;
//FIFO file path
char* myfifo = "/tmp/myfifo";
//Creating named file FIFO
//mkfifo=make fifo | pathname, permission
mkfifo(myfifo, 0666);
char arr1[100], arr2[100];
while(1)
{
//Open fifo for wirte only
fd=open(myfifo, O_WRONLY); 
printf("Me: ");
//file descriptor stdin = standard input, where data is sent to and read by the program
//fgets = reads a line and store it in the string
//taking input 'arr2' from user, 100 is max length
fgets(arr2,100,stdin);
//write the input 'arr2' on FIFO and close it
write(fd,arr2,strlen(arr2)+1);
close(fd);
//open fifo for read only
fd=open(myfifo, O_RDONLY);
//read form fifo
read(fd,arr1,sizeof(arr1));
//print the read message
printf("Kamisha: %s",arr1);
close(fd);
}
return 0;
}
