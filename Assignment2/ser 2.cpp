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
#include <queue>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <omp.h>
#include <algorithm>
#define MAX_BUFFER_SIZE 2048
#define Total_Court 4
#define PORT 8081
FILE *inFile,*outputFile;
char line[MAX_BUFFER_SIZE];
char ch[1024];

int num_thread=0;



//FILE *inFile,*outputFile;
//char line[MAX_BUFFER_SIZE];
struct v{
    int id;
    int time;
    //char gender;
    int fd;
    char preference;
    v(int _id, int _time, int _fd,char _preference)
        : id(_id), time(_time),fd(_fd), preference(_preference) {}

};
struct s{
    int id;
    int time;
    char gender;
    char preference;
    int fd;
    s(int _id, int _time, char _gender, char _preference, int _fd)
        : id(_id), time(_time), gender(_gender), preference(_preference), fd(_fd) {}

};

struct Court {
    int start;
    int game_end=-1;
    int court_num;
    int count=0;
    int max_time;
    char p;
    std::vector<s> pids;
};


Court T[Total_Court];
int l_clock=0,com_clock=0;
std::queue<s> single_Queue;
std::queue<s> double_Queue;
std::queue<s> partialsingle_Queue;
std::queue<s> partialdouble_Queue;
int count=0;

void addQueue(int id, int t,char g,char p,int clsocket){
  
  
  
  //std :: cout << id << p << "\n"; 

    

        if(p=='S'){       
        single_Queue.push(s(id,t,g,p,clsocket));        
        }
        if(p=='D'){
        double_Queue.push(s(id,t,g,p,clsocket));    
        }
        if(p=='b'){
        partialsingle_Queue.push(s(id,t,g,p,clsocket));         
        }
        if(p=='B'){
        partialdouble_Queue.push(s(id,t,g,p,clsocket));      
        }
 

}
    
    void handleClient(int clsocket){ 
       
        
     //   std :: cout << "cls" << clsocket << "\n";
        char buffer[1024];
     
            ssize_t bytesRead = recv(clsocket, buffer, sizeof(buffer) - 1, 0);
 if (bytesRead == -1) {
                perror("Error receiving data1");
                close(clsocket);
                //continue; // Continue to the next iteration
            }
            if(bytesRead>0)
            {

        
             char* type[4];
          const char delimiter[] = ",";

//     // Use strtok to split the string
               char *token = strtok(buffer, delimiter);
                int i=0; 
//     // Loop through the tokens and print them
    while (token != NULL) {
       // printf("%s\n", token);
        type[i]=token;
        i++;
        token = strtok(NULL, delimiter);
    }
    //printf("%s %s \n",type[2],type[3]);
        char* id=type[0];
        char* timeslot=type[1];
        char* g=type[2];
        char* p =type[3];
         int id1=atoi(id);
         int time=atoi(timeslot);
         
       
         
             addQueue(id1,time,*g,*p,clsocket);
         
            
          
  //  } 
            }  
       
     
    }
 
 
 
void assignT(int pid,int start,char gen,char pref,int i,int fd){
    if(T[i].count==0){
          T[i].court_num=i+1;
          if(pref=='S'|| pref=='b'){
            if(gen=='F')
            T[i].max_time=5;
            else
            T[i].max_time=10;
          }
          else{
             if(gen=='F')
            T[i].max_time=10;
            else
            T[i].max_time=15;

          }
            T[i].count=1;
            T[i].pids.push_back(s(pid,start,gen,pref,fd));
            if(pref=='b'||pref=='S')
            T[i].p='S';
            else
            T[i].p='D';

}
    else{
            int t;
              if(pref=='S'|| pref=='b'){
            if(gen=='F')
            t=5;
            else
            t=10;
          }
          if(pref=='D'||pref=='B'){
             if(gen=='F')
            t=10;
            else
            t=15;

          }
            if(T[i].max_time<=t)
            T[i].max_time=t;
            T[i].pids.push_back(s(pid,start,gen,pref,fd));
             #pragma omp atomic
            T[i].count+=1;
            if(T[i].p=='S'&&T[i].count==2)
            {
                 T[i].start=l_clock;
                 T[i].game_end=T[i].start+T[i].max_time;
                
         /*   std:: cout << T[i].start << " " << T[i].game_end << " "<< T[i].court_num << " " ;
            for(int j=0;j<2;j++){
                std::cout << T[i].pids[j].id <<" ";
            }
            std::cout << "\n";
            */
            
            }
            
                if(T[i].p=='D'&& T[i].count==4 )
            {
                T[i].start=l_clock;
                T[i].game_end=T[i].start+T[i].max_time;
                /*  std:: cout << T[i].start << " " << T[i].game_end << " "<< T[i].court_num << " " ;
               for(int j=0;j<4;j++){
                std::cout << T[i].pids[j].id <<" ";
            }
            std::cout << "\n";
            */
                
            }   
    }
}
 
