#include <sys/stat.h>
#include <sys/types.h>//data types pid_t
#include <sys/wait.h>//declaration for waiting
#include <stdlib.h>//standard libraray functions
#include <stdio.h>//standard buffered I/O
#include <mqueue.h>//message queues
#include <unistd.h>//define miscellaneous symbolic constants and types
#include <fcntl.h>//file control options
#include <time.h>
//defining values to represent two message types
#define MSG_EMPTY 0
#define MSG_NUMBER 1
void client(mqd_t queue);
void server(mqd_t queue);
//hold the message type and the integer value
typedef struct { char type; int value; } message_t;
//pass message structs from client process to server process
typedef union { message_t message; char buffer[sizeof(message_t)]; } packet_t;
int main(void) {
 //getting and setting the attributes of a msg queue
 struct mq_attr attr = {
 .mq_maxmsg = 10,//max no. of msgs
 .mq_msgsize = sizeof(message_t),//max msg size
 .mq_curmsgs = 0,//no. of msgs currently queued
 };
 //message queues descriptors to open message queue using mq_open
 mqd_t queue = mq_open("/mq_project", O_CREAT | O_RDWR, 0666, &attr);
 srand(time(NULL));
 //used for process ids and process group ids. fork to create parent and child process
 pid_t pid = fork();
 //if parent id > 0 then execute client else execute server
 if (pid > 0) { client(queue); }
 else { server(queue); }
}
void client(mqd_t queue) {
 //passing msg to server process
 packet_t packet;
 //opening file using fopen
 FILE *f = fopen("client.txt", "w");
 
 for (int i = 0; i < 20; i++) 
 {
 packet.message.type = MSG_NUMBER;
 packet.message.value = rand();
 mq_send(queue, packet.buffer, sizeof(message_t), 0);
 fprintf(f, "client (parent) sending value: %d\n", packet.message.value);
 }
 //loop executed, client sends a final msg of MSG_EMPTY
 packet.message.type = MSG_EMPTY;
 mq_send(queue, packet.buffer, sizeof(message_t), 0);
 //closes the queue and client.txt file
 mq_close(queue);
 fclose(f);
 wait(NULL);
}
void server(mqd_t queue) {
 packet_t packet;
 FILE *f = fopen("server.txt", "w");
 mq_receive(queue, packet.buffer, sizeof(message_t), 0);
 while (packet.message.type != MSG_EMPTY) 
 {
 fprintf(f, "server (child) receiving value: %d\n", packet.message.value);
 mq_receive(queue, packet.buffer, sizeof(message_t), 0);
 }
 mq_close(queue);
 fclose(f);
 //for destroying msg queue
 mq_unlink("/mq_project");
}
