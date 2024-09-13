#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <math.h>
#include <iostream>
#include <sys/wait.h>
#include <sys/types.h>


#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8081
#define MAX_BUFFER_SIZE 2048

int createSocket() {
    int clientSocket;
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    // else{
    //     printf("created\n");
    // }
  
    return clientSocket;
}

void connectToServer(int clientSocket) {
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddr.sin_port = htons(SERVER_PORT);

    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }
   else{
          printf("connected\n");
    }
}

void sendData(int clientSocket, const char *data) {
    send(clientSocket, data, strlen(data), 0);
}

void receiveAndWriteToFile(int clientSocket, FILE *outputFile,pid_t p) {
    char buffer[MAX_BUFFER_SIZE];
 
        // std :: cout << getpid() << " "<< p<<"\n";
     
   int  valread = recv(clientSocket, buffer, sizeof(buffer), 0);
   
   //  std :: cout << getpid() << " "<< p<<"\n";
    if(p!=0)
            waitpid(p,NULL,0);
    std :: cout << buffer <<"\n";
    
     
    if (valread > 0) { 
         
        fprintf(outputFile, "%s\n", buffer);
    }
}

void closeSocket(int clientSocket) {
    close(clientSocket);
}

void processLineAndSendRequest(const char *line, FILE *outputFile,pid_t p) {
//    std :: cout << getpid() << " "<< p<<"\n";
    int clientSocket = createSocket();
    connectToServer(clientSocket);
    //  printf("%s",line);
    sendData(clientSocket, line);
  
    receiveAndWriteToFile(clientSocket, outputFile, p);
    closeSocket(clientSocket);
}

int main(int argc, char *argv[]) {
   

    FILE *csvFile,*outputFile;
    char line[MAX_BUFFER_SIZE];
     char line1[MAX_BUFFER_SIZE];
     int count=0;
     if ((csvFile = fopen("test_case.csv", "r")) == NULL) {
        perror("Error opening CSV file");
        exit(EXIT_FAILURE);
    }
     fgets(line1, sizeof(line1), csvFile); 
       while(fgets(line, sizeof(line), csvFile) != NULL) {
        line[strcspn(line, "\n")] = '\0'; // Remove newline characters
        count++;

       }
       std :: cout << count << "\n";

        fclose(csvFile);
char response[MAX_BUFFER_SIZE];
            //snprintf(response, sizeof(response), "%d", result);
            size_t offset=0;
          //  char c='"';
             offset += snprintf(response + offset, sizeof(response) - offset, "%d ,",count);
             if (offset >= sizeof(response)) {
            fprintf(stderr, "Buffer overflow detected!\n");
            //break;  // You may want to handle this situation appropriately
        }
         int clientSocket1= createSocket();
    connectToServer(clientSocket1);
    //  printf("%s",line);
    sendData(clientSocket1, response);
    char buff[1024];
    int y=0;
    while(1){

    ssize_t bytesRead = recv(clientSocket1, buff, sizeof(buff) - 1, 0);
    if (bytesRead == -1) {
        perror("Error receiving data");
        close(clientSocket1);
        exit(EXIT_FAILURE);
    }
    if(bytesRead>0)
    y=1;
    if(y==1)
    break;
    }


    close(clientSocket1);
  




    if ((csvFile = fopen("test_case.csv", "r")) == NULL) {
        perror("Error opening CSV file");
        exit(EXIT_FAILURE);
    }

    if ((outputFile = fopen("output.csv", "a")) == NULL) {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }

    //fprintf(outputFile, "Request Time,Type,Room,Timeslot,Status, \n");
   // for(int i=0;i<integerValue;i++){
    fgets(line, sizeof(line), csvFile);  // Assuming this is a header line
   // }
    
    char ch[MAX_BUFFER_SIZE];
     pid_t pre=0,pp;
     

    while(fgets(line, sizeof(line), csvFile) != NULL|| count>0) {
        line[strcspn(line, "\n")] = '\0'; // Remove newline characters
        sscanf(line, "%s ", ch);
       printf("%s\n",ch);
       
       
        pid_t p=fork();
        if(p==0)
         {  
               
         processLineAndSendRequest(ch, outputFile,pre);
         return 0;
         }
         sleep(2);
         pp=p;
         pre=pp;
         // break;
        // sleep(1);
        count--;
    }
    while (wait(nullptr) > 0);
     // std :: cout << "asd";

    fclose(outputFile);
    fclose(csvFile);
     

    return 0;
}










