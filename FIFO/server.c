#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
int main()
{
int fd1;
//fifo file path
char* myfifo = "/tmp/myfifo";
//Creating named file FIFO
//mkfifo=make fifo | pathname, permission
mkfifo(myfifo, 0666);
char str1[100], str2[100];
while(1)
{
//open fifo for read only
fd1=open(myfifo, O_RDONLY);
read(fd1,str1,100);
//print the read string then close
printf("Ushna: %s",str1);
close(fd1);
//now open write mode and write. take string from user
fd1=open(myfifo, O_WRONLY);
printf("Me: ");
//taking input 'str2' from user, 100 is max length
fgets(str2,100,stdin);
//write the input 'str2' on FIFO and close it
write(fd1,str2,strlen(str2)+1);
close(fd1);
}
return 0;
}