int assign_court(int id1,int time,char gen,char pr,int fd){
  
for(int i=0;i<4;i++){
    if(T[i].count==0){

         assignT(id1,time,gen,pr,i,fd);  
         return 1;
    }
    if(pr=='b'||pr=='S'){
        if((T[i].p=='S')&&(T[i].count<2)){
         assignT(id1,time,gen,pr,i,fd);  
         return 1;
    }
}
     if((pr=='B'||pr=='D')&&(T[i].p=='D')&&T[i].count<4){
         assignT(id1,time,gen,pr,i,fd);  
         return 1;
    }
}
   
   
   for(int i=0;i<4;i++){
   if(pr=='b'){
    if(T[i].count<4&&T[i].p=='D'){
         assignT(id1,time,gen,pr,i,fd); 
         return 1;
    }
   }
  if(pr=='B'){
    if(T[i].count<2&&T[i].p=='S'){
         assignT(id1,time,gen,pr,i,fd); 
         return 1;
    }

  }

   }
    return 0; 
   
    }

void service(){
       // read_file(); 
    //  std::cout <<  "Start Time,game end,Court number,Number of player," << "\n";
       
       int f,common=1;
  
    while(1){
        #pragma omp parallel
        {
            #pragma omp single
            {
                #pragma omp task
                {
      
         for(int i=0;i<4;i++){
              int co=0;
                    if(l_clock==T[i].game_end){
                         // std:: cout << T[i].start << " " << T[i].game_end << " "<< T[i].court_num << " " ;
                          int no;
                          if(T[i].p=='S')
                          no=2;
                          else
                          no =4;
               for(int j=0;j<no;j++){
                  char response[MAX_BUFFER_SIZE];
            //snprintf(response, sizeof(response), "%d", result);
            size_t offset=0;
          //  char c='"';
             offset += snprintf(response + offset, sizeof(response) - offset, "%d %d %d %d %c %s",T[i].pids[j].id,T[i].start,T[i].game_end,T[i].court_num,T[i].pids[j].preference,"          ");
             if (offset >= sizeof(response)) {
            fprintf(stderr, "Buffer overflow detected!\n");
            //break;  // You may want to handle this situation appropriately
        }
        #pragma omp critical
        {
          send(T[i].pids[j].fd, response, offset, 0);
        }
                std::cout << T[i].pids[j].id <<" "<< T[i].start << " "<< T[i].game_end << " "<< T[i].court_num<< " "<< T[i].pids[j].preference ;
                 std::cout << "\n";
                }
               
                        T[i].count=0;
                        T[i].pids.clear();
                    }
                    if(T[i].count<2&&T[i].p=='S')
                    {                      
                        
                         for(int j=0;j<T[i].count;j++){
                            if(l_clock-T[i].pids[j].time>=30){
                                 char response[MAX_BUFFER_SIZE];
           
            size_t offset=0;
        
             offset += snprintf(response + offset, sizeof(response) - offset, "%d %d %d %s %s ",T[i].pids[j].id,T[i].pids[j].time+30,-1," ","         ");
             if (offset >= sizeof(response)) {
            fprintf(stderr, "Buffer overflow detected!\n");
            //break;  // You may want to handle this situation appropriately
        }
        #pragma omp critical
        {
            int se= send(T[i].pids[j].fd, response, offset, 0);
        }
                std::cout << T[i].pids[j].id <<" "<< l_clock <<" "<< "-1";
                T[i].count=T[i].count-1;
                           co++;

                            }

            }
            while(co>0){
             T[i].pids.erase(T[i].pids.begin());
             co--;
            }

                    }
                     if(T[i].count<4&&T[i].p=='D')
                    {
                           for(int j=0;j<T[i].count;j++){
                            if(l_clock-T[i].pids[j].time>=30){
                                 char response[MAX_BUFFER_SIZE];
           
            size_t offset=0;
        
             offset += snprintf(response + offset, sizeof(response) - offset, "%d %d %d %s %s ",T[i].pids[j].id,T[i].pids[j].time+30,-1,"    ","     ");
             if (offset >= sizeof(response)) {
            fprintf(stderr, "Buffer overflow detected!\n");
            //break;  // You may want to handle this situation appropriately
        }
           #pragma omp critical
           {
          send(T[i].pids[j].fd, response, offset, 0);  
           }
                std::cout << T[i].pids[j].id <<" "<< l_clock <<" "<< "-1" << "\n";
                T[i].count=T[i].count-1;
                            co++;

                            }

            }
            while(co>0){
             T[i].pids.erase(T[i].pids.begin());
             co--;
            }
                    }

                }
            while(!single_Queue.empty()){
               
                if(single_Queue.front().time<=l_clock){
                 f=assign_court(single_Queue.front().id,single_Queue.front().time,single_Queue.front().gender,single_Queue.front().preference,single_Queue.front().fd);
             // std :: cout << single_Queue.front().id << single_Queue.front().preference << "\n";
              if(f==1)
                 single_Queue.pop();
                 else
                 break;
                
                }
                else
                break;
            }
            while(!double_Queue.empty()){
                if(double_Queue.front().time<=l_clock){
                     f=assign_court(double_Queue.front().id,double_Queue.front().time,double_Queue.front().gender,double_Queue.front().preference,double_Queue.front().fd);
                    if(f==1)
               double_Queue.pop();
               else
               break;
                }
                else
                break;
            }
            while(!partialsingle_Queue.empty()){
                if(partialsingle_Queue.front().time<=l_clock){
                  f=assign_court(partialsingle_Queue.front().id,partialsingle_Queue.front().time,partialsingle_Queue.front().gender,partialsingle_Queue.front().preference,partialsingle_Queue.front().fd);
                  if(f==1)
                   partialsingle_Queue.pop();
                   else
                   break;

            }
            else
            break;

            }
            while(!partialdouble_Queue.empty()){
                if(partialdouble_Queue.front().time<=l_clock){
                     f=assign_court(partialdouble_Queue.front().id,partialdouble_Queue.front().time,partialdouble_Queue.front().gender,partialdouble_Queue.front().preference,partialdouble_Queue.front().fd);
                       if(f==1)
                       partialdouble_Queue.pop();
                       else
                       break;
                }
                break;
            }
            }
        }
        }
            if((single_Queue.empty()&&double_Queue.empty()&&partialdouble_Queue.empty()&&partialsingle_Queue.empty())&&(T[1].count==0&&T[0].count==0&&T[2].count==0&&T[3].count==0)){
                    com_clock=1;
            }
            if(com_clock==1)
            break;
        #pragma omp atomic
            l_clock++;

   
    }
 //  std :: cout << com_clock;
}


