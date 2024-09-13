#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <queue>
#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
#define MAX_BUFFER_SIZE 2048
#define Total_Court 4


FILE *inFile,*outputFile;
char line[MAX_BUFFER_SIZE];
char ch[1024];

struct Court {
    int start;
    int game_end=0;
    int court_num;
    int count=0;
    int max_time;
    char p;
    std::vector<int> pids;
};
struct s{
    int id;
    int time;
    char gender;
    char preference;
    s(int _id, int _time, char _gender, char _preference)
        : id(_id), time(_time), gender(_gender), preference(_preference) {}

};

Court T[Total_Court];
int l_clock=0,com_clock=0;
std::queue<s> single_Queue;
std::queue<s> double_Queue;
std::queue<s> partialsingle_Queue;
std::queue<s> partialdouble_Queue;
int count=0;

void addQueue(int id, int t,char g,char p){
        if(p=='S'){       
        single_Queue.push(s(id,t,g,p));        
        }
        if(p=='D'){
        double_Queue.push(s(id,t,g,p));    
        }
        if(p=='b'){
        partialsingle_Queue.push(s(id,t,g,p));         
        }
        if(p=='B'){
        partialdouble_Queue.push(s(id,t,g,p));      
        }

}

void read_file(){
    

    if ((inFile = fopen("test_case.csv", "r")) == NULL) {
        perror("Error opening CSV file");
        exit(EXIT_FAILURE);
    }

    fgets(line, sizeof(line), inFile);  // Assuming this is a header line
    
   
    while (fgets(line, sizeof(line), inFile) != NULL) {
        line[strcspn(line, "\n")] = '\0'; // Remove newline characters
        sscanf(line, "%s ", ch);
            char* type[4];
      
    const char delimiter[] = ",";

//     // Use strtok to split the string
               char *token = strtok(ch, delimiter);
                int i=0;
//     // Loop through the tokens and print them
    while (token != NULL) {
       // printf("%s\n", token);
        type[i]=token;
        i++;
        token = strtok(NULL, delimiter);
    }
  //  printf("%s %s \n",type[2],type[3]);
        char* id=type[0];
        char* timeslot=type[1];
        char* g=type[2];
        char* p=type[3];
         int id1=atoi(id);
         int time=atoi(timeslot);
           // com_clock=time;
         addQueue(id1,time,*g,*p);
   
}
}


void assignT(int pid,int start,char gen,char pref,int i){
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
            T[i].pids.push_back(pid);
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
            T[i].pids.push_back(pid);
            T[i].count+=1;
            if(T[i].p=='S'&&T[i].count==2)
            {
                 T[i].start=l_clock;
                 T[i].game_end=T[i].start+T[i].max_time;
                
            std:: cout << T[i].start << " " << T[i].game_end << " "<< T[i].court_num << " " ;
            for(int e:T[i].pids){
                std::cout << e <<" ";
            }
            std::cout << "\n";
            
            }
            
                if(T[i].p=='D'&& T[i].count==4 )
            {
                T[i].start=l_clock;
                T[i].game_end=T[i].start+T[i].max_time;
                  std:: cout << T[i].start << " " << T[i].game_end << " "<< T[i].court_num << " " ;
                    for(int e:T[i].pids){
                std::cout << e <<" ";
            }
            std::cout << "\n";
                
            }   
    }
}
 
int assign_court(int id1,int time,char gen,char pr){
  
for(int i=0;i<4;i++){
    if(T[i].count==0){

         assignT(id1,time,gen,pr,i);  
         return 1;
    }
    if(pr=='b'||pr=='S'){
        if((T[i].p=='S')&&(T[i].count<2)){
         assignT(id1,time,gen,pr,i);  
         return 1;
    }
}
     if((pr=='B'||pr=='D')&&(T[i].p=='D')&&T[i].count<4){
         assignT(id1,time,gen,pr,i);  
         return 1;
    }
}
   
   
   for(int i=0;i<4;i++){
   if(pr=='b'){
    if(T[i].count<4&&T[i].p=='D'){
         assignT(id1,time,gen,pr,i); 
         return 1;
    }
   }
  if(pr=='B'){
    if(T[i].count<2&&T[i].p=='S'){
         assignT(id1,time,gen,pr,i); 
         return 1;
    }

  }

   }
    return 0; 
   
    }

int main(){
        read_file(); 
      std::cout <<  "Start Time,game end,Court number,Number of player," << "\n";
       
       int f,common=1;
  
    while(1){
         for(int i=0;i<4;i++){
                    if(l_clock==T[i].game_end){
                        T[i].count=0;
                        T[i].pids.clear();
                    }
                }
            while(!single_Queue.empty()){
               
                if(single_Queue.front().time<=l_clock){
                 f=assign_court(single_Queue.front().id,single_Queue.front().time,single_Queue.front().gender,single_Queue.front().preference);
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
                     f=assign_court(double_Queue.front().id,double_Queue.front().time,double_Queue.front().gender,double_Queue.front().preference);
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
                  f=assign_court(partialsingle_Queue.front().id,partialsingle_Queue.front().time,partialsingle_Queue.front().gender,partialsingle_Queue.front().preference);
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
                     f=assign_court(partialdouble_Queue.front().id,partialdouble_Queue.front().time,partialdouble_Queue.front().gender,partialdouble_Queue.front().preference);
                       if(f==1)
                       partialdouble_Queue.pop();
                       else
                       break;
                }
                break;
            }
            if((single_Queue.empty()&&double_Queue.empty()&&partialdouble_Queue.empty()&&partialsingle_Queue.empty())){
                    com_clock=1;
            }
            if(com_clock==1)
            break;

            l_clock++;

    }
 //  std :: cout << com_clock;
}