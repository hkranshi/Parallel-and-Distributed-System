#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <math.h>

#define PORT 8081
#define MAX_BUFFER_SIZE 1024
#define MAX_CLASSROOMS 5
#define MAX_TIMESLOTS 8
int counter=0;
typedef struct {
    int classroo;
    struct timeval Time;
    char req[12];
} Booking;
  typedef struct{
    int classroom;
     char req[12];
    
} status;
Booking bookedSlots[MAX_CLASSROOMS + 1][MAX_TIMESLOTS + 1] ;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int bookClassroom(char* classroom1, char* timeslot) {
    pthread_mutex_lock(&mutex);
    struct timeval Tlstart;
    gettimeofday(&Tlstart, NULL);
    int classroom=atoi(classroom1);
    char c[2];
    char *g=c;
     *g=timeslot[6];
     *(g+1)=timeslot[7];
    double t=atof(c);
    int qq=t;
   if(qq==9||qq==12||qq==15||qq==18){
        t=t+0.5;
     }
    double io=((t-8)/1.5);
     int ti=io;
   // int lp=;
        if ( bookedSlots[classroom][ti].classroo== ti) {
             pthread_mutex_unlock(&mutex);
            return -1;
        } 
        else {
           bookedSlots[classroom][ti].classroo = ti;
            char *o=bookedSlots[classroom][ti].req;
            strcpy(o,timeslot);
            
            gettimeofday(&Tlstart, NULL);
            bookedSlots[classroom][ti].Time = Tlstart;
            counter=counter+1;
            pthread_mutex_unlock(&mutex);
            return 0;
        }

 }

int cancelBooking(char* classroom1, char *timeslot) {
    pthread_mutex_lock(&mutex);
    struct timeval Tlstart;
 int classroom=atoi(classroom1);
    char c[2];
    char *g=c;
     *g=timeslot[6];
     *(g+1)=timeslot[7];
    double t=atof(c);
    int qq=t;
   if(qq==9||qq==12||qq==15||qq==18){
        t=t+0.5;
     }
    double io=((t-8)/1.5);
     int ti=io;
        
  //   if (classroom > 0 && classroom < 6 && ti > 0 && ti < 9) {
    int lp=bookedSlots[classroom][ti].classroo;
        if (lp!=ti){
            pthread_mutex_unlock(&mutex);
            return -3;
        }
        else {
             gettimeofday(&Tlstart, NULL);
            double t = (Tlstart.tv_sec - bookedSlots[classroom][ti].Time.tv_sec);
                      
            if (t < 20.0){
            pthread_mutex_unlock(&mutex);
                return -2;
            }
            else {
                bookedSlots[classroom][ti].classroo = 0;
                counter=counter-1;
                pthread_mutex_unlock(&mutex);
                return 0;
            }
        }
    }


status* get_status() {
    pthread_mutex_lock(&mutex);
  //  status p[counter]={0};
  
status* show=malloc(counter*sizeof(status));
int i=0;
for(int j=1;j<=MAX_CLASSROOMS;j++){
    for(int k=1;k<=MAX_TIMESLOTS;k++){
        if(bookedSlots[j][k].classroo==k){
            show[i].classroom=j;
            
           char *e =show[i].req;
           char *g=bookedSlots[j][k].req;
           i++;
           strcpy(e,g);
        }
    }
}


    pthread_mutex_unlock(&mutex);
    return show;
}

void *handleClientRequest(void *arg) {
    int clientSocket = *((int *)arg);
    char buffer[MAX_BUFFER_SIZE];
    int valread;

    // Read the client request
    valread = recv(clientSocket, buffer, sizeof(buffer), 0);

    if (valread > 0) {
        // Extract classroom and timeslot from the received message
       // char type[6];
       // char req_time[11];
       // int id;
        char* type[4];
      
    const char delimiter[] = ",";

    // Use strtok to split the string
    char *token = strtok(buffer, delimiter);
    int i=0;
    // Loop through the tokens and print them
    while (token != NULL) {
        //printf("%s\n", token);
        type[i]=token;
        i++;
        token = strtok(NULL, delimiter);
    }
        //sscanf(ch,"%d %s", &id, type);
       // sscanf(buffer, "%d %s %d %s", &timeslot, type, &classroom, req_time);

        if (strcmp(type[1], "GET")) {
            int result;

            if (strcmp(type[1], "BOOK") == 0) {
                result = bookClassroom(type[2], type[3]);
            } else {
                result = cancelBooking(type[2], type[3]);
               
            }

            // Send the status code back to the client
            char response[MAX_BUFFER_SIZE];
            snprintf(response, sizeof(response), "%d", result);
            send(clientSocket, response,sizeof(int), 0);
        } 
        else {
            // Handle GET request
            int p=counter;
            status* result1= get_status();
            int result=0;
            char response[MAX_BUFFER_SIZE];
            //snprintf(response, sizeof(response), "%d", result);
            size_t offset=0;
            char c='"';
             offset += snprintf(response + offset, sizeof(response) - offset, "0,%c{",c);
            for (int i = 0; i <p; i++) {
        // Assuming a simple string representation here, adjust as needed
        offset += snprintf(response + offset, sizeof(response) - offset, "( '%d' , '%s' ), ", result1[i].classroom, result1[i].req);
    }
     offset += snprintf(response + offset, sizeof(response) - offset, "}%c",c);
      if (offset >= sizeof(response)) {
            fprintf(stderr, "Buffer overflow detected!\n");
            //break;  // You may want to handle this situation appropriately
        }
        free(result1);
            send(clientSocket, response, offset, 0);
        }
    }

    // Close the client socket
    close(clientSocket);

    // Unlock the mutex before exiting the thread
 //   pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main() {
    int serverSocket, newSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    pthread_t thread;

    // Create socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 300) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);
    while (1) {
        // Accept a new connection
        if ((newSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrLen)) < 0) {
            perror("Accept failed");
            continue; // Continue to the next iteration
        }
        else
        
    
        // Create a new thread to handle the client request
        if (pthread_create(&thread, NULL, handleClientRequest, (void *)&newSocket) != 0) {
            perror("Thread creation failed");
            exit(EXIT_FAILURE);
        }

        // Detach the thread to allow it to run independently
        pthread_detach(thread);
    }

    // Close the server socket
    close(serverSocket);

    return 0;
}