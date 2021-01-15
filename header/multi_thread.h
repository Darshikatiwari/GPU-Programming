#include <pthread.h>
#include <ctime>
#include <chrono>
#include <fstream>
using namespace std;

#define TIME_NOW std::chrono::high_resolution_clock::now()
#define TIME_DIFF(gran, start, end) std::chrono::duration_cast<gran>(end - start).count()

#define THREADS 48
pthread_mutex_t alock;
pthread_mutex_t block;
int step =0;
int step1 = 0;


struct thd1 {
        int N;
        int *matA;
        int *matB;
        int *output;
    };

void* st(void * threadarg)
{
    //pthread_mutex_lock(&lock);
    struct thd1 *global = (struct thd1 *) threadarg;
    // global = (struct thd1 *) threadarg;
    
    pthread_mutex_lock(&alock);
     int core =step;
     step = step+1;
    int from =(core*global->N)/THREADS;
    int to =((core+1)*global->N)/THREADS;
    pthread_mutex_unlock(&alock);
    // Iterate over first half of output elements
    auto begin = TIME_NOW;
    for(int i = from; i < to; ++i) {
        int temp = 0;
        // Iterate over diagonal elements
        for(int j = 0; j < i + 1; ++j) {
            int rowA = j;
            int colA = i - j;
            int rowB = i - j;
            int colB = global->N - j - 1;
            temp += global->matA[rowA * global->N + colA] * global->matB[rowB * global->N + colB];
        }
        global->output[i] = temp;
    }
    auto end = TIME_NOW;
    //cout << "Loop 1: " << (double)TIME_DIFF(std::chrono::microseconds, begin, end) / 1000.0 << " ms\n"; 
    
    
    begin = TIME_NOW;

    pthread_mutex_lock(&block);
    int core1 =step1;
    step1=step1+1;
    int start =(core1*global->N)/THREADS +global->N;
    int stop=((core1+1)*global->N )/THREADS + global->N;
     if (core1== THREADS-1){
        stop = stop-1;
    }  
    pthread_mutex_unlock(&block);
   // cout<<"start: "<<start;
    //cout<<"stop: "<<stop;
      
    // Iterate over second half of output elements
    for(int i = start; i < stop; ++i) {
        int temp = 0;
        // Iterate over diagonal elements
        for(int j = 0; j < 2 * global->N - (i + 1); ++j) {
            int rowA = i + 1 + j - global->N;
            int colA = global->N - j - 1;
            int rowB = global->N - j - 1;
            int colB = 2 * global->N - j - 2 - i;
            temp += global->matA[rowA * global->N + colA] * global->matB[rowB * global->N + colB];
        }
        global->output[i] = temp;
    }

     end= TIME_NOW;
    //cout << "Loop 2: " << (double)TIME_DIFF(std::chrono::microseconds, begin, end) / 1000.0 << " ms\n"; 
    

//  for( int i = 0 ; i < global->N ; i ++)
//  { int t = i * global->N ; int p = global->N - i - 1 ; 
//  for ( int j = 0 ; j < global->N - i ; j ++)
//  { 
//      global->output [ i + j ]+= global->matA [ t + j ]* global->matB [ j * global->N +( p )]; 
     
//      //cout <<"\n for i "<<i+j <<" output is "<< output[i+j];
//       } }

// for ( int i = global->N - 1 ; i > 0 ; i --)
// { int t = i * global->N ; for ( int j = i - 1 ; j >= 0 ; j --)
// { int p = global->N - j - 1 ; global->output [ p + i ]+= global->matB [ t + j ]* global->matA [( p *global-> N )+ i ]; 
//    // cout <<"\n for i "<<i+j <<" output is "<< output[i+j]; 
//    } }

    //pthread_mutex_unlock(&lock);
    pthread_exit(NULL);
}


void multiThread(int N, int *matA, int *matB, int *output)
{
    int rc;
  
    // struct thd {
    //     int N =N;
    //     int *matA=matA;
    //     int *matB = matB;
    //     int *output = output;
    // };

    struct thd1 thread[THREADS];
    pthread_t tid[THREADS];

    //cout<<"sds"<<data->N;
    auto begin = TIME_NOW;
    for (int i =  0; i < THREADS; i++)
    {   thread[i].N =N;
    thread[i].matA=matA;
    thread[i].matB=matB;
    thread[i].output=output;   

        rc = pthread_create(&tid[i], NULL,st, (void *)&thread[i]);
        
    }
    auto end = TIME_NOW;
    cout << "Threads execution time: " << (double)TIME_DIFF(std::chrono::microseconds, begin, end) / 1000.0 << " ms\n"; 
    if (rc){
        printf("ERRoR:::");
    }
    for (int i = 0; i < THREADS; i++)
    {
         pthread_join(tid[i],NULL);
    }
    

}


