#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

struct ThreadArgs {
    int start_row;
    int end_row;
   int **a;
};
pthread_mutex_t mutex;
#define n 2048 // Adjust the size based on your requirements
struct timeval Tlstart;
struct timeval Tlend;
float timedifference_msec(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}

static int **a;
int **b;
int **b1;
int **c;
int **d;
int **f;
int **t1,**t2,**t3,**t4,**t5,**t6,**t7,**t8;

void allocateMatrix(int rows, int cols) {
    // Allocate memory for rows
    c= (int **)malloc(rows * sizeof(int *));
    d=(int **)malloc(rows * sizeof(int *));
  f= (int **)malloc(rows * sizeof(int *));
    if (c == NULL||d==NULL||f==NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for columns
    for (int i = 0; i < rows; i++) {
        c[i] = (int *)malloc(cols * sizeof(int));
        d[i]=(int *)malloc(cols * sizeof(int));
        f[i]=(int *)malloc(cols * sizeof(int));
        if (c[i] == NULL||d[i]==NULL||f[i]==NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }
    }
}

void allocateMatrix1(int rows, int cols) {
    // Allocate memory for rows
    a= (int **)malloc(rows * sizeof(int *));
    b=(int **)malloc(rows * sizeof(int *));
     b1=(int **)malloc(rows * sizeof(int *));
    
    if (a == NULL||b==NULL||b1==NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for columns
    for (int i = 0; i < rows; i++) {
        a[i] = (int *)malloc(cols * sizeof(int));
        b[i]=(int *)malloc(cols * sizeof(int));
        b1[i]=(int *)malloc(cols * sizeof(int));
        if (a[i] == NULL||b[i]==NULL||b1==NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }
    }
}


void freeMatrix(int rows) {
    for (int i = 0; i < rows; i++) {
        free(d[i]);
         free(c[i]);
    }
    free(d);
    free(c);
}
void freeMatrix1(int rows) {
    for (int i = 0; i < rows; i++) {
       
        free(b[i]);
        free(f[i]);
    }
   
    free(b);
    free(f);
}

int findsum(int *a)
{
     int i;
     int sum=0;
     for(i=0;i<8;i++)
     sum=sum+a[i];
     
     return sum;
}

void* multiply_rows(void* arg) {
  //  pthread_mutex_lock(&mutex);
    struct ThreadArgs* args = (struct ThreadArgs*)arg;
    int o=args->start_row,r1=args->end_row;
    int **c1=args->a;
 //  printf("%d %d \n",o, r1);
 // matrixMultiplicationSIMD(b,f,a,o,r);
   int **a=b;
   int **b=b1;
   
   int i,j,k,p,q,r;
   
        int start_i=o,end_i=r1-1,start_j=0,end_j=n-1,start_k=0,end_k=n-1;

 for(i=start_i,p=0;i<=end_i;i++,p++)
        {
             for(j=start_j,q=0;j<=end_j;j++,q++)
             { 
                __m256i c = _mm256_set_epi32(0,0,0,0, 0,0, 0,0);
                int *ptr1=*(a+i)+start_k;
                int *ptr2=*(b+j)+start_k;
                 for(k=start_k,r=0;k<=end_k;k=k+8,r++)
                 {
                   __m256i mask = _mm256_setr_epi32(-20, -72, -48, -9, -100, -3, -5, -8);  
                   __m256i m1 = _mm256_maskload_epi32(ptr1, mask);
                   __m256i m2 = _mm256_maskload_epi32(ptr2, mask);
                   __m256i result = _mm256_mullo_epi32(m1, m2);
                   c=_mm256_add_epi32(c,result);
                   ptr1=ptr1+8;
                   ptr2=ptr2+8;
                 }
                 
                 c1[p][q]=findsum((int*)(&c));
                // a[p][q]=c1[p][q];
                // printf("%d \t",a[p][q]);
             }
            // printf("\n");
        }
//pthread_mutex_unlock(&mutex);


    pthread_exit(NULL);
}






   







void matrixSumAVX(int** a, int** b, int** c, int n1) {
          
         __m256i mask = _mm256_setr_epi32(-20, -72, -48, -9, -100, -3, -5, -8);
for(int i=0;i<n1;i++)
{
    int *ptr1=a[i];
    int *ptr2=b[i];
    int *ptr3=c[i];
    for(int j=0;j<n1;j=j+8)
    {
              
         __m256 avx_a = _mm256_loadu_ps((float*)ptr1);
        __m256 avx_b = _mm256_loadu_ps((float*)ptr2);

       __m256 avx_result = _mm256_add_ps(avx_a, avx_b);
        
        _mm256_storeu_ps((float*)ptr3, avx_result);
              //ptr3=(int*)pfloat;
              ptr1=ptr1+8;
              ptr2=ptr2+8;
              
              ptr3=ptr3+8;
           
   

        }
    }
}
 void transpose(int** a,int** b){
   for(int i=0;i<n;i++){
    for(int j=0;j<n;j++){
       a[j][i]=b[i][j];

    }
   // printf("\n");
} 

 }

int main() {

       gettimeofday(&Tlstart, NULL);
     allocateMatrix(n,n);

     for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
           // b[i][j]=rand()%10;
            c[i][j]=rand()%10;
            d[i][j]=rand()%10;
             
            
        }
      
    }
//       printf("A\n\n\n");
// for(int i=0;i<n;i++){
//     for(int j=0;j<n;j++){
//         printf("%d \t",c[i][j]);

//     }
//     printf("\n");
// }
//      printf("A\n\n\n");
// for(int i=0;i<n;i++){
//     for(int j=0;j<n;j++){
//         printf("%d \t",d[i][j]);

//     }
//     printf("\n");
// }
   matrixSumAVX(c,d,f,n);
//      printf("F\n\n\n");
// for(int i=0;i<n;i++){
//     for(int j=0;j<n;j++){
//         printf("%d \t",f[i][j]);

//     }
//     printf("\n");
// }
   
   
       freeMatrix(n);
   
   allocateMatrix1(n,n);


   for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
           b[i][j]=rand()%10;
            a[i][j]=0;
        }
      
    }
    transpose(b1,f);