int main(){
       int f,common=1;
         
   
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

     if ((newSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrLen)) < 0) {
                //f1=1;
           perror("Accept failed");
            
            //continue; // Continue to the next iteration
        }
      
        char buffer1[1024];
            ssize_t bytesRead1 = recv(newSocket, buffer1, sizeof(buffer1) - 1, 0);
 if (bytesRead1 == -1) {
                perror("Error receiving data");
                close(newSocket);
               // continue; // Continue to the next iteration
            }
          char* type1[1];
          const char delimiter[] = ",";

//     // Use strtok to split the string
               char *token1 = strtok(buffer1, delimiter);
                int i1=0; 
//     // Loop through the tokens and print them
    while (token1 != NULL) {
       // printf("%s\n", token);
        type1[i1]=token1;
        i1++;
        token1 = strtok(NULL, delimiter);
    }
    //printf("%s %s \n",type[2],type[3]);
        char* numt1=type1[0];
         int numt=atoi(numt1);
         std :: cout << numt << "\n";
        num_thread=numt;
     const char* response = "Hello, client! I got number of request.";
    send(newSocket, response, strlen(response), 0);

        close(newSocket);


          
     #pragma omp parallel for num_threads(num_thread)
    for(int i=0;i<num_thread;i++){
            //sleep(1);
         newSocket = 0;    
            if ((newSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrLen)) < 0) {
                //f1=1;
            perror("Accept failed");
            
            continue; // Continue to the next iteration
        }
       
       
     // std::cout << omp_get_thread_num() << "\n";
                
            int *csocket=(int *)malloc(sizeof(int));
            *csocket=newSocket;
            int clsocket=*((int*)csocket); 
         
          
       
              //  printf("asdf %d\n",clsocket);
              
            handleClient(clsocket);
    }
  
    service();

    return 0;  
}



                              
