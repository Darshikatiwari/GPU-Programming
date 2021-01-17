#include <chrono>
#include <ctime>
#include <fstream>
using namespace std;

//transpose of matrix B
void B_transpose(int *dst, const int *src, int n) noexcept 
{
    size_t i,j,k;
    size_t block = 64;

    for (i = 0; i < n; i += block)  //block-wise access
    {
        for(j = 0; j < n; ++j) 
        {
            for(k = 0; k < block && i + k < n; ++k) 
            {
                dst[j*n + i + k] = src[(i + k)*n + j]; //assignment of the transposed values
            }
        }
    }
}


// single threaded function
void singleThread(int N, int *matA, int *matB, int *output)
{
    //Time Synchronization
    auto begin = TIME_NOW;

    //pointer to store transposed matrix
    int *dst = new int[N * N];
    
    // transpose calculation
    B_transpose(dst,matB, N);

    //transposed matrix B
    matB=dst;
    
    auto end = TIME_NOW;

    //Total Time taken by the loop to compute transpose
    auto transposetime=(double)TIME_DIFF(std::chrono::microseconds, begin, end) / 1000.0;
    cout << "\nTime for calculating Transpose: " << transposetime ;    
    
    begin = TIME_NOW;

    //DMM on the transposed matrix
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            output[i+j]+=matA[i*N+j]*matB[(N-i-1)*N+j];
        }
    }
    
    end = TIME_NOW;
    //Overall loop time calculation
    transposetime=(double)TIME_DIFF(std::chrono::microseconds, begin, end) / 1000.0;
    cout << "\nTime for executing Loop: " << transposetime;    

    
}
