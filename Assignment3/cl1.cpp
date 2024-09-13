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
#include <mutex>
#include <mpi.h>
#include <vector>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8081
#define MAX_BUFFER_SIZE 1024
pthread_mutex_t mutex;
struct s{
    int id;
    int prank;
    
    s(int _id, int _prank)
        : id(_id), prank(_prank){}

};

struct Court {
    int total=0;
    int game_end=0;
    int court_num=0;
    int count=0;
   
    std::vector<s> pids;
};

int createSocket() {
    int clientSocket;
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
  
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
    std :: cout << data << "\n";
    send(clientSocket, data, strlen(data), 0);
}

void receiveAndWriteToFile(int clientSocket) {
    char buffer[MAX_BUFFER_SIZE];
    int valread = recv(clientSocket, buffer, sizeof(buffer), 0);
    char buff[MAX_BUFFER_SIZE];
    sscanf(buffer,"%s",buff);
    if (valread > 0) {
      
              char* type[6];
          const char delimiter[] = ",";

//     // Use strtok to split the string
               char *token = strtok(buffer, delimiter);
                int i=0; 
//     // Loop through the tokens and print them
    while (token != NULL) {
        //printf("%s\n", token);
        
        type[i]=token;
       // printf("%s\n", type[i]);
        i++;
        token = strtok(NULL, delimiter);
    }
    
    int id=atoi(type[0]);
    int st=atoi(type[1]);
    int et=atoi(type[2]);
    //  std::cout<< et<<" "<<v<<" "<<idd<< "\n";
    if(et==-1)
    {
            std::cout<< id<<" "<<st<<" "<<et<< "\n";

    }
    else{

    
    //printf("%s %s \n",type[2],type[3]);

  
       
        char* p =type[4];
       
      
         int id1=atoi(type[5]);
         int court=atoi(type[3]);
        // char c=type[3];
         std:: cout  << id << " "<<st << " "<< et<< " " << court << "  "<< type[4] << "\n";
    }
    
    
                  
   // std::cout << buffer << "\n";
MPI_Send( buff , MAX_BUFFER_SIZE , MPI_CHAR , 0, 0 , MPI_COMM_WORLD);
   
    int winid;
    MPI_Status fstatus;
    MPI_Recv( &winid , 1 , MPI_INT , 0 , 0 , MPI_COMM_WORLD , &fstatus);
  //  std :: cout << "fin " << winid << "\n";
    if(winid==-2){
        int winid1;
    MPI_Status fstatus;
    MPI_Recv( &winid1 , 1 , MPI_INT , 0 , 0 , MPI_COMM_WORLD , &fstatus);
  //  std :: cout << "pid " << winid1 << "\n";
     MPI_Status st;
            char received_message[MAX_BUFFER_SIZE];
        MPI_Recv(received_message,sizeof(received_message), MPI_CHAR, winid1, 0, MPI_COMM_WORLD, &st);
        std::string received_str(received_message);
        int rq;
        MPI_Comm_rank(MPI_COMM_WORLD,&rq);
        std :: cout << winid1 << " "<< winid1<< " to "<< rq << " "<< id<< ":" << received_str << "\n";
        std::string message1="Thank You!";
        MPI_Send(message1.c_str(), message1.size() + 1,MPI_CHAR,winid1,0,MPI_COMM_WORLD);
    }
    if(winid==-4){
        int winid1;
    MPI_Status fstatus;
    MPI_Recv( &winid1 , 1 , MPI_INT , 0 , 0 , MPI_COMM_WORLD , &fstatus);
  //  std :: cout << "pid " << winid1 << "\n";
    int winid2;
    MPI_Status fstatus1;
    MPI_Recv( &winid2 , 1 , MPI_INT , 0 , 0 , MPI_COMM_WORLD , &fstatus1);
 //   std :: cout << "pid " << winid2 << "\n";
    int winid11;
    MPI_Status fstatus2;
    MPI_Recv( &winid11, 1 , MPI_INT , 0 , 0 , MPI_COMM_WORLD , &fstatus2);
  //  std :: cout << "pid " << winid11 << "\n";
      MPI_Status st,st1,st2;
 char received_message[MAX_BUFFER_SIZE];
 char received_message1[MAX_BUFFER_SIZE];
 char received_message2[MAX_BUFFER_SIZE];


        MPI_Recv(received_message,sizeof(received_message), MPI_CHAR, winid1, 0, MPI_COMM_WORLD, &st);
        std::string received_str(received_message);
        int rq;
        MPI_Comm_rank(MPI_COMM_WORLD,&rq);
        std :: cout << winid1 << " "<< winid1<< " to "<< rq << " "<< id<< ":" << received_str << "\n";
        std::string message1="Thank You!";
        MPI_Send(message1.c_str(), message1.size() + 1,MPI_CHAR,winid1,0,MPI_COMM_WORLD);
        MPI_Recv(received_message1,sizeof(received_message1), MPI_CHAR, winid2, 0, MPI_COMM_WORLD, &st1);
        std::string received_str1(received_message1);
          std :: cout << winid2 << " "<< winid2<< " to "<< rq << " "<< id<< ":" << received_str1 << "\n";
         

        MPI_Send(message1.c_str(), message1.size() + 1,MPI_CHAR,winid2,0,MPI_COMM_WORLD);
        MPI_Recv(received_message2,sizeof(received_message2), MPI_CHAR, winid11, 0, MPI_COMM_WORLD, &st2);
        std::string received_str2(received_message2);
        std :: cout << winid11 << " "<< winid11<< " to "<< rq << " "<< id<< ":" << received_str2 << "\n";
       

        MPI_Send(message1.c_str(), message1.size() + 1,MPI_CHAR,winid11,0,MPI_COMM_WORLD);
    }
 
    if(winid>0){
            std::string message1="Congratulations!";

        MPI_Send(message1.c_str(), message1.size() + 1,MPI_CHAR,winid,0,MPI_COMM_WORLD);

          char received_message[MAX_BUFFER_SIZE];
        MPI_Recv(received_message,sizeof(received_message), MPI_CHAR,winid, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::string received_str(received_message);
        int rq;
        MPI_Comm_rank(MPI_COMM_WORLD,&rq);
         std :: cout << winid << " "<< winid<< " to "<< rq << " "<< id<< ":" << received_str << "\n";

    }
   
    }

   

    
}

void closeSocket(int clientSocket) {
    close(clientSocket);
}

void processLineAndSendRequest(const char *line) {
    int clientSocket = createSocket();
    connectToServer(clientSocket);
    sendData(clientSocket, line);
    receiveAndWriteToFile(clientSocket);
    closeSocket(clientSocket);
}


int main(int argc, char *argv[]) {
    int rank, size;
    int pp=0,pre=0;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    Court t[size];  
    FILE *csvFile, *outputFile;
    char line[MAX_BUFFER_SIZE];
        pthread_mutex_init(&mutex,NULL);
    if (rank == 0) {
        // Rank 0 reads the file, determines the number of lines, and broadcasts the count to all processes
        if ((csvFile = fopen("test_case.csv", "r")) == NULL) {
            perror("Error opening CSV file");
            exit(EXIT_FAILURE);
        }

        int count = 0;
        fgets(line, sizeof(line), csvFile) ;
        while (fgets(line, sizeof(line), csvFile) != NULL) {
            count++;
        }

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
    int totline;
    totline=count;
    char buff[1024];
    int y1=0;
    while(1){

    ssize_t bytesRead = recv(clientSocket1, buff, sizeof(buff) - 1, 0);
    if (bytesRead == -1) {
        perror("Error receiving data");
        close(clientSocket1);
        exit(EXIT_FAILURE);
    }
    if(bytesRead>0)
    y1=1;
    if(y1==1)
    break;
    }
    close(clientSocket1);

     
        if ((csvFile = fopen("test_case.csv", "r")) == NULL) {
            perror("Error opening CSV file");
            exit(EXIT_FAILURE);
        }

        fgets(line, sizeof(line), csvFile);  // Assuming this is a header line
        


    
        for (int i = 1; i < size; i++) {
            char ch[MAX_BUFFER_SIZE];
            if (fgets(line, sizeof(line), csvFile) != NULL||count>0) {
                line[strcspn(line,"\n")]='\0';
                sscanf(line,"%s",ch);
               // printf("%s %d \n",ch ,i);
                MPI_Send(ch, MAX_BUFFER_SIZE, MPI_CHAR, i, 0, MPI_COMM_WORLD);
              
              sleep(5);
            }
        }

        fclose(csvFile);


    char buf[MAX_BUFFER_SIZE];
  int  ii=0;
    while(ii<19)
    {
     MPI_Status stat1;
                MPI_Request re;
                MPI_Recv( buf , MAX_BUFFER_SIZE , MPI_CHAR ,MPI_ANY_SOURCE ,0, MPI_COMM_WORLD,&stat1);
                    char* type[6];
          const char delimiter[] = ",";

//     // Use strtok to split the string
               char *token = strtok(buf, delimiter);
                int i=0; 
//     // Loop through the tokens and print them
    while (token != NULL) {
        //printf("%s\n", token);
        
        type[i]=token;
       // printf("%s\n", type[i]);
        i++;
        token = strtok(NULL, delimiter);
    }
    
    int id=atoi(type[0]);
    int st=atoi(type[1]);
    int et=atoi(type[2]);
    //  std::cout<< et<<" "<<v<<" "<<idd<< "\n";
    if(et==-1)
    {
           // std::cout<< id<<" "<<st<<" "<<et<< "\n";
           
            int rank1=stat1.MPI_SOURCE;
            int sbuf=-1;
            MPI_Send( &sbuf, 1, MPI_INT , rank1 , 0 , MPI_COMM_WORLD);

    }
    else{
       
        char* p =type[4];
      
         int id1=atoi(type[5]);
         int court=atoi(type[3]);
        // char c=type[3];
        int rank2=stat1.MPI_SOURCE;
      //   std:: cout << id << " "<<st << " "<< et<< " " << court << "  "<< type[4] << "\n";
         for(int y=0;y<size;y++){
            if(t[y].game_end==0){
                t[y].game_end=et;
                t[y].court_num=court;
                if(id1==20)
                t[y].total=2;
                else
                t[y].total=4;
                t[y].count++;
                t[y].pids.push_back(s(id,rank2));
                break;
            }
            if(t[y].game_end>0){
                    if(t[y].game_end==et&&t[y].court_num==court){
                            t[y].pids.push_back(s(id,rank2));
                            t[y].count+=1;
                          
                        
                        break;     
                    }
                   
            }
         }
            
    }
ii++;

    }
    for(int y=0;y<size;y++)
    {

            if(t[y].game_end>0){
                    
                         int wrank=t[y].pids[0].prank;
                         int wid=t[y].pids[0].id;
                            for(int u=1;u<t[y].total;u++){
                         
             int prank1=t[y].pids[u].prank;
                          MPI_Send( &wrank,1 , MPI_INT , prank1 , 0 , MPI_COMM_WORLD);
                            }
                            int tre;
                        if(t[y].total==2)    
                         tre=-2;
                           else
                          tre=-4;
              
            
                          MPI_Send( &tre,1, MPI_INT , wrank , 0 , MPI_COMM_WORLD);
                           for(int u=1;u<t[y].total;u++){
                             int prank1=t[y].pids[u].prank;
                             MPI_Send( &prank1,1, MPI_INT , wrank , 0 , MPI_COMM_WORLD);
                           }
                             
                            
                    
            }


    }

    }
    else {
          char line1[MAX_BUFFER_SIZE];
            MPI_Recv(line1, MAX_BUFFER_SIZE, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
               processLineAndSendRequest(line1);
            
        }
        
    
    
    MPI_Finalize();

    return 0;
}

//creating process for each line so rank is equal to the id of process
//to compile server file #g++ -fopenmp ser.cpp -o s
// to run server file # ./s
// to compile client file mpic++ cl1.cpp -o cl
// to run mpirun -np 20 ./cl
// 20 is total no of line +1 as rank 0 i used for communication and other purpose
// in input file 1st line will be heading type thing