//       printf("\n\nB1\n\n\n");
//     for(int i=0;i<n;i++){
//     for(int j=0;j<n;j++){
//         printf("%d \t",b1[i][j]);

//     }
//     printf("\n");
// }
//          printf("B\n\n\n");
// for(int i=0;i<n;i++){
//     for(int j=0;j<n;j++){
//         printf("%d \t",b[i][j]);

//     }
//     printf("\n");
// }


      int num_threads = 8; 

    
    int ra = n / num_threads;
    int rows_per_thread=ra;
        
    t1=(int**)malloc(ra*sizeof(int*));
    t2=(int**)malloc(ra*sizeof(int*));
    t3=(int**)malloc(ra*sizeof(int*));
    t4=(int**)malloc(ra*sizeof(int*));
    t5=(int**)malloc(ra*sizeof(int*));
    t6=(int**)malloc(ra*sizeof(int*));
    t7=(int**)malloc(ra*sizeof(int*));
    t8=(int**)malloc(ra*sizeof(int*));
    
     for(int i=0;i<ra;i++)
    {
       t1[i]=(int*)malloc(n*sizeof(int));
       t2[i]=(int*)malloc(n*sizeof(int));
       t3[i]=(int*)malloc(n*sizeof(int));
       t4[i]=(int*)malloc(n*sizeof(int));
       t5[i]=(int*)malloc(n*sizeof(int));
       t6[i]=(int*)malloc(n*sizeof(int));
       t7[i]=(int*)malloc(n*sizeof(int));
       t8[i]=(int*)malloc(n*sizeof(int));
    }

    //  printf("%d\n",rows_per_thread);
    // Create and start threads
   pthread_t threads[num_threads];
   struct ThreadArgs thread_args[num_threads];
    int i=0;
    {
    thread_args[0].start_row=i*rows_per_thread;
    thread_args[0].a=t1;
    i++;
    thread_args[0].end_row=i*rows_per_thread;
    thread_args[1].start_row=i*rows_per_thread;
       thread_args[1].a=t2;
    i++;
    thread_args[1].end_row=i*rows_per_thread;
    thread_args[2].start_row=i*rows_per_thread;
    thread_args[2].a=t3;
    i++;
    thread_args[2].end_row=i*rows_per_thread;
    thread_args[3].start_row=i*rows_per_thread;
    thread_args[3].a=t4;
    i++;
    thread_args[3].end_row=i*rows_per_thread;
    thread_args[4].start_row=i*rows_per_thread;
    thread_args[4].a=t5;
    i++;
    thread_args[4].end_row=i*rows_per_thread;
    thread_args[5].start_row=i*rows_per_thread;
    thread_args[5].a=t6;
    i++;
    thread_args[5].end_row=i*rows_per_thread;
    thread_args[6].start_row=i*rows_per_thread;
    thread_args[6].a=t7;
    i++;
    thread_args[6].end_row=i*rows_per_thread;
    thread_args[7].start_row=i*rows_per_thread;
    thread_args[7].a=t8;
    i++;
    thread_args[7].end_row=i*rows_per_thread;
    }
   
    if(pthread_mutex_init(&mutex, NULL))
    {
        printf("Unable to initialize a mutex\n");
        return -1;
    }
    pthread_create( &threads[0], NULL, multiply_rows,(void*) &thread_args[0]);

    pthread_create( &threads[1], NULL, multiply_rows,(void*) &thread_args[1]);
    pthread_create( &threads[2], NULL, multiply_rows,(void*) &thread_args[2]);
    pthread_create( &threads[3], NULL, multiply_rows,(void*) &thread_args[3]);
    pthread_create( &threads[4], NULL, multiply_rows,(void*) &thread_args[4]);
    pthread_create( &threads[5], NULL, multiply_rows,(void*) &thread_args[5]);
    pthread_create( &threads[6], NULL, multiply_rows,(void*) &thread_args[6]);
    pthread_create( &threads[7], NULL, multiply_rows,(void*) &thread_args[7]);
    
   
   

        pthread_join(threads[0], NULL);
        pthread_join(threads[1], NULL);
        pthread_join(threads[2], NULL);
        pthread_join(threads[3], NULL);
        pthread_join(threads[4], NULL);
        pthread_join(threads[5], NULL);
            pthread_join(threads[6], NULL);
            pthread_join(threads[7], NULL);
    
    freeMatrix1(n);
    int r=0;
        
   
            for(int i=0;i<ra;i++){
                for(int j=0;j<n;j++){

                    a[i][j]=t1[i][j];
                }
            }
                  for(int i=0;i<ra;i++){
                for(int j=0;j<n;j++){

                    a[ra+i][j]=t2[i][j];
                }
            }
                 for(int i=0;i<ra;i++){
                for(int j=0;j<n;j++){

                    a[2*ra+i][j]=t3[i][j];
                }
            }
                for(int i=0;i<ra;i++){
                for(int j=0;j<n;j++){

                    a[3*ra+i][j]=t4[i][j];
                }
            }
             for(int i=0;i<ra;i++){
                for(int j=0;j<n;j++){

                    a[4*ra+i][j]=t5[i][j];
                }
            }
            for(int i=0;i<ra;i++){
                for(int j=0;j<n;j++){

                    a[5*ra+i][j]=t6[i][j];
                }
            }
            for(int i=0;i<ra;i++){
                for(int j=0;j<n;j++){

                    a[6*ra+i][j]=t7[i][j];
                }
            }
            for(int i=0;i<ra;i++){
                for(int j=0;j<n;j++){

                    a[7*ra+i][j]=t8[i][j];
                }
            }
    


//      printf("A\n\n\n");
// for(int i=0;i<n;i++){
//     for(int j=0;j<n;j++){
//         printf("%d \t",a[i][j]);

//     }
//     printf("\n");
// }



 gettimeofday(&Tlend, NULL);
   
    
   
	float response_time = timedifference_msec(Tlstart, Tlend);
	printf("Time taken= %f \n",response_time/1000);

    

    
    return 0;
}

