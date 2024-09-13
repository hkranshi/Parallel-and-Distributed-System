#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <math.h>

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
//    else{
//          printf("connected\n");
//    }
}

void sendData(int clientSocket, const char *data) {
    send(clientSocket, data, strlen(data), 0);
}

void receiveAndWriteToFile(int clientSocket, FILE *outputFile,const char *data) {
    char buffer[MAX_BUFFER_SIZE];
    int valread = recv(clientSocket, buffer, sizeof(buffer), 0);

    if (valread > 0) {
        fprintf(outputFile, "%s,%s,\n",data, buffer);
    }
}

void closeSocket(int clientSocket) {
    close(clientSocket);
}

void processLineAndSendRequest(const char *line, FILE *outputFile) {
    int clientSocket = createSocket();
    connectToServer(clientSocket);
    //  printf("%s",line);
    sendData(clientSocket, line);
  
    receiveAndWriteToFile(clientSocket, outputFile,line);
    closeSocket(clientSocket);
}

int main() {
    FILE *csvFile,*outputFile;
    char line[MAX_BUFFER_SIZE];

    if ((csvFile = fopen("test_case_2.csv", "r")) == NULL) {
        perror("Error opening CSV file");
        exit(EXIT_FAILURE);
    }

    if ((outputFile = fopen("output.csv", "a")) == NULL) {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }

    fprintf(outputFile, "Request Time,Type,Room,Timeslot,Status, \n");

    fgets(line, sizeof(line), csvFile);  // Assuming this is a header line
    
    char ch[1024];
    while (fgets(line, sizeof(line), csvFile) != NULL) {
        line[strcspn(line, "\n")] = '\0'; // Remove newline characters
        sscanf(line, "%s ", ch);
//        printf("%s\n",ch);
    
//           char* type[4];
      
//     const char delimiter[] = ",";

//     // Use strtok to split the string
//     char *token = strtok(ch, delimiter);
//     int i=0;
//     // Loop through the tokens and print them
//     while (token != NULL) {
//        // printf("%s\n", token);
//         type[i]=token;
//         i++;
//         token = strtok(NULL, delimiter);
//     }
//   //  printf("%s %s \n",type[2],type[3]);
//  char* classroom1=type[2];
//     char* timeslot=type[3];
    
//      int classroom=atoi(classroom1);
//     char c[2];
//     char *g=c;
//      *g=timeslot[6];
//      *(g+1)=timeslot[7];
//     double t=atof(c);
//     int qq=t;
//    if(qq==9||qq==12||qq==15||qq==18){
//         t=t+0.5;
//      }
//     double io=((t-8)/1.5);
//      int ti=io;

//         printf("%d %d\n",classroom,ti);
         processLineAndSendRequest(ch, outputFile);
           sleep(1);
    }

    fclose(outputFile);
    fclose(csvFile);

    return 0;
}










